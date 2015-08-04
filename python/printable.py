class SyncSaver(object):
    def __init__(self, filename):
        self.__f = open(filename, 'w')
        self.__fmt = "%6d %6d %10d  " + \
                "%+2d  %6.2f %+4.2f %+4.2f   " + \
                "%+2d  %6.2f %+4.2f %+4.2f    " + \
                "%6.1f  %+4.2f    %d \n"

    def __del__(self):
        self.__f.close()

    def __call__(self, event, combo):
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
