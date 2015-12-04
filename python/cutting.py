import sys

import ROOT as r
r.gSystem.Load("libttHTauRoast")

from ttH.TauRoast.processing import Process
from ttH.TauRoast.useful import code2cut

class Cut(object):
    def __init__(self, name, code=None):
        self._r = code2cut(name.encode('ascii', 'ignore'), code) if code else None
        self._name = name

    def __getitem__(self, key):
        return self._r[str(key)]

    def __unicode__(self):
        return self._name

    def __str__(self):
        return unicode(self).encode('utf-8')

    def __getstate__(self):
        counts = dict([(p, self[p]) for p in self.processes()])
        return self._name, counts

    def __setstate__(self, state):
        name, counts = state
        self._name = name
        self._r = r.fastlane.StaticCut(name.encode('ascii', 'ignore'))
        for p, count in counts.items():
            self._r[p] = count

    def raw(self):
        return self._r

    def processes(self):
        return self._r.processes()

class StaticCut(Cut):
    def __init__(self, name):
        super(StaticCut, self).__init__(name)
        self._r = r.fastlane.StaticCut(name)

    def __setitem__(self, key, value):
        self._r[str(key)] = value

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
        if str(proc).startswith("collisions"):
            dsetnorm[proc] = cuts[-1][proc]
            luminorm[proc] = cuts[-1][proc]
        else:
            p = Process.get(proc)
            scale = processed[proc] / float(weights[proc])
            dsetnorm[proc] = cuts[-1][proc] * scale
            luminorm[proc] = cuts[-1][proc] * scale * lumi * p.cross_section / float(p.events)
    cuts.append(dsetnorm)
    cuts.append(luminorm)

def cutflow(cuts, procs, relative=False, f=sys.stdout):
    expanded_proc = [Process.expand(proc) for proc in procs]

    cutdata = [[sum(float(cut[p]) for p in ps) for ps in expanded_proc] for cut in cuts]

    if relative:
        for i in xrange(1, len(cutdata)):
            cutdata[-i] = [(float(b) / a if a != 0 else 0) for a, b in zip(cutdata[-(i + 1)], cutdata[-i])]

    namelength = max(len(unicode(cut)) for cut in cuts)
    fieldlengths = []
    for proc, subprocs in zip(procs, expanded_proc):
        val = max(sum(cut[p] for p in subprocs) for cut in cuts)
        length = max(len(proc), len("{:.2f}".format(float(val))))
        fieldlengths.append(length)

    header = u"{{:{0}}}".format(namelength) \
            + u"".join(u"   {{:{0}}}".format(fl) for fl in fieldlengths) \
            + u"\n"
    body = u"{{:{0}}}".format(namelength) \
            + "".join("   {{:{0}.2f}}".format(fl) for fl in fieldlengths) \
            + "\n"

    f.write(header.format("Cut", *procs))
    f.write("-" * namelength + "".join("   " + "-" * fl for fl in fieldlengths) + "\n")
    for cut, data in zip(cuts, cutdata):
        f.write(body.format(cut, *data))
