#  vim: set fileencoding=utf-8 :
import codecs
import logging
import os
import yaml

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from sklearn import cross_validation
from sklearn.cross_validation import ShuffleSplit, train_test_split
from sklearn.externals import joblib
# from sklearn.tree import DecisionTreeClassifier
# from sklearn.ensemble import AdaBoostClassifier
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.feature_selection import RFECV
from sklearn.learning_curve import learning_curve
from sklearn.metrics import roc_curve, auc
from sklearn import grid_search

from root_numpy import array2tree, root2array, rec2array, tree2array

from ttH.TauRoast.external.sklearn_to_tmva import gbr_to_tmva

matplotlib.style.use('ggplot')

CV = 4
NJOBS = 48


def load(config):
    datadir = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data')
    with open(os.path.join(datadir, 'mva.yaml')) as f:
        setup = yaml.load(f)
    return setup


def train(config):
    from ttH.TauRoast.processing import Process

    setup = load(config)
    fn = os.path.join(config.get("indir", config["outdir"]), "ntuple.root")

    outdir = os.path.join(config["outdir"], 'sklearn')
    if not os.path.exists(outdir):
        os.makedirs(outdir)

    signal = None
    for p in sum([Process.expand(n) for n in setup['signals']], []):
        logging.debug('reading {}'.format(p))
        s = rec2array(root2array(fn, str(p), setup['variables']))
        if signal is not None:
            signal = np.concatenate((signal, s))
        else:
            signal = s

    background = None
    for p in sum([Process.expand(n) for n in setup['backgrounds']], []):
        logging.debug('reading {}'.format(p))
        b = rec2array(root2array(fn, str(p), setup['variables']))
        if background is not None:
            background = np.concatenate((background, b))
        else:
            background = b

    plot_correlations(outdir, setup["variables"], signal, background)
    plot_inputs(outdir, setup["variables"], signal, background)

    x = np.concatenate((signal, background))
    y = np.concatenate((np.ones(signal.shape[0]),
                        np.zeros(background.shape[0])))

    # dt = DecisionTreeClassifier(max_depth=3,
    #                             min_samples_leaf=500)
    # bdt = AdaBoostClassifier(dt,
    #                          algorithm='SAMME',
    #                          n_estimators=800,
    #                          learning_rate=0.5)
    bdt = GradientBoostingClassifier(n_estimators=500,
                                     max_depth=3,
                                     subsample=0.5,
                                     max_features=0.5,
                                     min_samples_leaf=10,
                                     min_samples_split=20,
                                     learning_rate=0.02)

    logging.info("starting cross validation")
    scores = cross_validation.cross_val_score(bdt, x, y, scoring="roc_auc", n_jobs=NJOBS, cv=CV)
    out = u'training accuracy: {} ± {}\n'.format(scores.mean(), scores.std())
    with codecs.open(os.path.join(outdir, "log-accuracy.txt"), "w", encoding="utf8") as fd:
        fd.write(out)

    if 'selection' in setup.get('features', []):
        run_feature_elimination(outdir, bdt, x, y, setup)

    if 'parameters' in setup.get('features', []):
        run_grid_search(outdir, bdt, x, y)

    if 'learning' in setup.get('features', []):
        plot_learning_curve(outdir, bdt, x, y)

    logging.info("training bdt")
    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=.2)
    bdt.fit(x_train, y_train)

    out = u'Feature importance\n===================\n\n'
    for var, score in sorted(zip(setup['variables'], bdt.feature_importances_), key=lambda (x, y): y):
        out += '{:30}: {:>10.4f}\n'.format(var, score)
    with codecs.open(os.path.join(outdir, "log-feature-importance.txt"), "w", encoding="utf8") as fd:
        fd.write(out)

    fn = os.path.join(outdir, "bdt", "bdt.pkl")
    if not os.path.exists(os.path.dirname(fn)):
        os.makedirs(os.path.dirname(fn))
    joblib.dump(bdt, fn)

    df = pd.DataFrame(x_train, columns=setup["variables"])
    gbr_to_tmva(bdt, df, os.path.join(outdir, "weights.xml"))

    logging.info("creating roc, output")
    plot_roc(outdir, bdt, [(x_test, y_test, 'testing'), (x_train, y_train, 'training')])
    plot_output(outdir, bdt, [(x_test, y_test, 'testing'), (x_train, y_train, 'training')])


def evaluate(config, tree):
    setup = load(config)
    data = rec2array(tree2array(tree.raw(), setup["variables"]))

    fn = os.path.join(config.get("mvadir", config.get("indir", config["outdir"])), "sklearn", "bdt", "bdt.pkl")
    bdt = joblib.load(fn)
    scores = []
    if len(data) > 0:
        scores = bdt.decision_function(data)
    scores = np.array([(s,) for s in scores], [('bdt', 'float64')])
    tree.mva(array2tree(scores))


def run_feature_elimination(outdir, bdt, x, y, setup):
    logging.info("starting feature selection")
    rfecv = RFECV(estimator=bdt, step=1, cv=CV, scoring='roc_auc')  # new in 18.1: , n_jobs=NJOBS)
    rfecv.fit(x, y)

    plot_feature_elimination(outdir, rfecv)

    out = u'Feature selection\n=================\n\n'
    out += u'optimal feature count: {}\n\nranking\n-------\n'.format(rfecv.n_features_)
    for n, v in enumerate(setup["variables"]):
        out += u'{:30}: {:>5}\n'.format(v, rfecv.ranking_[n])
    with codecs.open(os.path.join(outdir, "log-feature-elimination.txt"), "w", encoding="utf8") as fd:
        fd.write(out)


