import glob
import logging
import os
import re
import time

import ROOT as r
r.gSystem.Load("libttHTauRoast")

from ttH.TauRoast.botany import Tree
from ttH.TauRoast.useful import vectorize

NTUPLE_GLOB = '{channel}_test*.root'


class Process(object):
    __processes__ = {}

    def __init__(self, name, fullname, limitname):
        self._name = name
        self._fullname = fullname
        self._limitname = limitname if limitname else name

        if name in Process.__processes__:
            raise KeyError("Process with name {0} defined twice".format(name))
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

    def __init__(self, name, paths, events, fullname=None, limitname=None, cross_section=1, cutflow="default", additional_cuts=None):
        super(BasicProcess, self).__init__(name, fullname, limitname)

        self.__paths = paths
        self.__events = events
        self.__cross_section = cross_section
        self.__cutflow = cutflow
        self.__add_cuts = additional_cuts if additional_cuts else []

    def _setup_counts(self, counts):
        from ttH.TauRoast.cutting import StaticCut
        if len(counts) == 0:
            counts.append(StaticCut("Dataset"))

            rx = re.compile(r'pass\w*Cut\(.*"(.*)".*\)')
            fn = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'plugins', 'TauProcessor.cc')
            with open(fn) as f:
                for line in f:
                    m = rx.search(line)
                    if m:
                        counts.append(StaticCut(m.group(1)))

    def copy(self, fct=lambda s: s):
        return BasicProcess(fct(self._name), self.__paths, self.__events,
                            fct(self._fullname), fct(self._limitname),
                            self.__cross_section, self.__add_cuts)

    def analyze(self, filename, counts, cuts, weights, systematics, basedir, limit=-1, debug=False):
        from ttH.TauRoast.useful import config
        from ttH.TauRoast.printable import SyncSaver

        if str(self).startswith("collisions"):
            systematics = "NA"

        if debug:
            for i, cut in enumerate(cuts):
                cut.callback(SyncSaver(os.path.join(os.path.dirname(filename), "cut_{0}_{1}.txt".format(self, i)), systematics))

        files = sum([glob.glob(os.path.join(basedir, p, NTUPLE_GLOB.format(channel=config.channel))) for p in self.__paths], [])
        cfiles = vectorize(files, 'std::string')
        if len(files) == 0:
            raise IOError("could not find any files in {}".format(", ".join(self.__paths)))
        hist = r.fastlane.get_cuts(config.channel + "Taus", cfiles)
        if hist is None:
            raise IOError("Could not produce cutflow histogram from directory '{0}'".format(os.path.join(basedir, p)))

        self._setup_counts(counts)
        for n, cut in enumerate(counts[1:], 1):
            cut[self] = hist.GetBinContent(n)

        counts[0][self] = self.__events

        for w in weights:
            w[self] = 0

        suffix = '' if systematics == 'NA' else '_' + systematics
        tree = Tree(filename, str(self) + suffix)

        ccuts = vectorize(cuts, 'fastlane::Cut*')
        cweights = vectorize(weights, 'fastlane::StaticCut*')

        def log(i):
            logging.info("processing {0}, event {1}".format(str(self), i))
        now = time.clock()
        r.fastlane.process(str(self), config.channel, cfiles, tree.raw(), ccuts, cweights, systematics, log, limit)
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
    def cutflow(self):
        return self.__cutflow

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
