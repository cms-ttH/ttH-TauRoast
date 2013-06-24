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
    for (proc, cfg) in config['processes'].items():
        if not proc in config['analysis']['process']:
            continue

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
        weights = roast.Weights()

        for e in p.GetGoodEventsForSignal():
            branches.GetEntry(e.entry)

            idx = e.combos[0]

            w_top_pt = 1.
            w_lepton = 1.
            w_pu = 1.
            w_tau1_trigger = 1.
            w_tau2_trigger = 1.
            w_tau_id = 1.
            w_q2 = 1.
            w_jet_csv = 1.

            if 'topPt' in flags:
                if flags['topPt'] == "normal":
                    w_top_pt = branches.Ev_topPtWeight
                elif flags['topPt'] == "up":
                    w_top_pt = branches.Ev_topPtWeightUp
                elif flags['topPt'] == "down":
                    w_top_pt = branches.Ev_topPtWeightDown

            if 'leptonSF' in flags:
                if flags['leptonSF'] == "normal":
                    w_lepton = branches.TTL_LeptonEventWeight[idx]

            if 'PUcorr' in flags:
                if flags['PUcorr'] == "normal":
                    w_pu = branches.Ev_puWeight
                elif flags['PUcorr'] == "up":
                    w_pu = branches.Ev_puWeightUp
                elif flags['PUcorr'] == "down":
                    w_pu = branches.Ev_puWeightDown
                elif flags['PUcorr'] == "2012A":
                    w_pu = branches.Ev_puWeight2012A
                elif flags['PUcorr'] == "2012B":
                    w_pu = branches.Ev_puWeight2012B
                elif flags['PUcorr'] == "2012C":
                    w_pu = branches.Ev_puWeight2012C
                elif flags['PUcorr'] == "2012D":
                    w_pu = branches.Ev_puWeight2012D
                elif flags['PUcorr'] == "2012AB":
                    w_pu = branches.Ev_puWeight2012AB
                elif flags['PUcorr'] == "2012BC":
                    w_pu = branches.Ev_puWeight2012BC
                elif flags['PUcorr'] == "2012CD":
                    w_pu = branches.Ev_puWeight2012CD
                elif flags['PUcorr'] == "2012ABC":
                    w_pu = branches.Ev_puWeight2012ABC

            if 'qSquared' in flags:
                if flags['qSquared'] == "up":
                    iQ2SysWeight = branches.Ev_q2WeightUp
                elif flags['qSquared'] == "down":
                    iQ2SysWeight = branches.Ev_q2WeightDown

            if 'CSVeventWeight' in flags:
                if flags['CSVeventWeight'] == "normal":
                    w_jet_csv = branches.TTL_CSVeventWeight[idx]
            if 'CSVeventWeightHF' in flags:
                if flags['CSVeventWeightHF'] == "down":
                    w_jet_csv = branches.TTL_CSVeventWeightHFdown[idx]
                elif flags['CSVeventWeightHF'] == "up":
                    w_jet_csv = branches.TTL_CSVeventWeightHFup[idx]
            if 'CSVeventWeightLF' in flags:
                if flags['CSVeventWeightLF'] == "down":
                    w_jet_csv = branches.TTL_CSVeventWeightLFdown[idx]
                elif flags['CSVeventWeightLF'] == "up":
                    w_jet_csv = branches.TTL_CSVeventWeightLFup[idx]
            if 'CSVeventWeightHFStats1' in flags:
                if flags['CSVeventWeightHFStats1'] == "down":
                    w_jet_csv = branches.TTL_CSVeventWeightHFStats1down[idx]
                elif flags['CSVeventWeightHFStats1'] == "up":
                    w_jet_csv = branches.TTL_CSVeventWeightHFStats1up[idx]
            if 'CSVeventWeightLFStats1' in flags:
                if flags['CSVeventWeightLFStats1'] == "down":
                    w_jet_csv = branches.TTL_CSVeventWeightLFStats1down[idx]
                elif flags['CSVeventWeightLFStats1'] == "up":
                    w_jet_csv = branches.TTL_CSVeventWeightLFStats1up[idx]
            if 'CSVeventWeightHFStats2' in flags:
                if flags['CSVeventWeightHFStats2'] == "down":
                    w_jet_csv = branches.TTL_CSVeventWeightHFStats2down[idx]
                elif flags['CSVeventWeightHFStats2'] == "up":
                    w_jet_csv = branches.TTL_CSVeventWeightHFStats2up[idx]
            if 'CSVeventWeightLFStats2' in flags:
                if flags['CSVeventWeightLFStats2'] == "down":
                    w_jet_csv = branches.TTL_CSVeventWeightLFStats2down[idx]
                elif flags['CSVeventWeightLFStats2'] == "up":
                    w_jet_csv = branches.TTL_CSVeventWeightLFStats2up[idx]

            w = 1.
            match = -1
            if 'eTauFake' in flags:
                if flags['eTauFake'] == "up":
                    match = 4
                    w = config['physics']['systematics']['e_fake']
                elif flags['eTauFake'] == "down":
                    match = 4
                    w = 2 - config['physics']['systematics']['e_fake']
            if 'jetTauFake' in flags:
                if flags['jetTauFake'] == "up":
                    match = 1
                    w = config['physics']['systematics']['jet_fake']
                elif flags['jetTauFake'] == "down":
                    match = 1
                    w = 2 - config['physics']['systematics']['jet_fake']
            if 'tauIdEff' in flags:
                if flags['tauIdEff'] == "up":
                    match = 2
                    w = config['physics']['systematics']['tau_id']
                elif flags['tauIdEff'] == "down":
                    match = 2
                    w = 2 - config['physics']['systematics']['tau_id']

            if branches.GetTau1MatchIndex(idx) == match:
                w_tau_id *= w;
            if branches.GetTau2MatchIndex(idx) == match:
                w_tau_id *= w;

            weights.topPtCorrection += w_top_pt
            weights.leptonCorrection += w_lepton
            weights.puCorrection += w_pu
            weights.tau1Trigger += w_tau1_trigger * w_pu
            weights.tau2Trigger += w_tau2_trigger * w_tau1_trigger * w_pu
            weights.tauIdSys += w_tau_id
            weights.q2Sys += w_q2
            weights.jetCSV += w_jet_csv
            weights.total += 1

            w_tot = w_lepton * w_pu * w_tau1_trigger * w_tau2_trigger * w_tau_id * w_q2 * w_top_pt

            branches.FillHistograms(p.GetHContainerForSignal(), idx, w_tot, w_pu)
            # print p.GetHContainerForSignal().size()
            # print e.entry
            # print p.GetGoodEventsForSignal().size()

        top_pt_eff = 0
        lepton_eff = 0
        pu_eff = 0
        tau1_trigger_eff = 0
        tau2_trigger_eff = 0
        tau_id_eff = 0
        q2_eff = 0
        jet_csv_eff = 0

        if weights.total > 0:
            top_pt_eff = weights.topPtCorrection / weights.total
            lepton_eff = weights.leptonCorrection / weights.total
            pu_eff = weights.puCorrection / weights.total
            tau1_trigger_eff = weights.tau1Trigger / weights.puCorrection
            tau2_trigger_eff = weights.tau2Trigger / weights.tau1Trigger
            tau_id_eff = weights.tauIdSys / weights.total
            q2_eff = weights.q2Sys / weights.total
            jet_csv_eff = weights.jetCSV / weights.total

            print math.sqrt(weights.tau2Trigger / weights.total)
            # for (name, wrapper) in p.GetHContainerForSignal():
                # wrapper.ScaleErrorBy(math.sqrt(weights.tau2Trigger /
                    # weights.total))

        if "topPt" in flags:
            cutflow.RegisterCut("topPt SF", 2, top_pt_eff * cutflow.GetLastCountForSignal())
        if "leptonSF" in flags:
            cutflow.RegisterCut("topPt SF", 2, top_pt_eff * cutflow.GetLastCountForSignal())
        if "PUcorr" in flags:
            cutflow.RegisterCut("PU reweighing", 2, pu_eff * cutflow.GetLastCountForSignal())
        if "qSquared" in flags:
            cutflow.RegisterCut("Q^2 shift", 2, q2_eff * cutflow.GetLastCountForSignal())
        if "eTauFake" in flags or "jetTauFake" in flags or "tauIdEff" in flags:
            cutflow.RegisterCut("tau ID sys", 2, tau_id_eff * cutflow.GetLastCountForSignal())
        if "JetCSVWeight" in flags:
            cutflow.RegisterCut("jet CSV wt.", 2, jet_csv_eff * cutflow.GetLastCountForSignal())
