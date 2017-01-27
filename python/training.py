#  vim: set fileencoding=utf-8 :
import codecs
from glob import glob
import logging
import os
import pickle
import yaml

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from sklearn import cross_validation
from sklearn.cross_validation import ShuffleSplit, train_test_split
# from sklearn.tree import DecisionTreeClassifier
# from sklearn.ensemble import AdaBoostClassifier
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.feature_selection import RFECV
from sklearn.learning_curve import learning_curve
from sklearn.metrics import roc_curve, auc, roc_auc_score
from sklearn import grid_search

from root_numpy import array2tree, root2array, rec2array, tree2array

from ttH.TauRoast.external.sklearn_to_tmva import gbr_to_tmva

matplotlib.style.use('ggplot')

CV = 4
NJOBS = 48
COEF = 6


def tmva_like(cls):
    def fun(data):
        ret = 0
        for t in cls.estimators_[:, 0]:
            r = t.tree_.predict(np.array([list(data)], dtype="float32")) / cls.n_estimators * COEF
            ret += r[0, 0]
        return 2.0 / (1.0 + np.exp(-2.0 * ret)) - 1
    return fun


def load(config):
    datadir = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data')
    with open(os.path.join(datadir, 'mva.yaml')) as f:
        setup = yaml.load(f)
    return setup


def read_inputs(config, setup):
    from ttH.TauRoast.processing import Process

    fn = os.path.join(config.get("indir", config["outdir"]), "ntuple.root")

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

    events = min(len(signal), len(background))
    logging.info("using only {} events from signal, background".format(events))

    return signal[:events, :], background[:events, :]


def create_bdts(outdir, setup, x_train, y_train):
    # dt = DecisionTreeClassifier(max_depth=3,
    #                             min_samples_leaf=500)
    # bdt = AdaBoostClassifier(dt,
    #                          algorithm='SAMME',
    #                          n_estimators=800,
    #                          learning_rate=0.5)
    if 'training' in setup.get('features', []):
        logging.info("training bdt(s)")
        for n, cfg in enumerate(setup['trees']):
            dirname = os.path.join(outdir, 'bdt-{}'.format(n))
            if not os.path.exists(dirname):
                os.makedirs(dirname)
            label = cfg.pop('label', 'bdt-{}'.format(n))
            bdt = GradientBoostingClassifier(**cfg)
            bdt.label = label
            bdt.fit(x_train, y_train)
            with open(os.path.join(dirname, "bdt.pkl"), 'wb') as fd:
                pickle.dump((bdt, label), fd)
            with codecs.open(os.path.join(dirname, "configuration.txt"), "w", encoding="utf8") as fd:
                fd.write('{}\n'.format(cfg))
            yield bdt
    else:
        for dirname in glob(os.path.join(outdir, "bdt-*")):
            with open(os.path.join(dirname, "bdt.pkl"), "rb") as fd:
                bdt, label = pickle.load(fd)
                bdt.label = label
                yield bdt


