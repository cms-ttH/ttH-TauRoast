# vim: ts=4:sw=4:et:sta
import logging
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

def analyze(config, module):
    """Create a list of processes, as defined in `process_vXX.yaml`, and
    specified in the process/analysis section of the configuration.  Return
    analyzed processes."""

    r.TH1.SetDefaultSumw2()

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
                logging.error("unable to create histogram %s", name)

        processes.push_back(p)

    cuts = []
    for cutcfg in config['physics']['cuts']:
        min = cutcfg['min'] if 'min' in cutcfg else -float('inf')
        max = cutcfg['max'] if 'max' in cutcfg else float('inf')
        cuts.append(roast.CutFlow.Cut(cutcfg['name'], min, max))

    processed = []
    for p in processes:
        n = module.analyze(p, vectorize(cuts), config['analysis']['max events'],
                lambda i: logging.info("analyzing %s, event %i", p.GetShortName(), i) if i % 1000 == 0 else None)
        logging.info("analyzed %i events of %s", n, p.GetShortName())
        processed.append(p)
    return processed

def fill_histos(config, processes, module):
    flags = config['physics']['flags']
    split = config['analysis']['split']

    procs = filter(lambda p: p.GetShortName() in config['analysis']['process'], processes)

    for p in procs:
        p.ResetHistograms()

        branches = module.Branches(p.GetTreeName(), p.GetNtuplePaths())
        cutflow = p.GetCutFlow()

        if config['physics']['pair selection'] == 'iso':
            select = module.IsoPicker()
        elif config['physics']['pair selection'] == 'pt':
            select = roast.PtPicker()
        else:
            logging.critical("invalid selection mechanism: %s", config['physics']['pair selection'])
            sys.exit(1)

        weights = r.std.vector('roast::Weight')()
        for (flag, val) in flags.items():
            try:
                strength = -1.
                if flag in config['physics']['systematics']:
                    if val == "up":
                        strength = config['physics']['systematics'][flag]
                    elif val == "down":
                        strength = 2 - config['physics']['systematics'][flag]
                    else:
                        logging.critcal("invalid direction for flag %s: %s", flag, val)
                        sys.exit(1)

                if flag == 'brSF':
                    if not p.GetShortName().startswith("TTH_"):
                        strength = 1.

                if val == "nominal":
                    weights.push_back(roast.Weight.Create(flag, roast.Weight.kNominal, strength))
                elif val == "up":
                    weights.push_back(roast.Weight.Create(flag, roast.Weight.kUp, strength))
                elif val == "down":
                    weights.push_back(roast.Weight.Create(flag, roast.Weight.kDown, strength))
            except Exception, e:
                logging.critical("could not create flag %s with value %s", flag, val)
                sys.exit(1)

        name = p.GetShortName()

        log = lambda i: logging.info("filling %s, event %i", name, i) if i % 25 == 0 else None
        splitter = split[name] if name in split else 0

        logging.debug("selecting best particle combination with %s", repr(select))
        logging.debug("splitting sample with %s", repr(splitter))

        split_count = module.fill(p, weights, log, splitter, select)
        total_count = p.GetGoodEvents().size()

        for weight in weights:
            weight.RegisterCut(cutflow)

        if total_count > 0:
            ratio = split_count / float(total_count)
        else:
            ratio = 0

        cutflow.RegisterCutFromLast("Sample splitting", 2, ratio)
        logging.info("filled %i events of %s", split_count, name)