def run_grid_search(outdir, bdt, x, y):
    logging.info('starting hyper-parameter optimization')
    param_grid = {
        "n_estimators": [500, 1000, 2000, 5000],
        "max_depth": [3, 4, 5, 6, 7, 8],
        "learning_rate": [.002, .005, .01, .02, .1, .2]
    }

    clf = grid_search.GridSearchCV(bdt, param_grid, cv=CV, scoring='roc_auc', n_jobs=NJOBS)
    clf.fit(x, y)

    out = '\nHyper-parameter optimization\n'
    out += '============================\n\n'
    out += 'Best estimator: {}\n'.format(clf.best_estimator_)
    out += '\nFull Scores\n'
    out += '-----------\n\n'
    for params, mean_score, scores in clf.grid_scores_:
        out += u'{:0.4f} (±{:0.4f}) for {}\n'.format(mean_score, scores.std(), params)
    with codecs.open(os.path.join(outdir, "log-hyper-parameters.txt"), "w", encoding="utf8") as fd:
        fd.write(out)


def plot_correlations(outdir, vars, sig, bkg):
    for data, label in ((sig, "Signal"), (bkg, "Background")):
        d = pd.DataFrame(data, columns=vars)
        sns.heatmap(d.corr(), annot=True, fmt=".2f", linewidth=.5)
        plt.title(label + " Correlations")
        plt.tight_layout()
        plt.savefig(os.path.join(outdir, 'correlations_{}.png'.format(label.lower())))
        plt.close()


def plot_feature_elimination(outdir, cls):
    plt.plot(range(1, len(cls.grid_scores_) + 1), cls.grid_scores_)
    plt.xlabel('# features')
    plt.ylabel('Score (ROC auc)')
    plt.savefig(os.path.join(outdir, 'feature_elimination.png'))


def plot_inputs(outdir, vars, sig, bkg):
    for n, var in enumerate(vars):
        _, bins = np.histogram(np.concatenate((sig[:, n], bkg[:, n])), bins=40)
        sns.distplot(bkg[:, n], bins=bins, kde=False, norm_hist=True, label='background')
        sns.distplot(sig[:, n], bins=bins, kde=False, norm_hist=True, label='signal')
        plt.title(var)
        plt.legend()
        plt.savefig(os.path.join(outdir, 'input_{}.png'.format(var)))
        plt.close()


def plot_learning_curve(outdir, cls, x, y):
    logging.info("creating learning curve")
    train_sizes, train_scores, test_scores = learning_curve(cls, x, y,
                                                            cv=ShuffleSplit(len(x), n_iter=100, test_size=1.0 / CV),
                                                            n_jobs=NJOBS,
                                                            train_sizes=np.linspace(.1, .9, 5),
                                                            scoring='roc_auc')
    train_scores_mean = np.mean(train_scores, axis=1)
    train_scores_std = np.std(train_scores, axis=1)
    test_scores_mean = np.mean(test_scores, axis=1)
    test_scores_std = np.std(test_scores, axis=1)

    plt.fill_between(train_sizes,
                     train_scores_mean - train_scores_std,
                     train_scores_mean + train_scores_std,
                     alpha=.2, color='r')
    plt.fill_between(train_sizes,
                     test_scores_mean - test_scores_std,
                     test_scores_mean + test_scores_std,
                     alpha=.2, color='g')
    plt.plot(train_sizes, train_scores_mean, 'o-', color='r', label='Training score')
    plt.plot(train_sizes, test_scores_mean, 'o-', color='g', label='Cross-validation score')

    plt.xlabel("Sample size")
    plt.ylabel("Score (ROC area)")

    plt.legend()
    plt.savefig(os.path.join(outdir, 'learning_curve.png'))
    plt.close()


def plot_output(outdir, cls, data):
    outputs = []
    for x, y, label in data:
        sig = cls.decision_function(x[y > .5]).ravel()
        bkg = cls.decision_function(x[y < .5]).ravel()
        outputs.append((sig, bkg, label))

    bins = np.linspace(min(np.min(v) for v in (sig, bkg) for (sig, bkg, _) in outputs),
                       max(np.max(v) for v in (sig, bkg) for (sig, bkg, _) in outputs),
                       40)

    for sig, bkg, label in outputs:
        if label == 'training':
            sns.distplot(bkg, bins=bins, color='r', kde=False, norm_hist=True, label='background (training)')
            sns.distplot(sig, bins=bins, color='b', kde=False, norm_hist=True, label='signal (training)')
        else:
            centers = (bins[:-1] + bins[1:]) * .5
            bcounts, _ = np.histogram(bkg, bins=bins, density=True)
            plt.plot(centers, bcounts, 'o', color='r', label='background (testing)')
            scounts, _ = np.histogram(sig, bins=bins, density=True)
            plt.plot(centers, scounts, 'o', color='b', label='signal (testing)')
    plt.xlabel('BDT output')

    plt.legend(loc='best')
    plt.savefig(os.path.join(outdir, 'output.png'))
    plt.close()


def plot_roc(outdir, cls, data):
    for x, y, label in data:
        decisions = cls.decision_function(x)
        fpr, tpr, thresholds = roc_curve(y, decisions)
        roc_auc = auc(fpr, tpr)

        plt.plot(fpr, tpr, lw=1, label='ROC for {} (area = {:0.2f})'.format(label, roc_auc))
    plt.xlabel('Background efficiency')
    plt.ylabel('Signal efficiency')

    plt.legend(loc='best')
    plt.savefig(os.path.join(outdir, 'roc.png'))
    plt.close()