def train(config):
    setup = load(config)
    signal, background = read_inputs(config, setup)

    outdir = os.path.join(config["outdir"], 'sklearn')
    if not os.path.exists(outdir):
        os.makedirs(outdir)

    plot_correlations(outdir, setup["variables"], signal, background)
    plot_inputs(outdir, setup["variables"], signal, background)

    x = np.concatenate((signal, background))
    y = np.concatenate((np.ones(signal.shape[0]),
                        np.zeros(background.shape[0])))

    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=1.0 / CV)
    bdts = list(create_bdts(outdir, setup, x_train, y_train))

    if 'validation' in setup.get('features', []):
        run_cross_validation(outdir, bdts, x, y)
    if 'selection' in setup.get('features', []):
        run_feature_elimination(outdir, bdts, x, y, setup)
    if 'parameters' in setup.get('features', []):
        run_grid_search(outdir, bdts[0], x, y)
    if 'learning' in setup.get('features', []):
        plot_learning_curve(outdir, bdts, x, y)
    if 'validation_curve' in setup.get('features', []):
        run_validation_curve(outdir, bdts, x_train, y_train, x_test, y_test)

    df = pd.DataFrame(x_train, columns=setup["variables"])
    for n, bdt in enumerate(bdts):
        out = u'Feature importance\n===================\n\n'
        for var, score in sorted(zip(setup['variables'], bdt.feature_importances_), key=lambda (x, y): y):
            out += '{:30}: {:>10.4f}\n'.format(var, score)
        with codecs.open(os.path.join(outdir, "bdt-{}".format(n), "log-feature-importance.txt"), "w", encoding="utf8") as fd:
            fd.write(out)

        gbr_to_tmva(bdt, df, os.path.join(outdir, "bdt-{}".format(n), "weights.xml"), coef=COEF)

    logging.info("creating roc, output")
    plot_roc(outdir, bdts, x_train, y_train, x_test, y_test, setup['variables'].index('tt_visiblemass'))
    plot_output(outdir, bdts, [(x_test, y_test, 'testing'), (x_train, y_train, 'training')],
                'decision-function.png', np.linspace(-7, 7, 40), lambda cls, data: cls.decision_function(data))
    plot_output(outdir, bdts, [(x_test, y_test, 'testing'), (x_train, y_train, 'training')],
                'signal-probability.png', np.linspace(0, 1, 40), lambda cls, data: cls.predict_proba(data)[:, 1])
    # plot_output(outdir, bdts, [(x_test, y_test, 'testing'), (x_train, y_train, 'training')],
    #             'tmva-like.png', np.linspace(-1, 1, 40), lambda cls, data: np.apply_along_axis(tmva_like(cls), 1, data))


def evaluate(config, tree):
    setup = load(config)
    data = rec2array(tree2array(tree.raw(), setup["variables"]))

    default = os.path.join(config.get("indir", config["outdir"]), "sklearn", "bdt-0")
    fn = os.path.join(config.get("mvadir", default), "bdt.pkl")
    with open(fn, 'rb') as fd:
        bdt = pickle.load(fd)
    scores = []
    tmva = []
    if len(data) > 0:
        scores = bdt.predict_proba(data)[:, 1]
        tmva = np.apply_along_axis(tmva_like(bdt), 1, data)
    output = np.array(zip(scores, tmva), [('bdt', 'float64'), ('tmva', 'float64')])
    tree.mva(array2tree(output))


def run_cross_validation(outdir, bdts, x, y):
    logging.info("starting cross validation")
    for n, bdt in enumerate(bdts):
        scores = cross_validation.cross_val_score(bdt, x, y, scoring="roc_auc", n_jobs=NJOBS, cv=CV)
        out = u'training accuracy: {} ± {}\n'.format(scores.mean(), scores.std())
        with codecs.open(os.path.join(outdir, "bdt-{}".format(n), "log-accuracy.txt"), "w", encoding="utf8") as fd:
            fd.write(out)


def run_validation_curve(outdir, bdts, x_train, y_train, x_test, y_test):
    logging.info("saving validation curve")
    for bdt in bdts:
        test_score, train_score = np.empty(len(bdt.estimators_)), np.empty(len(bdt.estimators_))

        for i, pred in enumerate(bdt.staged_decision_function(x_test)):
            test_score[i] = 1 - roc_auc_score(y_test, pred)
        for i, pred in enumerate(bdt.staged_decision_function(x_train)):
            train_score[i] = 1 - roc_auc_score(y_train, pred)

        best = np.argmin(test_score)
        line = plt.plot(test_score, label=bdt.label)
        plt.plot(train_score, '--', color=line[-1].get_color())

        plt.xlabel("Number of boosting iterations")
        plt.ylabel("1 - area under ROC")
        plt.axvline(x=best, color=line[-1].get_color())
    plt.legend(loc='best')
    plt.savefig(os.path.join(outdir, 'validation-curve.png'))
    plt.close()


