import ROOT as r

efake = 1.05
jfake = 1.2
ideff = 1.06

def calculate_weights(event, combo):
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
            'idEffDown': (2 - ideff)**num_taus
    }

    return dict((k.lower(), v) for k, v in sys.items())
