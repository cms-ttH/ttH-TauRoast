class Process(object):
    __processes__ = {}

    def __init__(self, name, fullname, limitname):
        self._name = name
        self._fullname = fullname
        self._limitname = limitname

        Process.__processes__[name] = self

    def __unicode__(self):
        return self._name

    def __str__(self):
        return unicode(self).encode('utf-8')

    def process(self):
        raise NotImplementedError

    @classmethod
    def get(cls, name):
        return cls.__processes__[name]

    @classmethod
    def expand(cls, name):
        return map(cls.get, cls.__processes__[name].process())

class BasicProcess(Process):
    def __init__(self, name, paths, events, fullname=None, limitname=None, sample=-1, cross_section=1):
        super(BasicProcess, self).__init__(name, fullname, limitname)

        self.__paths = paths
        self.__events = events
        self.__sample = sample
        self.__cross_section = cross_section

    def process(self):
        return [self._name]

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
