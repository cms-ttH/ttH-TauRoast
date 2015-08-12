from array import array

import ROOT as r

from ttH.TauRoast.useful import Snippet

class Leaf(Snippet):
    __leaves = {}

    def __init__(self, name, kind, code):
        if kind.lower() not in ('i', 'f'):
            raise NotImplementedError

        if name in Leaf.__leaves:
            raise KeyError("Leaf {0} defined twice".format(name))

        Leaf.__leaves[name] = self

        super(Leaf, self).__init__(code)

        self.__val = array(kind.lower(), [0] if kind.lower() == 'i' else [0.])
        self.__name = name
        self.__kind = kind

    def pick(self, event, selected, passed, globals=None):
        self.__val[0] = self._execute(event, selected, globals=globals, locals={'combos': passed})

    def register(self, tree):
        tree.Branch(self.__name, self.__val, '{0}/{1}'.format(self.__name, self.__kind.upper()))

    @classmethod
    def leaves(cls):
        return cls.__leaves.values()

class Forest(object):
    def __init__(self, filename, name):
        self.__f = r.TFile(filename, 'UPDATE')
        self.__t = r.TTree(str(name), 'ntuple')
        for l in Leaf.leaves():
            l.register(self.__t)

    def fill(self, event, selected, passed, globals=None):
        for l in Leaf.leaves():
            l.pick(event, selected, passed, globals)
        self.__t.Fill()

    def __del__(self):
        self.__f.WriteObject(self.__t, self.__t.GetName())
        self.__f.Close()
