import os

class SyncSaver(object):
    def __init__(self, channel, filename):
        if not os.path.isdir(os.path.dirname(filename)):
            os.makedirs(os.path.dirname(filename))
        self.__f = open(filename, 'w')
        if channel == 'lj':
            self.__fmt = "%6d %8d %10d   " + \
                    "%6.2f %+4.2f %+4.2f   " + \
                    "%6.2f %6.2f %6.2f %6.2f   " + \
                    "%3d\n"
            # "%+7.3f %+7.3f %+7.3f %+7.3f   %+7.3f   %2d  %2d   %2d   %2d  %2d\n"
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
        leptons = combo.leptons()
        jets = combo.jets()
        first = leptons[0]
        jet1 = jets[0].p4().pt() if len(jets) > 0 else -99
        jet2 = jets[1].p4().pt() if len(jets) > 1 else -99
        jet3 = jets[2].p4().pt() if len(jets) > 2 else -99
        jet4 = jets[3].p4().pt() if len(jets) > 3 else -99
        self.__f.write(self.__fmt % (
            event.run(), event.lumi(), event.event(),
            first.p4().pt(), first.p4().eta(), first.p4().phi(),
            jet1, jet2, jet3, jet4, len(jets)))

          # printf("%6d %8d %10d   %6.2f %+4.2f %+4.2f   %6.2f %6.2f %6.2f %6.2f   %+7.3f %+7.3f %+7.3f %+7.3f   %+7.3f   %2d  %2d   %2d   %2d  %2d\n",
          #   run, lumi, event,
          #   lep1_pt, lep1_eta, lep1_phi,
          #   jet1_pt, jet2_pt, jet3_pt, jet4_pt,
          #   jet1_CSVv2, jet2_CSVv2, jet3_CSVv2, jet4_CSVv2,
          #   MET,
          #   n_jets, n_btags,
          #   ttHFCategory,
          #   n_toptags, n_higgstags);

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
