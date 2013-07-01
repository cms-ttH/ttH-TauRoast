import ROOT as r

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

def vectorize(items, cls):
    """Create a vector of class `cls` and fill it with the object in the
    list `items`."""
    res = r.std.vector(cls)()
    for i in items:
        res.push_back(i)
    return res

