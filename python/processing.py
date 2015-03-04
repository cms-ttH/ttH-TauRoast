class Process(object):
    __processes__ = {}

    def __init__(self, name, fullname, limitname):
        self.__name = name
        self.__fullname = fullname
        self.__limitname = limitname

        Process.__processes__[name] = self

class BasicProcess(Process):
    def __init__(self, name, path, events, fullname=None, limitname=None, sample=-1, cross_section=1):
        super(BasicProcess, self).__init__(name, fullname, limitname)

        self.__path = path
        self.__events = events
        self.__sample = sample
        self.__cross_section = cross_section

class CombinedProcess(Process):
    def __init__(self, name, subprocesses, limitname=None, fullname=None):
        super(CombinedProcess, self).__init__(name, fullname, limitname)

        self.__subprocesses = subprocesses
