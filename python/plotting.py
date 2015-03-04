import ROOT as r

from ttH.TauRoast.utils import *

class Plot(object):
    __plots = []

    def __init__(self, code, name, labels, binning, dim=1, draw=""):
        self.__name = name
        self.__code = code
        self.__opt = draw

        self.__args = [name, ";".join([""] + labels)] + binning
        self.__hists = {}

        if dim == 1:
            self.__class = r.TH1F
        elif dim == 2:
            self.__class = r.TH2F

        Plot.__plots.append(self)

    def draw(self, *args):
        self.__hists.values()[0].Draw(self.__opt, *args)

    def fill(self, process, event, combo):
        try:
            hist = self.__hists[process]
        except KeyError:
            args = list(self.__args)
            args[0] += "_{p}".format(p=process)
            self.__hists[process] = self.__class(*args)
            hist = self.__hists[process]

        locals = {'histo': hist}
        globals = {
                'event': event,
                'btag': btag,
                'taus': combo.taus(),
                'leptons': combo.leptons(),
                'jets': combo.jets()
        }
        exec self.__code in locals, globals

    @property
    def name(self):
        return self.__name

    @classmethod
    def plots(cls):
        return cls.__plots
