from array import array
import logging

import ROOT as r

from ttH.TauRoast.useful import Snippet

class Leaf(Snippet):
    __leaves = {}
    __finals = {}

    def __init__(self, name, kind, code, final=False):
        if kind.lower() not in ('i', 'f', '[f]'):
            raise NotImplementedError

        if name in Leaf.__leaves or name in Leaf.__finals:
            raise KeyError("Leaf {0} defined twice".format(name))

        if final:
            Leaf.__finals[name] = self
        else:
            Leaf.__leaves[name] = self

        super(Leaf, self).__init__(code)

        self.__name = name
        self.__kind = kind
        if kind.lower() == 'i':
            self.__val = array('i', [0])
        elif kind.lower() == 'f':
            self.__val = array('f', [0.])
        else:
            self.__kind = None
            self.__val = r.vector('float')()

    @property
    def name(self):
        return self.__name

    @property
    def kind(self):
        return self.__kind

    @property
    def value(self):
        if not self.__kind:
            return self.__val[0]
        return self.__val

    def pick(self, event, selected, passed, weight, globals=None):
        res = self._execute(event, selected, globals=globals,
                locals={
                    'combos': passed,
                    'weight': weight
        })

        if isinstance(res, list):
            self.__val.clear()
            for r in res:
                self.__val.push_back(r)
        else:
            self.__val[0] = res

    def attach(self, tree):
        tree.SetBranchAddress(self.__name, self.__val)

    def register(self, tree):
        if self.__kind:
            tree.Branch(self.__name, self.__val, '{0}/{1}'.format(self.__name, self.__kind.upper()))
        else:
            tree.Branch(self.__name, self.__val)

    @classmethod
    def leaves(cls):
        for k in sorted(cls.__leaves.keys()):
            yield cls.__leaves[k]
        for k in sorted(cls.__finals.keys()):
            yield cls.__finals[k]

class Tree(object):
    def __init__(self, filename, name):
        self.__f = r.TFile(filename, 'UPDATE')
        self.__t = r.TTree(str(name), 'ntuple')
        for l in Leaf.leaves():
            l.register(self.__t)

    def fill(self, event, selected, passed, weight, globals=None):
        for l in Leaf.leaves():
            l.pick(event, selected, passed, weight, globals)
        self.__t.Fill()

    def __del__(self):
        self.__f.WriteObject(self.__t, self.__t.GetName())
        self.__f.Close()

class Forest(object):
    __instance = None

    def __init__(self, filename):
        if Forest.__instance:
            raise ValueError("Forest already setup!")
        self.__f = r.TFile(filename, 'READ')
        Forest.__instance = self

    def __del__(self):
        self.__f.Close()
        Forest.__instance = None

    def _draw(self, name, *args, **kwargs):
        logging.debug("Generating histogram for {0} with: {1}".format(name, args))
        try:
            self.__f.Get(name).Draw(*args, **kwargs)
        except ReferenceError:
            self.__f.ls()
            raise ValueError("Can't access tree for {0}.".format(name))

    def __getitem__(self, key):
        return self.__f.Get(key)

    @classmethod
    def draw(cls, name, *args, **kwargs):
        cls.__instance._draw(name, *args)
