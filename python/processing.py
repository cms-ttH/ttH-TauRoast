import glob
import logging
import math
import os
import re
import time

import ROOT as r
r.gSystem.Load("libttHTauRoast")

from ttH.TauRoast import training
from ttH.TauRoast.botany import Tree
from ttH.TauRoast.useful import vectorize

NTUPLE_GLOB = '{channel}_test*.root'


class Process(object):
    """A representation of a physical process."""
    __processes__ = {}

    def __init__(self, name, fullname, limitname):
        """Create a new process called `name`.

        Use a more descriptive `fullname` for legends, and `limitname` when
        saving to set limits.
        """
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
        return cls.__processes__[str(name)]

    @classmethod
    def pop(cls, name):
        return cls.__processes__.pop(name)

    @classmethod
    def procs(cls):
        return cls.__processes__.values()

    @classmethod
    def expand(cls, name):
        return map(cls.get, cls.__processes__[str(name)].process())


class BasicProcess(Process):

    def __init__(self, name, paths, events, fullname=None, limitname=None, cross_section=1, cutflow="default", additional_cuts=None, relativesys=None):
        super(BasicProcess, self).__init__(name, fullname, limitname)

        self.__paths = paths
        self.__events = events
        self.__cross_section = cross_section
        self.__cutflow = cutflow
        self.__add_cuts = additional_cuts if additional_cuts else []
        self.relativesys(relativesys if relativesys else [])

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

    def copy(self, fct=lambda s: s, cutflow=None):
        return BasicProcess(fct(self._name), self.__paths, self.__events,
                            fct(self._fullname), fct(self._limitname),
                            self.__cross_section, cutflow if cutflow else self.__cutflow,
                            self.__add_cuts)

    def relativesys(self, newsys=None):
        """Query and set yield uncertainties.

        If called with an argument, set the yield uncertainties. Return the
        current yield uncertainties.
        """
        if newsys is not None:
            self.__relativesys = math.sqrt(sum(s**2 for s in newsys))
        return self.__relativesys

    def analyze(self, cfg, filename, counts, cuts, weights, systematics, debug=False):
        from ttH.TauRoast.useful import config
        from ttH.TauRoast.printable import SyncSaver

        tau_id = cfg.get(u"tau ID", "Tight")
        basedir = cfg['ntupledir']
        limit = cfg.get('event limit', -1)

        if str(self).startswith("collisions") or str(self).startswith("fakes"):
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
        r.fastlane.process(str(self), config.channel, cfiles, tree.raw(), ccuts, cweights, systematics, tau_id, log, limit)
        logging.debug("time spent processing: {0}".format(time.clock() - now))

    def add_mva(self, cfg, filename, systematics):
        if 'mvadict' in cfg:
            def transform(names):
                for name in names:
                    yield cfg['mvadict'].get(name, name)
        else:
            transform = None
        suffix = '' if systematics == 'NA' else '_' + systematics
        tree = Tree(filename, str(self) + suffix, read=True)
        logging.info("evaluating MVA for {}".format(self))
        now = time.clock()
        try:
            training.evaluate(cfg, tree, cfg.get('mvas', []), transform)
        except (IOError, ValueError) as e:
            logging.error("can't evaluate MVA: {}".format(e))
        logging.debug("time spent evaluating MVA: {0}".format(time.clock() - now))

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
    """A category of processes."""

    def __init__(self, name, subprocesses, limitname=None, fullname=None, factor=1, relativesys=None):
        super(CombinedProcess, self).__init__(name, fullname, limitname)

        self.__subprocesses = subprocesses
        self.__factor = factor
        if relativesys:
            self.relativesys(relativesys)

    def copy(self, fct=lambda s: s, factor=None):
        return CombinedProcess(fct(self._name), self.__subprocesses,
                               fct(self._limitname), fct(self._fullname),
                               factor if factor is not None else self.__factor)

    def relativesys(self, sys):
        """Set relative yield uncertainties."""
        for p in self.__subprocesses:
            Process.get(p).relativesys(sys)

    def process(self):
        return sum((Process.get(n).process() for n in self.__subprocesses), [])

    @property
    def subprocesses(self):
        return self.__subprocesses

    @property
    def factor(self):
        return self.__factor


def split_procs(config):
    subtract = []
    for p in set(sum((Process.expand(p) for p in config['plot'] + config['limits']), [])):
        if isinstance(p, BasicProcess) and p.cutflow == 'signal' and not str(p).endswith('signal'):
            if not any([str(p).startswith(x) for x in 'ttH collisions fakes'.split()]):
                p.copy(fct=lambda s: '{}_fake'.format(s), cutflow='fake')
                subtract.append(str(p) + '_fake')
    try:
        p = Process.pop('fakes_single')
        p.copy(fct=lambda s: s + '_raw')
        CombinedProcess(
            'fakes_single',
            ['fakes_single_raw'] + subtract,
            p.limitname,
            p.fullname,
            -1
        )
    except KeyError:
        pass
