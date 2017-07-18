from contextlib import contextmanager
from itertools import groupby

import codecs
import logging
import os
import shutil
import yaml

import ROOT as r

from ttH.TauRoast import useful
from ttH.TauRoast.botany import Forest
from ttH.TauRoast.cutting import StaticCut, Cut, Cutflows, cutflow, normalize
from ttH.TauRoast.plotting import Plot
from ttH.TauRoast.processing import Process


def expand_systematics(systematics, weights):
    all_systematics = [('NA', weights)]
    for unc in set(systematics) - set(['NA']):
        if unc not in ('CMS_ttHl_JES', 'CMS_ttHl_tauES'):
            new_weights = weights[:]
            if 'btag' in unc:
                new_weights = [w for w in new_weights if w != 'CSVWeight']
            if 'FRjt' in unc:
                new_weights = [w for w in new_weights if w != 'Fake']
            if unc in new_weights:
                new_weights.remove(unc)
            all_systematics.append(('NA', new_weights + [unc + 'Up']))
            all_systematics.append(('NA', new_weights + [unc + 'Down']))
        else:
            all_systematics.append((unc + 'Up', weights))
            all_systematics.append((unc + 'Down', weights))
    return all_systematics


def setup_cuts(config):
    cutflows = Cutflows()
    cutflownames = [k.replace(' cuts', '') for k in config if k.endswith(' cuts') and not k.startswith('baseline')]
    for name in cutflownames:
        weights = config.get(name + ' weights')
        systematics = config.get(name + ' systematics', [])
        for unc in [s for s, w in expand_systematics(systematics, weights)]:
            counts = []
            cuts = [Cut("Ntuple analyzed", "true")]
            weights = []

            for cfg in config.get('baseline cuts', []) + config[name + ' cuts']:
                cuts.append(Cut(*cfg.items()[0]))

            for weight in config[name + ' weights']:
                weights.append(StaticCut(weight))

            suffix = '' if unc == 'NA' else '_' + unc
            cutflows[name + suffix] = (counts, cuts, weights)
    return cutflows


def load_cutflows(config):
    fn = os.path.join(config.get("indir", config["outdir"]), "cutflow.pkl")

    return Cutflows(fn)


def split_cuts(concatenated_cutflows):
    cutflows = Cutflows()
    for name, all_cuts in concatenated_cutflows.items():
        cuts = tuple(list(g) for k, g in groupby(all_cuts, key=type))
        if len(cuts) == 1:
            cuts = ([], cuts[0], [])
        elif len(cuts) == 2:
            cuts = ([], cuts[0], cuts[1])
        cutflows[name] = cuts
    return cutflows


def dump_categories(args, config):
    unweighed = config.get("mode") == "sync"
    categories, definitions = get_categories(config)
    counts = []

    fn = os.path.join(config.get("indir", config["outdir"]), "plots.root")
    f = r.TFile(fn, "READ")

    if not f.IsOpen():
        raise IOError("Can't read file '{0}'".format(fn))

    for proc in config['plot']:
        count = StaticCut(proc)
        for cat in categories:
            count[cat] = Plot.get_event_count(f, proc, cat, config['histformat'], unweighed)
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
        if name.endswith('Up') or name.endswith('Down'):
            continue

        normalize(cuts, config["lumi"], config.get("event limit"))
        try:
            cutflow(cuts, config["plot"])
        except UnicodeEncodeError:
            pass

        name = name.replace(' ', '_')
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

    for proc in set(sum((Process.expand(p) for p in config['plot'] + config['limits']), [])):
        uncertainties = ['NA']
        if args.systematics:
            weights = config.get(proc.cutflow + ' weights')
            systematics = config.get(proc.cutflow + ' systematics', [])
            uncertainties = [s for s, w in expand_systematics(systematics, weights)]
        for unc in uncertainties:
            suffix = '' if unc == 'NA' else '_' + unc
            counts, cuts, weights = cutflows[proc.cutflow + suffix]

            if len(counts) > 0 and str(proc) in counts[0].processes():
                continue

            logging.info("using systematics: " + unc)

            local_cuts = list(cuts)
            for cfg in proc.additional_cuts:
                local_cuts.insert(0, Cut(*cfg))

            proc.analyze(config, fn, counts, local_cuts, weights, unc, args.debug_cuts)

    concatenated_cutflows = Cutflows()
    for name, (counts, cuts, weights) in cutflows.items():
        cuts = counts + cuts + weights
        normalize(cuts, config["lumi"], config.get("event limit"))
        concatenated_cutflows[name] = cuts

    concatenated_cutflows.save(config)


