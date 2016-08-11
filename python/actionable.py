from contextlib import contextmanager
from itertools import groupby

import codecs
import logging
import os
import pickle
import yaml

import ROOT as r

from ttH.TauRoast import useful
from ttH.TauRoast.botany import Forest
from ttH.TauRoast.cutting import StaticCut, Cut, cutflow, normalize
from ttH.TauRoast.plotting import Plot
from ttH.TauRoast.processing import Process


def setup_cuts(config):
    cutflows = {}
    for name, cut_cfgs in config["cuts"].items():
        counts = []
        cuts = [Cut("Ntuple analyzed", "true")]
        weights = []

        for cfg in cut_cfgs:
            cuts.append(Cut(*cfg.items()[0]))

        for weight in config["weights"]:
            weights.append(StaticCut(weight))

        cutflows[name] = (counts, cuts, weights)
    return cutflows


def load_cutflows(config):
    cutflows = {}
    fn = os.path.join(config.get("indir", config["outdir"]), "cutflow.pkl")
    with open(fn, 'rb') as f:
        for name, cuts in pickle.load(f).items():
            cutflows[name] = cuts[:-2]
    return cutflows


def split_cuts(concatenated_cutflows):
    cutflows = {}
    for name, all_cuts in concatenated_cutflows.items():
        cutflows[name] = tuple(list(g) for k, g in groupby(all_cuts, key=type))


def dump_categories(args, config):
    categories, definitions = get_categories(config)
    counts = []

    fn = os.path.join(config.get("indir", config["outdir"]), "plots.root")
    f = r.TFile(fn, "READ")

    if not f.IsOpen():
        raise IOError("Can't read file '{0}'".format(fn))

    for proc in config['plot']:
        count = StaticCut(proc)
        for cat in categories:
            count[cat] = Plot.get_event_count(f, proc, cat, config['histformat'])
        counts.append(count)

    try:
        useful.print_cuts(counts, categories)
    except UnicodeEncodeError:
        pass

    with codecs.open(os.path.join(config["outdir"], "categories.txt"), "w", encoding="utf8") as fd:
        useful.print_cuts(counts, categories, f=fd)


def dump_cuts(args, config):
    cutflows = load_cutflows(config)
    for name, cuts in cutflows.items():
        normalize(cuts, config["lumi"], config.get("event limit"))
        try:
            cutflow(cuts, config["plot"])
        except UnicodeEncodeError:
            pass

        with codecs.open(os.path.join(config["outdir"], "cuts_{}.txt").format(name), "w", encoding="utf8") as fd:
            cutflow(cuts, config["plot"], f=fd)
        with codecs.open(os.path.join(config["outdir"], "cuts_{}_relative.txt").format(name), "w", encoding="utf8") as fd:
            cutflow(cuts, config["plot"], f=fd, relative=True)
        with codecs.open(os.path.join(config["outdir"], "cuts_{}_weighed.txt".format(name)), "w", encoding="utf8") as fd:
            cutflow(cuts, config["plot"], f=fd, weighed=True)


def analyze(args, config):
    fn = os.path.join(config["outdir"], "ntuple.root")

    if args.reuse:
        cutflows = split_cuts(load_cutflows(config))
    else:
        if os.path.exists(fn):
            os.unlink(fn)
        cutflows = setup_cuts(config)

    for proc in sum(map(Process.expand, config['plot']), []):
        counts, cuts, weights = cutflows[proc.cutflow]

        if len(counts) > 0 and str(proc) in counts[0].processes():
            continue
        local_cuts = list(cuts)
        for cfg in proc.additional_cuts:
            local_cuts.insert(0, Cut(*cfg))

        proc.analyze(fn, counts, local_cuts, weights, config["systematics"], config[
                     'ntupledir'], config.get('event limit', -1), args.debug_cuts)

    concatenated_cutflows = {}
    for name, (counts, cuts, weights) in cutflows.items():
        cuts = counts + cuts + weights
        normalize(cuts, config["lumi"], config.get("event limit"))
        concatenated_cutflows[name] = cuts

    fn = os.path.join(config["outdir"], "cutflow.pkl")
    with open(fn, 'wb') as f:
        pickle.dump(concatenated_cutflows, f)


def get_categories(config):
    categories = []
    definitions = []
    for cfg in config.get("categories", [{'inclusive': None}]):
        category, definition = cfg.items()[0]
        categories.append(category)
        definitions.append(definition)
    return categories, definitions


@contextmanager
def open_rootfile(fn, mode="UPDATE"):
    f = r.TFile(fn, mode)

    if not f.IsOpen():
        raise IOError("Can't read file '{0}'".format(fn))

    yield f

    f.Write()
    f.Close()


def fill(args, config):
    cutflows = load_cutflows(config)
    for name, cuts in cutflows.items():
        normalize(cuts, config["lumi"], config.get("event limit"))

    categories, definitions = get_categories(config)

    atomic_processes = set(sum(map(Process.expand, config['plot']), []))
    limit_processes = set(config["limits"]) & set(config["plot"])
    all_processes = set(Process.get(n) for n in limit_processes) | atomic_processes
    not_processed = set(config["limits"]) - set(config["plot"])

    if len(not_processed) > 0:
        logging.error(
            "the limit plots will not be saved for: {}".format(", ".join(not_processed)))

    if len(all_processes) != len(set([p.limitname for p in all_processes])):
        logging.error(
            "the limit names of the processes are not unique and will lead to collisions!")

    fn = os.path.join(config.get("indir", config["outdir"]), "ntuple.root")
    forest = Forest(fn)

    for category, definition in zip(categories, definitions):
        Plot.reset()

        for proc in atomic_processes:
            for p in Plot.plots():
                p.fill(proc, config["weights"], definition)

        fn = os.path.join(config["outdir"], "plots.root")
        with open_rootfile(fn) as f:
            for p in Plot.plots():
                p.write(f, cutflows, category, fmt=config["histformat"])

        fn = os.path.join(config["outdir"], "limits.root")
        with open_rootfile(fn) as f:
            for p in Plot.plots():
                p.write(f, cutflows, category, procs=limit_processes, fmt=config["histformat"])

        timing = sorted(Plot.plots(), key=lambda p: p._time)
        for p in timing[:10] + timing[-10:]:
            logging.debug("plot filling time for {0}: {1}".format(p, p._time))
    del forest


def plot(args, config):
    datadir = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data')
    with open(os.path.join(datadir, 'plot.yaml')) as f:
        plotconfig = yaml.load(f)

    categories, _ = get_categories(config)
    for category in categories:
        Plot.reset()

        processes = sum(map(Process.expand, config['plot']), [])
        fn = os.path.join(config.get("indir", config["outdir"]), "plots.root")
        f = r.TFile(fn, "READ")

        if not f.IsOpen():
            raise IOError("Can't read file '{0}'".format(fn))

        for p in Plot.plots():
            p.read(f, category, processes, fmt=config["histformat"])
            p.save(plotconfig, os.path.join(config["outdir"], category))

        f.Close()
