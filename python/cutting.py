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
        self.__procs = set()
        self.__last = None

    def __call__(self, process, event, combo, systematics='NA'):
        passed = self._execute(event, combo, systematics=systematics, use_exec=False)

        if passed:
            id = (event.run(), event.lumi(), event.event())
            if id == self.__last:
                return passed
            self.__last = id

            try:
                self[process] += 1
            except KeyError:
                self[process] = 1
        return passed

    def __getitem__(self, key):
        try:
            return self._counts[str(key)]
        except KeyError:
            return 0

    def __setitem__(self, key, value):
        self.__procs.add(key)
        self._counts[str(key)] = value

    def __unicode__(self):
        return self._name

    def __str__(self):
        return unicode(self).encode('utf-8')

    def processes(self):
        return self.__procs

class StaticCut(Cut):
    def __init__(self, name):
        super(StaticCut, self).__init__(name)

    def __call__(self, process, event, combo):
        return True

    def __getitem__(self, key):
        return self._counts[str(key)]

def normalize(cuts, lumi):
    weights = None
    processed = None

    for cut in cuts:
        if str(cut).lower() == "dataset processed":
            processed = cut
        elif str(cut).lower() == "dataset event weights":
            weights = cut
        elif processed and weights:
            break

    dsetnorm = StaticCut("Dataset norm")
    luminorm = StaticCut("Luminosity norm")
    for proc in cuts[-1].processes():
        scale = processed[proc] / float(weights[proc])
        dsetnorm[proc] = cuts[-1][proc] * scale
        luminorm[proc] = cuts[-1][proc] * scale * lumi * proc.cross_section / float(proc.events)
    cuts.append(dsetnorm)
    cuts.append(luminorm)

def cutflow(cuts, procs, relative=False, f=sys.stdout):
    expanded_proc = [Process.expand(proc) for proc in procs]

    cutdata = [[sum(float(cut[p]) for p in ps) for ps in expanded_proc] for cut in cuts]

    if relative:
        for i in xrange(1, len(cutdata)):
            cutdata[-i] = [float(b) / a for a, b in zip(cutdata[-(i + 1)], cutdata[-i])]

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
    for cut, data in zip(cuts, cutdata):
        f.write(format.format(cut, *data))
