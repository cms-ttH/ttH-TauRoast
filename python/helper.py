import os
import ROOT as r
import sys

r.gROOT.SetBatch()
r.gSystem.Load("libTTHTauTauRoast")

try:
    from ROOT import roast
except:
    sys.stderr.write("Failed to import 'roast'!\n")
    sys.exit(1)

def train_mva(config, processes):
    cfg = config['analysis']['final mva']
    if not os.path.exists(cfg['dir']):
        os.makedirs(cfg['dir'])

    mva = roast.ttl.MVABase(cfg['dir'], vectorize(cfg['variables'], 'string'), 1)

    sig = get_process(cfg['signal'], processes)
    bkg = vectorize(
            map(lambda p: get_process(p, processes), cfg['background']),
            'roast::Process*')
    mva.CreateTrainingSample(sig, bkg)

    m = r.std.map('string,string')()
    for k, v in cfg['methods'].items():
        m[k] = v

    mva.TrainMVA(m)

def book_mva(config, processes):
    cfg = config['analysis']['final mva']
    if not os.path.exists(cfg['dir']):
        os.makedirs(cfg['dir'])

    for method, opts in cfg['methods'].items():
        mva = roast.ttl.MVABase(cfg['dir'], vectorize(cfg['variables']), 1)
        if mva.BookMVA(method):
            roast.register_mva(method, mva);

def combine_processes(config, ps):
    for (alias, cfg) in config['analysis']['combine'].items():
        to_combine = cfg['processes']
        res = roast.Process(get_process(to_combine[0], ps))

        for other in to_combine[1:]:
            res.Add(get_process(other, ps))

        res.SetShortName(alias)
        res.SetNiceName(cfg['name'])
        res.SetLabelForLegend(cfg['legend'])
        res.SetColor(cfg['color'])

        ps.append(res)

get_signals = lambda ps: filter(lambda p: p.IsSignal(), ps)
get_backgrounds = lambda ps: filter(lambda p: p.IsBackground(), ps)

def get_collisions(ps):
    res = filter(lambda p: p.IsCollisions(), ps)
    if len(res) != 1:
        # FIXME turn into real exception
        raise "Not one collision process found!"
    return res[0]

def get_process(name, processes):
    res = filter(lambda p: p.GetShortName() == name, processes)
    if len(res) != 1:
        # FIXME turn into real exception
        raise "More than one process with name " + name
    return res[0]

def normalize_processes(config, processes):
    lumi = config['physics']['lumi']

    try:
        coll = get_collisions(processes)
        if coll.GetNOEanalyzed() != coll.GetNOEinNtuple():
            lumi *= coll.GetNOEanalyzed() / float(coll.GetNOEinNtuple())
        coll.GetCutFlow().RegisterCutFromLast("Lumi norm", 2, 1)
        coll.BuildNormalizedCutFlow()
    except:
        pass

    for p in processes:
        if p.IsCollisions():
            continue

        est_lumi = lumi * p.GetCrossSection() * p.GetBranchingRatio()
        raw_events = p.GetNOEinDS() * p.GetNOEanalyzed() / float(p.GetNOEinNtuple())

        p.ScaleHistograms(est_lumi / raw_events)
        p.GetCutFlow().RegisterCutFromLast("Lumi norm", 2, est_lumi / raw_events)
        p.BuildNormalizedCutFlow()

def print_cutflow(config, processes):
    procs = map(lambda n: get_process(n, processes), config['analysis']['plot'])
    cuts = map(lambda p: p.GetCutFlow().GetCuts(), procs)

    out = sys.stdout

    out.write(" " * 20)
    for p in procs:
        out.write("{0:^15}".format(p.GetShortName()))
    out.write("\n")

    for i in range(len(cuts[0])):
        out.write("{0:20}".format(cuts[0][i].name))
        for c in cuts:
            out.write("{0:15}".format(c[i].passedSignalEvents))
        out.write("\n")

def vectorize(items, cls=None):
    """Create a vector of class `cls` and fill it with the object in the
    list `items`."""
    if cls is None:
        cls = type(items[0])
    res = r.std.vector(cls)()
    for i in items:
        res.push_back(i)
    return res

