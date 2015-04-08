import sys

from ttH.TauRoast.processing import Process
from ttH.TauRoast.useful import Snippet

class Cut(Snippet):
    def __init__(self, name, code=None):
        if not code:
            code = name
        super(Cut, self).__init__(code)
        self._name = name
        self._counts = {}
        self.__last = None

    def __call__(self, process, event, combo):
        passed = self._execute(event, combo, use_exec=False)

        if passed:
            id = (event.run(), event.lumi(), event.event())
            if id == self.__last:
                return passed
            self.__last = id

            try:
                self._counts[process] += 1
            except KeyError:
                self._counts[process] = 1
        return passed

    def __getitem__(self, key):
        return self._counts[key]

    def __unicode__(self):
        return self._name

    def __str__(self):
        return unicode(self).encode('utf-8')

    def processes(self):
        return self._counts.keys()

class StaticCut(Cut):
    def __init__(self, name):
        self._name = name
        self._counts = {}

    def __call__(self, process, event, combo):
        return True

    def __getitem__(self, key):
        return self._counts[key]

    def __setitem__(self, key, value):
        self._counts[key] = value

def normalize(cuts, lumi):
    weights = None
    processed = None

    for cut in cuts:
        if str(cut).lower() == "dataset processed":
            processed = cut
        elif str(cut).lower() == "dataset event weights":
            weights = cut
        elif processed and cut:
            break

    dsetnorm = StaticCut("Dataset norm")
    luminorm = StaticCut("Luminosity norm")
    for proc in cuts[-1].processes():
        scale = processed[proc] / float(weights[proc])
        dsetnorm[proc] = cuts[-1][proc] * scale
        luminorm[proc] = cuts[-1][proc] * scale * lumi * proc.cross_section / float(proc.events)
    cuts.append(dsetnorm)
    cuts.append(luminorm)

def cutflow(cuts, procs, f=sys.stdout):
    expanded_proc = [Process.expand(proc) for proc in procs]

    namelength = max(len(unicode(cut)) for cut in cuts)
    fieldlengths = []
    for proc, subprocs in zip(procs, expanded_proc):
        val = sum(cuts[0][p] for p in subprocs)
        length = max(len(proc), len("{:.2f}".format(float(val))))
        fieldlengths.append(length)

    header = u"{{:{0}}}".format(namelength) \
            + u"".join(u"   {{:{0}}}".format(fl) for fl in fieldlengths) \
            + u"\n"
    format = u"{{:{0}}}".format(namelength) \
            + "".join("   {{:{0}.2f}}".format(fl) for fl in fieldlengths) \
            + "\n"

    f.write(header.format("Cut", *procs))
    f.write("-" * namelength + "".join("   " + "-" * fl for fl in fieldlengths) + "\n")
    for cut in cuts:
        f.write(format.format(cut, *[sum(float(cut[p]) for p in ps) for ps in expanded_proc]))