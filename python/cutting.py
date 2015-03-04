from ttH.TauRoast.utils import Snippet

class Cut(Snippet):
    def __init__(self, name, code=None):
        if not code:
            code = name
        super(Cut, self).__init__(code)
        self.__name = name
        self.__counts = {}

    def __call__(self, process, event, combo):
        passed = self._execute(event, combo, use_exec=False)
        if passed:
            try:
                self.__counts[process] += 1
            except KeyError:
                self.__counts[process] = 1
        return passed

    def __getitem__(self, key):
        return self.__counts[key]

    def __str__(self):
        return self.__name