def add_mva(args, config):
    fn = os.path.join(config["outdir"], "ntuple.root")
    for proc in set(sum((Process.expand(p) for p in config['plot'] + config['limits']), [])):
        systematics = ['NA']
        if args.systematics:
            weights = config.get(proc.cutflow + ' weights')
            systematics = config.get(proc.cutflow + ' systematics', [])
            systematics = set([s for s, w in expand_systematics(systematics, weights)])
        for unc in systematics:
            logging.info("using systematics: " + unc)
            proc.add_mva(config, fn, unc)


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

    atomic_processes = set(sum(map(Process.expand, config['plot'] + config['limits']), []))
    limit_processes = config["limits"]
    all_processes = set(Process.get(n) for n in limit_processes + config['plot']) | atomic_processes

    if len(all_processes) != len(set([p.limitname for p in all_processes])):
        logging.error("the limit names of the processes are not unique and will lead to collisions!")

    if 'indir' in config and config['indir'] != config['outdir']:
        shutil.copy(
            os.path.join(config['indir'], 'cutflow.pkl'),
            os.path.join(config['outdir'], 'cutflow.pkl')
        )

    fn = os.path.join(config.get("indir", config["outdir"]), "ntuple.root")
    forest = Forest(fn)

    for category, definition in zip(categories, definitions):
        logging.info("filling category: " + category)
        Plot.reset()

        for proc in atomic_processes:
            logging.info("filling process: " + str(proc))

            weights = config.get(proc.cutflow + ' weights')
            systematics = config.get(proc.cutflow + ' systematics', [])
            uncertainties = [('NA', weights)]
            if args.systematics:
                uncertainties = expand_systematics(systematics, weights)
            for n, (systematic, weights) in enumerate(uncertainties):
                logging.info("using systematics: " + systematic)
                logging.info("using weights: " + ", ".join(weights))
                for p in Plot.plots():
                    if (not args.essential and n == 0) or p.essential():
                        p.fill(proc, systematic, weights, definition)

        uncertainties = None
        if args.systematics:
            uncertainties = list(set(sum((config.get(p.cutflow + ' systematics', []) for p in atomic_processes), [])))

        logging.info("writing out plots for category: " + category)
        fn = os.path.join(config["outdir"], "plots.root")
        with open_rootfile(fn) as f:
            for p in Plot.plots():
                p.write(f, cutflows, category, uncertainties,
                        procs=all_processes, fmt=config["histformat"])

        discriminants = config.get("discriminants", [])
        fn = os.path.join(config["outdir"], "limits.root")
        with open_rootfile(fn) as f:
            for p in Plot.plots():
                if p.limitname in discriminants:
                    p.write(f, cutflows, category, uncertainties,
                            procs=limit_processes, fmt=config["histformat"])

        timing = sorted(Plot.plots(), key=lambda p: p._time)
        for p in timing[:10] + timing[-10:]:
            logging.debug("plot filling time for {0}: {1}".format(p, p._time))
        logging.info("done filling category: " + category)
    del forest


def plot(args, config):
    datadir = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data')
    with open(os.path.join(datadir, 'plot.yaml')) as f:
        plotconfig = yaml.load(f)
    plotconfig.update(config.get('plot override', {}))

    if args.adjust:
        fn = os.path.join(config.get("indir", config["outdir"]), "impacts.json")
        Plot.read_impacts(fn)

    categories, _ = get_categories(config)
    for category in categories:
        logging.info("saving plots for category: " + category)
        Plot.reset()

        processes = sum(map(Process.expand, config['plot']), [])
        fn = os.path.join(config.get("indir", config["outdir"]), "plots.root")
        f = r.TFile(fn, "READ")

        systematics = []
        if args.systematics:
            systematics = list(set(sum((config.get(p.cutflow + ' systematics', []) for p in processes), [])))

        if not f.IsOpen():
            raise IOError("Can't read file '{0}'".format(fn))

        for p in Plot.plots():
            if args.essential and not p.essential():
                continue
            p.read(f, category, processes, systematics=systematics,
                   fmt=config["histformat"])
            p.save(plotconfig, os.path.join(config["outdir"], category),
                   systematics=systematics)
            p.clear()

        f.Close()


def datacard(args, config):
    import CombineHarvester.CombineTools.ch as ch

    fn = os.path.join(config["outdir"], "limits.root")

    cats = list(enumerate([d.keys()[0] for d in config['categories']], 1))
    sigs = ['ttH']
    bkgs = [Process.get(p).limitname
            for p in config['limits'] if (not p.startswith('ttH')) and
                                         (not p.startswith('collisions'))]

    cb = ch.CombineHarvester()
    # cb.SetVerbosity(2)

    masses = ch.ValsFromRange('120:130|5')
    masses = ['125']
    cb.AddObservations(['*'], ['ttH'], ['13TeV'], ['ttl'], cats)
    cb.AddProcesses(['*'], ['ttH'], ['13TeV'], ['ttl'], bkgs, cats, False)
    cb.AddProcesses(masses, ['ttH'], ['13TeV'], ['ttl'], sigs, cats, True)

    cb.cp().AddSyst(cb, 'lumi_13TeV', 'lnN', ch.SystMap()(1.025))
    if args.systematics:
        for unc in config['systematics']:
            cb.cp().AddSyst(cb, unc, 'shape', ch.SystMap()(1.0))

    cb.cp().backgrounds().ExtractShapes(
        fn,
        config['histformat'].format(p='$PROCESS', c='$BIN', v=args.discriminant),
        config['histformat'].format(p='$PROCESS', c='$BIN', v=args.discriminant) + '_$SYSTEMATIC'
    )
    cb.cp().signals().ExtractShapes(
        fn,
        config['histformat'].format(p='$PROCESS$MASS', c='$BIN', v=args.discriminant),
        config['histformat'].format(p='$PROCESS$MASS', c='$BIN', v=args.discriminant) + '_$SYSTEMATIC'
    )

    cb.PrintObs().PrintProcs().PrintSysts()

    # cb.WriteDatacard("card.txt", "file.root")
    writer = ch.CardWriter('$TAG/$MASS/$ANALYSIS_$BIN.txt',
                           '$TAG/common/$ANALYSIS_$BIN.root')
    writer.SetVerbosity(1)
    writer.WriteCards('limits', cb)
