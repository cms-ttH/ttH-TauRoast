import os

from useful import btag

class SyncSaver(object):
    def __init__(self, channel, filename):
        if not os.path.isdir(os.path.dirname(filename)):
            os.makedirs(os.path.dirname(filename))
        self.__f = open(filename, 'w')
        if channel == 'lj':
            self.__f.write("\n")
            self._fct = self._print_lepjets
        elif channel == 'll':
            self.__fmt = "%6d %6d %10d  " + \
                    "%+2d  %6.2f %+4.2f %+4.2f   " + \
                    "%+2d  %6.2f %+4.2f %+4.2f    " + \
                    "%6.1f  %+4.2f    %d \n"
            self._fct = self._print_dilepton
        else:
            raise ValueError, channel

    def __call__(self, *args):
        self._fct(*args)

    def __del__(self):
        self.__f.close()

    def _print_lepjets(self, event, combo):
        def dtos(d):
            s = "{0:.6}".format(d)
            return "0" if s == "0.0" else s
        leptons = combo.leptons()
        jets = combo.jets()
        met = combo.met()
        first = leptons[0]
        jpts = ([dtos(j.p4().pt()) for j in jets] + ["0"] * 4)[:4]
        jcsvs = ([dtos(j.csv()) for j in jets] + ["0"] * 4)[:4]
        s = ",".join(map(str,
            [
                event.run(), event.lumi(), event.event(),
                1, 0,
                dtos(first.p4().pt()), dtos(first.p4().eta()), dtos(first.p4().phi()), dtos(first.relativeIsolation()), first.pdgId(),
            ] + [0] * 5 + jpts + jcsvs + map(dtos, [met.pt(), met.phi()])
        ))
        self.__f.write(s + "\n")

    def _print_dilepton(self, event, combo):
        leptons = combo.leptons()
        first, second = leptons[:2]
        self.__f.write(self.__fmt % (
            event.run(), event.lumi(), event.event(),
            first.pdgId(), first.p4().pt(), first.p4().eta(), first.p4().phi(),
            second.pdgId(), second.p4().pt(), second.p4().eta(), second.p4().phi(),
            event.met().pt(), event.met().phi(), len(combo.jets())))

#         fprintf(lep1,"%6d %6d %10d  %+2d  %6.2f %+4.2f %+4.2f   %+2d  %6.2f %+4.2f %+4.2f    %6.1f  %+4.2f    %d \n",
#             runNumber_intree, lumiBlock_intree, eventnum_intree,
#             preselected_leptons[0].pdgID, preselected_leptons[0].obj.Pt(), preselected_leptons[0].obj.Eta(), preselected_leptons[0].obj.Phi(),
#             preselected_leptons[1].pdgID, preselected_leptons[1].obj.Pt(), preselected_leptons[1].obj.Eta(), preselected_leptons[1].obj.Phi(),
#           theMET[0].pt_forSync, theMET[0].phi_forSync, int(preselected_jets.size()));
