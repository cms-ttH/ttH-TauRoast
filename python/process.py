# vim: ts=4:sw=4:et:sta
import logging
import math
import os
import random
import ROOT as r

try:
    from ROOT import roast
except:
    sys.stderr.write("Failed to import 'roast'!\n")
    sys.exit(1)

from ttH.TauRoast.helper import *

def analyze(config, module):
    """Create a list of processes, as defined in `process_vXX.yaml`, and
    specified in the process/analysis section of the configuration.  Return
    analyzed processes."""

    processes = r.std.vector('roast::Process*')()
    for proc in config['analysis']['process']:
        cfg = config['processes'][proc]

        # deal with non-physical processes later (combine them)
        if not 'ntuplePath' in cfg:
            continue

        vpaths = r.std.vector('string')()
        for p in cfg['ntuplePath']:
            vpaths.push_back(config['paths']['ntuples'].format(p=p))

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

        processes.push_back(p)

    err = False
    cuts = []
    for cutcfg in config['physics']['cuts']:
        min = cutcfg['min'] if 'min' in cutcfg else -float('inf')
        max = cutcfg['max'] if 'max' in cutcfg else float('inf')
        try:
            cuts.append(roast.CutFlow.ValueCut(cutcfg['name'], min, max))
        except:
            logging.error("undefined variable %s", cutcfg['name'])
            err = True
    if err:
        sys.exit(1)

    processed = []
    for p in processes:
        n = module.analyze(p, vectorize(cuts, 'roast::CutFlow::Cut*'), config['analysis']['max events'],
                lambda i: logging.info("analyzing %s, event %i", p.GetShortName(), i) if i % 1000 == 0 else None)
        logging.info("analyzed %i events of %s", n, p.GetShortName())
        processed.append(p)
    return processed

def fill_histos(config, processes, module):
    flags = config['physics']['flags']
    split = config['analysis']['split']

    procs = filter(lambda p: p.GetShortName() in config['analysis']['process'], processes)

    r.TH1.SetDefaultSumw2()

    for p in procs:
        p.ResetHistograms()

        logging.info("registering histograms for %s", p.GetShortName())
        for name, histcfg in config['histograms'].items():
            try:
                if 'values' in histcfg:
                    values = histcfg['values']
                else:
                    values = [name]

                if len(values) == 2:
                    (xval, yval) = values
                    axes = [r.TH1.GetXaxis, r.TH1.GetYaxis]

                    h = r.TH2F(
                            name + str(random.randint(0, 1000000000)),
                            ';'.join([name] + histcfg['axis labels']),
                            *histcfg['binning'])

                    if 'bin labels' in histcfg:
                        for (axis, labels) in zip(axes, histcfg['bin labels']):
                            for (n, l) in enumerate(labels):
                                axis(h).SetBinLabel(n + 1, l)
                    if 'visible' in histcfg:
                        for (axis, range) in zip(axes, histcfg['visible']):
                            axis(h).SetRangeUser(*map(float, range))

                    if 'max' in histcfg:
                        w = roast.HWrapper.Create2D(histcfg['dir'], h, xval, yval, histcfg['max'])
                    else:
                        w = roast.HWrapper.Create2D(histcfg['dir'], h, xval, yval)
                elif len(values) == 1:
                    (xval,) = values
                    h = r.TH1F(
                            name + str(random.randint(0, 1000000000)),
                            ';'.join([name] + histcfg['axis labels']),
                            *histcfg['binning'])

                    if 'bin labels' in histcfg:
                        for (n, l) in enumerate(histcfg['bin labels']):
                            h.GetXaxis().SetBinLabel(n + 1, l)
                    if 'visible' in histcfg:
                        h.GetXaxis().SetRangeUser(*map(float, histcfg['visible']))

                    if 'max' in histcfg:
                        w = roast.HWrapper.Create1D(histcfg['dir'], h, xval, histcfg['max'])
                    else:
                        w = roast.HWrapper.Create1D(histcfg['dir'], h, xval)
                else:
                    raise TypeError("Need either one or two values for histograms")

                if 'additional info' in histcfg:
                    vals = histcfg['additional info']
                    if isinstance(vals, list):
                        if len(vals) > 0:
                            w.SetAdditionalXInfo(vals[0])
                        if len(vals) > 1:
                            w.SetAdditionalYInfo(vals[1])
                    elif isinstance(vals, dict):
                        if 'x' in vals:
                            w.SetAdditionalXInfo(vals['x'])
                        if 'y' in vals:
                            w.SetAdditionalYInfo(vals['y'])

                if 'translate match id' in histcfg and histcfg['translate match id']:
                    w.SetTranslate()

                p.AddHistogram(name, w)
                logging.debug("added histogram %s", name)
            except Exception as e:
                logging.error("unable to create histogram %s", name)
                logging.debug("got: %s - %s", type(e), e)
                if type(e) is not Exception:
                    raise

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
        for (flag, val) in flags:
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
        if splitter:
            logging.debug("splitting sample with %s", repr(splitter))

        split_count = module.fill(p, weights, log, splitter, select)
        total_count = p.GetEvents().size()

        for weight in weights:
            weight.RegisterCut(cutflow)

        if total_count > 0:
            ratio = split_count / float(total_count)
        else:
            ratio = 0

        cutflow.RegisterCutFromLast("Sample splitting", ratio)
        logging.info("filled %i events of %s", split_count, name)
