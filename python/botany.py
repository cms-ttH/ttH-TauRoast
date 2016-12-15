import logging

import ROOT as r

from ttH.TauRoast.useful import code2leaf


class Leaf(object):
    __leaves = {}
    __finals = {}

    def __init__(self, name, kind, code, final=False):
        if kind.lower() not in ('i', 'f', '[f]', '[i]'):
            raise NotImplementedError("'{}' is not a valid type".format(kind))

        if name in Leaf.__leaves or name in Leaf.__finals:
            raise KeyError("Leaf {0} defined twice".format(name))

        if final:
            Leaf.__finals[name] = self
        else:
            Leaf.__leaves[name] = self

        self.__name = name
        self.__kind = kind

        typename = 'std::vector<float>'
        if kind.lower() == 'i':
            typename = 'int'
        elif kind.lower() == 'f':
            typename = 'float'
        elif kind.lower() == '[i]':
            typename = 'std::vector<int>'

        self._r = code2leaf(name, typename, code)

    @property
    def name(self):
        return self.__name

    @property
    def kind(self):
        return self.__kind

    @property
    def value(self):
        return None
        if not self.__kind:
            return self.__val[0]
        return self.__val

    def attach(self, tree):
        tree.SetBranchAddress(self.__name, self.__val)

    def grow(self, tree):
        self._r.grow(tree)

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
        self.__b = None
        for l in Leaf.leaves():
            l.grow(self.__t)

    def raw(self):
        return self.__t

    def mva(self, mvatree):
        self.__b = mvatree
        self.__b.SetName(self.__b.GetName() + "_mva")

    def __del__(self):
        if self.__b:
            self.__f.WriteObject(self.__b, self.__b.GetName())
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
            tree = self.__f.Get(name)
            tree.AddFriend(name + "_mva")
            tree.Draw(*args, **kwargs)
        except ReferenceError:
            self.__f.ls()
            raise ValueError("Can't access tree for {0}.".format(name))

    def __getitem__(self, key):
        return self.__f.Get(key)

    @classmethod
    def draw(cls, name, *args, **kwargs):
        cls.__instance._draw(name, *args)
