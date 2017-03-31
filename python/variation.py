import logging
import os
import ROOT as r

from ttH.TauRoast.botany import Forest, Leaf

principal = None

class Principle(object):
    def __init__(self, wdir, variables, method="BDTG"):
        self.__method = method
        self.__reader = r.TMVA.Reader("!Color:!Silent")
        leaves = dict((l.name, l) for l in Leaf.leaves())
        for var in variables:
            pass
            # raise NotImplementedError()
            # self.__reader.AddVariable(var, leaves[var].value)
        fn = os.path.join(wdir, 'weights', 'TMVAClassification_{0}.weights.xml'.format(self.__method))
        if not os.path.isfile(fn):
            raise IOError()
        self.__reader.BookMVA(self.__method, fn)

    def evaluate(self):
        return self.__reader.EvaluateMVA(self.__method)

def setup(wdir, variables):
    global principal
    if principal is not None:
        logging.error("MVA already set up!")
        return
    try:
        principal = Principle(wdir, variables)
    except IOError:
        logging.warning("MVA not trained yet!")
