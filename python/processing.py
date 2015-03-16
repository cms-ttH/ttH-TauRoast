import logging
import os

import ROOT as r

class Process(object):
    __processes__ = {}
    __limitnames__ = set()

    def __init__(self, name, fullname, limitname):
        self._name = name
        self._fullname = fullname
        self._limitname = limitname

        if name in Process.__processes__:
            raise KeyError("Plot with name {0} defined twice".format(name))
        if limitname in Process.__limitnames__ and limitname:
            raise KeyError("Plot with limit name {0} defined twice".format(limitname))
        elif limitname:
            Process.__limitnames__.add(limitname)
        Process.__processes__[name] = self

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
    def expand(cls, name):
        return map(cls.get, cls.__processes__[name].process())

class BasicProcess(Process):
    def __init__(self, name, paths, events, fullname=None, limitname=None, sample=-1, cross_section=1, additional_cuts=None):
        super(BasicProcess, self).__init__(name, fullname, limitname)

        self.__paths = paths
        self.__events = events
        self.__sample = sample
        self.__cross_section = cross_section
        self.__add_cuts = additional_cuts if additional_cuts else []

    def analyze(self, cuts, plots, basedir):
        logging.info("processing {}".format(self))

        cuts[0][self] = self.__events

        t = r.TChain("taus/events")
        for p in self.__paths:
            t.Add(os.path.join(basedir, p, '*.root'))

        event = r.superslim.Event()
        t.SetBranchAddress('Event', r.AddressOf(event))

        for i in range(t.GetEntries()):
            if i % 1000 == 1:
                logging.info("processing event {0}".format(i))

            t.GetEntry(i)
            for combo in event.combos():
                for cut in cuts:
                    if not cut(self, event, combo):
                        break
                else:
                    for plot in plots:
                        try:
                            plot.fill(self, event, combo)
                        except Exception, e:
                            print "error in", plot.name
                            print e
                break

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

    def process(self):
        procs = Process.processes()
        return sum((procs[n].process() for n in self.__subprocesses), [])
