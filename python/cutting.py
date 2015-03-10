import sys

from ttH.TauRoast.processing import Process
from ttH.TauRoast.utils import Snippet

class Cut(Snippet):
    def __init__(self, name, code=None):
        if not code:
            code = name
        super(Cut, self).__init__(code)
        self.__name = name
        self.__counts = {}
        self.__last = None

    def __call__(self, process, event, combo):
        passed = self._execute(event, combo, use_exec=False)

        if passed:
            id = (event.run(), event.lumi(), event.event())
            if id == self.__last:
                return passed
            self.__last = id

            try:
                self.__counts[process] += 1
            except KeyError:
                self.__counts[process] = 1
        return passed

    def __getitem__(self, key):
        return self.__counts[key]

    def __unicode__(self):
        return self.__name

    def __str__(self):
        return unicode(self).encode('utf-8')

def cutflow(cuts, procs, f=sys.stdout):
    expanded_proc = [Process.expand(proc) for proc in procs]

    namelength = len(max(unicode(cut) for cut in cuts))
    fieldlength = max(len(p) for p in procs)
    for ps in expanded_proc:
        fieldlength = max(len(str(sum(cuts[0][p] for p in ps))), fieldlength)

    format = "{{:{0}}}".format(namelength) + "   {{:{0}}}".format(fieldlength) * len(procs) + "\n"
    f.write(format.format("Cut", *procs))
    f.write("-" * namelength + ("   " + "-" * fieldlength) * len(procs) + "\n")
    for cut in cuts:
        f.write(format.format(cut, *[sum(cut[p] for p in ps) for ps in expanded_proc]))