def run_feature_elimination(outdir, bdts, x, y, setup):
    logging.info("starting feature selection")
    for n, bdt in enumerate(bdts):
        rfecv = RFECV(estimator=bdt, step=1, cv=CV, scoring='roc_auc')  # new in 18.1: , n_jobs=NJOBS)
        rfecv.fit(x, y)

        plot_feature_elimination(outdir, rfecv, n)

        out = u'Feature selection\n=================\n\n'
        out += u'optimal feature count: {}\n\nranking\n-------\n'.format(rfecv.n_features_)
        for n, v in enumerate(setup["variables"]):
            out += u'{:30}: {:>5}\n'.format(v, rfecv.ranking_[n])
        with codecs.open(os.path.join(outdir, "bdt-{}".format(n), "log-feature-elimination.txt"), "w", encoding="utf8") as fd:
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


def plot_feature_elimination(outdir, cls, n):
    plt.plot(range(1, len(cls.grid_scores_) + 1), cls.grid_scores_)
    plt.xlabel('# features')
    plt.ylabel('Score (ROC auc)')
    plt.savefig(os.path.join(outdir, 'bdt-{}'.format(n), 'feature-elimination.png'))


def plot_inputs(outdir, vars, sig, bkg):
    for n, var in enumerate(vars):
        _, bins = np.histogram(np.concatenate((sig[:, n], bkg[:, n])), bins=40)
        sns.distplot(bkg[:, n], bins=bins, kde=False, norm_hist=True, label='background')
        sns.distplot(sig[:, n], bins=bins, kde=False, norm_hist=True, label='signal')
        plt.title(var)
        plt.legend()
        plt.savefig(os.path.join(outdir, 'input_{}.png'.format(var)))
        plt.close()


def plot_learning_curve(outdir, bdts, x, y):
    logging.info("creating learning curve")
    for n, cls in enumerate(bdts):
        train_sizes, train_scores, test_scores = learning_curve(cls, x, y,
                                                                cv=ShuffleSplit(len(x),
                                                                                n_iter=100,
                                                                                test_size=1.0 / CV),
                                                                n_jobs=NJOBS,
                                                                train_sizes=np.linspace(.1, 1., 7),
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
        plt.savefig(os.path.join(outdir, 'bdt-{}'.format(n), 'learning-curve.png'.format(n)))
        plt.close()


def plot_output(outdir, bdts, data, filename, bins, fct):
    for n, cls in enumerate(bdts):
        outputs = []
        for x, y, label in data:
            sig = fct(cls, x[y > .5]).ravel()
            bkg = fct(cls, x[y < .5]).ravel()
            outputs.append((sig, bkg, label))

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
        plt.xlabel('BDT score')

        plt.legend(loc='best')
        plt.savefig(os.path.join(outdir, 'bdt-{}'.format(n), filename))
        plt.close()


def plot_roc(outdir, bdts, x_train, y_train, x_test, y_test, vismass):
    for cls in bdts:
        decisions = cls.decision_function(x_test)
        fpr, tpr, thresholds = roc_curve(y_test, decisions)
        roc_auc = auc(fpr, tpr)
        line = plt.plot(fpr, tpr, lw=1, label='ROC for {} (area = {:0.2f})'.format(cls.label, roc_auc))

        decisions = cls.decision_function(x_train)
        fpr, tpr, thresholds = roc_curve(y_train, decisions)
        plt.plot(fpr, tpr, '--', lw=1, color=line[-1].get_color())

    fpr, tpr, thresholds = roc_curve(y_test, x_test[:, vismass])
    roc_auc = auc(fpr, tpr)
    line = plt.plot(fpr, tpr, lw=1, label='ROC for visible mass (area = {:0.2f})'.format(roc_auc))

    fpr, tpr, thresholds = roc_curve(y_train, x_train[:, vismass])
    plt.plot(fpr, tpr, '--', lw=1, color=line[-1].get_color())

    plt.xlabel('Background efficiency')
    plt.ylabel('Signal efficiency')

    plt.legend(loc='best')
    plt.savefig(os.path.join(outdir, 'roc.png'))
    plt.close()
