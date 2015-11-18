import os

import ROOT as r

if r.gROOT.GetVersion().startswith('6'):
    header = os.path.join(os.environ["LOCALRT"], 'src', 'MiniAOD', 'MiniAODHelper', 'interface', 'CSVHelper.h')
    r.gInterpreter.ProcessLine('#include "{0}"'.format(header))

r.gSystem.Load("libMiniAODMiniAODHelper")

# Tau systematics
efake = 1.05
jfake = 1.2
ideff = 1.06

# PU systematics
class PUWeigher(object):
    def __init__(self, fn, histname='pileup'):
        import SimGeneral.MixingModule.mix_2015_25ns_Startup_PoissonOOTPU_cfi as mc_pu

        mc_vals = mc_pu.mix.input.nbPileupEvents.probValue
        mc_xs = mc_pu.mix.input.nbPileupEvents.probFunctionVariable
        mc = dict(zip(mc_xs, mc_vals))

        self.weights = []

        f = r.TFile(fn)
        hist = f.Get(histname)
        hist.Scale(1. / hist.Integral())
        for i in range(hist.GetNbinsX()):
            self.weights.append(hist.GetBinContent(i + 1) / mc[i])
        f.Close()

    def __call__(self, n):
        return self.weights[n]

puhelper = PUWeigher(os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data', 'pu.root'))
puhelper_up = PUWeigher(os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data', 'pu.root'), 'pileup_up')
puhelper_down = PUWeigher(os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data', 'pu.root'), 'pileup_down')

# Jet CSV systematics
csvsys = [
        ("NA", 0),
        ("JERUp", 0),
        ("JERDown", 0),
        ("JESUp", 7),
        ("JESDown", 8),
        ("LFContUp", 9),
        ("LFContDown", 10),
        ("HFContUp", 11),
        ("HFContDown", 12),
        ("HFStats1Up", 13),
        ("HFStats1Down", 14),
        ("HFStats2Up", 15),
        ("HFStats2Down", 16),
        ("LFStats1Up", 17),
        ("LFStats1Down", 18),
        ("LFStats2Up", 19),
        ("LFStats2Down", 20),
        ("CharmErr1Up", 21),
        ("CharmErr1Down", 22),
        ("CharmErr2Up", 23),
        ("CharmErr2Down", 24)
]

ivec = r.std.vector('int')
dvec = r.std.vector('double')
csvhelper = r.CSVHelper("data/csv_rwt_hf_IT_FlatSF_2015_07_27.root", "data/csv_rwt_lf_IT_FlatSF_2015_07_27.root")

def calculate_weights(event, combo, shift):
    jetpt, jeteta, jetcsv, jetflv = dvec(), dvec(), dvec(), ivec()
    for j in combo.jets():
        jetpt.push_back(j.p4().pt())
        jeteta.push_back(j.p4().eta())
        jetcsv.push_back(j.csv())
        jetflv.push_back(j.genPdgId())

    taus = [abs(t.genPdgId()) for t in combo.taus()]
    num_jets = len([n for n in taus if n not in (0, 11, 13, 15, 22, 23, 24, 25)])
    num_taus = len([n for n in taus if n == 15])
    num_eles = len([n for n in taus if n == 11])

    sys = {
            'jetTauFakeUp': jfake**num_jets,
            'jetTauFakeDown': (2 - jfake)**num_jets,
            'eTauFakeUp': efake**num_eles,
            'eTauFakeDown': (2 - efake)**num_eles,
            'idEffUp': ideff**num_taus,
            'idEffDown': (2 - ideff)**num_taus,
    }

    hf, lf, cf = r.Double(), r.Double(), r.Double()
    csv = csvhelper.getCSVWeight(jetpt, jeteta, jetcsv, jetflv, dict(csvsys)[shift], hf, lf, cf)

    sys['CSVWeight'] = csv
    sys['CSVWeightLF'] = lf
    sys['CSVWeightHF'] = hf
    sys['CSVWeightCF'] = cf

    if shift == 'NA':
        for name, idx in csvsys[5:]:
            sys[name] = csvhelper.getCSVWeight(jetpt, jeteta, jetcsv, jetflv, idx, hf, lf, cf)
    else:
        for name, idx in csvsys[5:]:
            sys[name] = 1.

    sys['PUWeight'] = puhelper(event.ntv())
    sys['PUWeightUp'] = puhelper_up(event.ntv())
    sys['PUWeightDown'] = puhelper_down(event.ntv())

    return dict((k.lower(), v) for k, v in sys.items())
