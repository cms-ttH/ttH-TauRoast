#  vim: set fileencoding=utf-8 :
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
from sklearn.cross_validation import train_test_split
from sklearn.externals import joblib
# from sklearn.tree import DecisionTreeClassifier
# from sklearn.ensemble import AdaBoostClassifier
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.metrics import roc_curve, auc

from root_numpy import array2tree, root2array, rec2array, tree2array

from ttH.TauRoast.external.sklearn_to_tmva import gbr_to_tmva

matplotlib.style.use('ggplot')


def load(config):
    datadir = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data')
    with open(os.path.join(datadir, 'mva.yaml')) as f:
        setup = yaml.load(f)
    return setup


def train(config):
    from ttH.TauRoast.processing import Process

    setup = load(config)
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

    plot_inputs(config["outdir"], setup["variables"], signal, background)

    x = np.concatenate((signal, background))
    y = np.concatenate((np.ones(signal.shape[0]),
                        np.zeros(background.shape[0])))

    # dt = DecisionTreeClassifier(max_depth=3,
    #                             min_samples_leaf=500)
    # bdt = AdaBoostClassifier(dt,
    #                          algorithm='SAMME',
    #                          n_estimators=800,
    #                          learning_rate=0.5)
    bdt = GradientBoostingClassifier(n_estimators=200,
                                     max_depth=3,
                                     subsample=0.5,
                                     max_features=0.5,
                                     learning_rate=0.02)

    scores = cross_validation.cross_val_score(bdt, x, y, scoring="roc_auc", n_jobs=6, cv=5)
    logging.info(u'training accuracy: {} ± {}'.format(scores.mean(), scores.std()))

    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=.2)
    bdt.fit(x_train, y_train)

    fn = os.path.join(config["outdir"], "sklearn", "bdt", "bdt.pkl")
    if not os.path.exists(os.path.dirname(fn)):
        os.makedirs(os.path.dirname(fn))
    joblib.dump(bdt, fn)

    df = pd.DataFrame(x_train, columns=setup["variables"])
    gbr_to_tmva(bdt, df, os.path.join(config["outdir"], "weights.xml"))

    plot_roc(config["outdir"], bdt, [(x_test, y_test, 'testing'), (x_train, y_train, 'training')])
    plot_output(config["outdir"], bdt, [(x_test, y_test, 'testing'), (x_train, y_train, 'training')])


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


def plot_inputs(outdir, vars, sig, bkg):
    outdir = os.path.join(outdir, 'sklearn')
    if not os.path.exists(outdir):
        os.makedirs(outdir)
    for n, var in enumerate(vars):
        _, bins = np.histogram(np.concatenate((sig[:, n], bkg[:, n])), bins=40)
        sns.distplot(bkg[:, n], bins=bins, kde=False, norm_hist=True, label='background')
        sns.distplot(sig[:, n], bins=bins, kde=False, norm_hist=True, label='signal')
        plt.title(var)
        plt.legend()
        plt.savefig(os.path.join(outdir, 'input_{}.png'.format(var)))
        plt.close()


def plot_output(outdir, cls, data):
    outdir = os.path.join(outdir, 'sklearn')
    if not os.path.exists(outdir):
        os.makedirs(outdir)
    for x, y, label in data:
        sig = cls.decision_function(x[y > .5]).ravel()
        bkg = cls.decision_function(x[y < .5]).ravel()

        bins = np.linspace(min(np.min(v) for v in (sig, bkg)),
                           max(np.max(v) for v in (sig, bkg)),
                           40)
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
    outdir = os.path.join(outdir, 'sklearn')
    if not os.path.exists(outdir):
        os.makedirs(outdir)

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
