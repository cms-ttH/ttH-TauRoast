import glob
import logging
import os
import re
import time

import ROOT as r
r.gSystem.Load("libttHTauRoast")

from ttH.TauRoast.botany import Tree
from ttH.TauRoast.useful import code2cut

class Process(object):
    __processes__ = {}
    __limitnames__ = set()

    def __init__(self, name, fullname, limitname):
        self._name = name
        self._fullname = fullname
        self._limitname = limitname if limitname else name

        if name in Process.__processes__:
            raise KeyError("Plot with name {0} defined twice".format(name))
        if limitname in Process.__limitnames__ and limitname:
            raise KeyError("Plot with limit name {0} defined twice".format(limitname))
        elif limitname:
            Process.__limitnames__.add(limitname)
        Process.__processes__[name] = self

    def copy(self, fct=lambda s: s):
        return Process(fct(self._name), fct(self._fullname), fct(self._limitname))

    def __unicode__(self):
        return self._name

    def __str__(self):
        return unicode(self).encode('utf-8')

    def process(self):
        raise NotImplementedError

    @property
    def fullname(self):
        return self._fullname

    @property
    def limitname(self):
        return self._limitname

    @classmethod
    def get(cls, name):
        return cls.__processes__[name]

    @classmethod
    def procs(cls):
        return cls.__processes__.values()

    @classmethod
    def expand(cls, name):
        return map(cls.get, cls.__processes__[str(name)].process())

class BasicProcess(Process):
    def __init__(self, name, paths, events, fullname=None, limitname=None, sample=-1, cross_section=1, additional_cuts=None):
        super(BasicProcess, self).__init__(name, fullname, limitname)

        self.__paths = paths
        self.__events = events
        self.__sample = sample
        self.__cross_section = cross_section
        self.__add_cuts = additional_cuts if additional_cuts else []

    def copy(self, fct=lambda s: s):
        return BasicProcess(fct(self._name), self.__paths, self.__events,
                fct(self._fullname), fct(self._limitname),
                self.__sample, self.__cross_section, self.__add_cuts)

    def analyze(self, filename, counts, cuts, weights, systematics, basedir, debug=False):
        logging.info("processing {}".format(self))

        if str(self).startswith("collisions"):
            systematics = "NA"

        from ttH.TauRoast.cutting import StaticCut
        from ttH.TauRoast.printable import SyncSaver

        if debug:
            for i, cut in enumerate(cuts):
                cut.callback(SyncSaver(os.path.join(os.path.dirname(filename), "cut_{0}_{1}.txt".format(self, i)), systematics))

        hist = None
        for p in self.__paths:
            for fn in glob.glob(os.path.join(basedir, p, '*.root')):
                try:
                    f = r.TFile(fn)
                    h = f.Get("taus/cuts")
                    h.SetDirectory(0)
                    f.Close()
                    if hist is None:
                        hist = h
                    else:
                        for n in range(hist.GetNbinsX()):
                            label = hist.GetXaxis().GetBinLabel(n+1)
                            h.GetXaxis().SetBinLabel(n+1, label)
                        hist.Add(h)
                except AttributeError as e:
                    logging.error("could not process {0}".format(fn))
                    raise

        if hist is None:
            raise IOError("No files found in '{0}'".format(os.path.join(basedir, p)))

        if len(counts) == 0:
            counts.append(StaticCut("Dataset"))
            names = []

            rx = re.compile(r'pass\w*Cut\(.*"(.*)".*\)')
            fn = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'plugins', 'TauProcessor.cc')
            with open(fn) as f:
                for line in f:
                    m = rx.search(line)
                    if m:
                        counts.append(StaticCut(m.group(1)))

        for n, cut in enumerate(counts[1:], 1):
            cut[self] = hist.GetBinContent(n)

        counts[0][self] = self.__events

        for w in weights:
            w[self] = 0

        tree = Tree(filename, self)
        chain = r.TChain("taus/events")
        for p in self.__paths:
            chain.Add(os.path.join(basedir, p, '*.root'))

        ccuts = r.std.vector('fastlane::Cut*')()
        for c in cuts:
            ccuts.push_back(c.raw())
        cweights = r.std.vector('fastlane::StaticCut*')()
        for c in weights:
            cweights.push_back(c.raw())

        logging.info("events left to process: {0}".format(chain.GetEntries()))

        def log(i):
            logging.info("processing event {0}".format(i))
        now = time.clock()
        r.fastlane.process(str(self), chain, tree.raw(), ccuts, cweights, systematics, log);
        logging.debug("time spent processing: {0}".format(time.clock() - now))

    def process(self):
        return [self._name]

    @property
    def additional_cuts(self):
        return self.__add_cuts

    @property
    def cross_section(self):
        return self.__cross_section

    @property
    def events(self):
        return self.__events

    @property
    def paths(self):
        return self.__paths

class CombinedProcess(Process):
    def __init__(self, name, subprocesses, limitname=None, fullname=None):
        super(CombinedProcess, self).__init__(name, fullname, limitname)

        self.__subprocesses = subprocesses

    def copy(self, fct=lambda s: s):
        return CombinedProcess(fct(self._name), map(fct, self.__subprocesses),
                fct(self._limitname), fct(self._fullname))

    def process(self):
        return sum((Process.get(n).process() for n in self.__subprocesses), [])
