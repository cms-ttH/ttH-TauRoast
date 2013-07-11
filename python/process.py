import math
import os
import ROOT as r

try:
    from ROOT import roast
except:
    sys.stderr.write("Failed to import 'roast'!\n")
    sys.exit(1)

from TTHTauTau.Roast.helper import *

histo_default = {
        'numBinsX': 0,
        'xMin': 0,
        'xMax': 0,
        'showOF': False,
        'showUF': False,
        'numBinsY': 0,
        'yMin': 0,
        'yMax': 0,
        'logx': False,
        'logy': False,
        'logz': False,
        'xTitle': '',
        'yTitle': '',
        'zTitle': '',
        'xMinVis': 0,
        'xMaxVis': 0,
        'yMinVis': 0,
        'yMaxVis': 0,
        'showText': True,
        'centerLabels': False
        }

def analyze(config):
    """Create a list of processes, as defined in `process_vXX.yaml`, and
    specified in the process/analysis section of the configuration.  Return
    analyzed processes."""

    processes = r.std.vector('roast::Process*')()
    for proc in config['analysis']['process']:
        cfg = config['processes'][proc]

        vpaths = r.std.vector('string')()
        for p in cfg['ntuplePath']:
            vpaths.push_back(os.path.join(config['paths']['ntuples'], p))

        p = roast.Process(
            proc,
            cfg['niceName'],
            cfg['labelForLegend'],
            cfg['type'],
            config['paths']['tree'],
            vpaths,
            cfg['color'],
            cfg['NOEinDS'],
            cfg['NoEreadByNUTter'],
            cfg['crossSection'],
            cfg['branchingRatio'],
            cfg['checkReality'] if 'checkReality' in cfg else False)

        for name, histcfg in config['histograms'].items():
            clone = histo_default.copy()
            clone.update(histcfg)
            try:
                if clone['type'] != "th1f":
                    continue
                w = roast.HWrapper(name, clone['dir'], clone['type'],
                        clone['xTitle'], clone['yTitle'], clone['zTitle'],
                        clone['logx'], clone['logy'], clone['logz'],
                        clone['numBinsX'], clone['xMin'], clone['xMax'], clone['xMinVis'], clone['xMaxVis'],
                        clone['numBinsY'], clone['yMin'], clone['yMax'], clone['yMinVis'], clone['yMaxVis'],
                        clone['showOF'], clone['showUF'], clone['centerLabels'], clone['showText'])
                p.AddHistogram(name, w)
            except:
                print "Can't create", name, ", please check config"

        processes.push_back(p)

    processed = []
    for p in processes:
        roast.ttl.analyze(p, vectorize(config['physics']['cuts'], 'string'), config['analysis']['max events'])
        processed.append(p)
    return processed

def fill_histos(config, processes):
    flags = config['physics']['flags']

    processed = []
    for p in processes:
        if p.GetShortName() not in config['analysis']['process']:
            continue

        p.ResetHistograms()

        branches = roast.ttl.Branches(p.GetTreeName(), p.GetNtuplePaths())
        cutflow = p.GetCutFlow()

        if config['physics']['pair selection'] == 'iso':
            select = lambda xs: sorted(
                    xs,
                    key=lambda x: branches.TTL_Tau1HPSbyIsolationMVA2raw[x]**2 + branches.TTL_Tau2HPSbyIsolationMVA2raw[x]**2,
                    reverse=True)[0]
        elif config['physics']['pair selection'] == 'pt':
            select = lambda xs: x[0]
        else:
            # FIXME handle properly
            raise

        weights = []
        for (flag, val) in flags.items():
            try:
                strength = -1.
                if flag in config['physics']['systematics']:
                    if val == "nominal":
                        # FIXME do this properly
                        raise
                    elif val == "up":
                        strength = config['physics']['systematics'][flag]
                    elif val == "down":
                        strength = 2 - config['physics']['systematics'][flag]

                if flag == 'brSF':
                    if not p.GetShortName().startswith("TTH_"):
                        strength = 1.

                if val == "nominal":
                    weights.append(roast.Weight.Create(flag, roast.Weight.kNominal, strength))
                elif val == "up":
                    weights.append(roast.Weight.Create(flag, roast.Weight.kUp, strength))
                elif val == "down":
                    weights.append(roast.Weight.Create(flag, roast.Weight.kDown, strength))
            except Exception, e:
                # FIXME log this properly
                print flag, val
                print e
                raise

        for e in p.GetGoodEvents():
            branches.GetEntry(e.entry)

            idx = select(e.combos)

            w_tot = 1.
            if p.IsMC():
                for weight in weights:
                    w_tot *= weight(branches, idx)

            branches.FillHistograms(p.GetHContainer(), idx, w_tot)

        for weight in weights:
            weight.RegisterCut(cutflow)
