import logging
import os
import pickle
import sys

import ROOT as r
r.gSystem.Load("libttHTauRoast")

from ttH.TauRoast.processing import Process
from ttH.TauRoast.useful import code2cut, print_cuts


class Cutflows(dict):

    def __init__(self, fn=None, *args, **kwargs):
        dict.__init__(self, *args, **kwargs)
        if not fn:
            return

        with open(fn, 'rb') as f:
            for name, cuts in pickle.load(f).items():
                dict.__setitem__(self, name, cuts[:-2])

    def __add__(self, other):
        for name, cuts in other.items():
            if name not in self:
                self[name] = cuts
                continue
            for theirs in cuts:
                for mine in self[name]:
                    try:
                        mine = mine + theirs
                    except ValueError:
                        continue

        return self

    def save(self, config):
        fn = os.path.join(config["outdir"], "cutflow.pkl")
        with open(fn, 'wb') as f:
            pickle.dump(self, f)


class Cut(object):

    def __init__(self, name, code=None):
        self._r = code2cut(name.encode('ascii', 'ignore'), code) if code else None
        self._name = name
        self._code = code

    def __getitem__(self, key):
        return self._r[str(key)]

    def __unicode__(self):
        return self._name

    def __str__(self):
        return unicode(self).encode('utf-8')

    def __getstate__(self):
        counts = dict([(p, self[p]) for p in self.processes()])
        return self._name, counts, self._code

    def __setstate__(self, state):
        name, counts, code = state
        self._name = name
        self._code = code
        if code:
            self._r = code2cut(name.encode('ascii', 'ignore'), code)
        else:
            self._r = r.fastlane.StaticCut(name.encode('ascii', 'ignore'))
        for p, count in counts.items():
            self._r[p] = count

    def __add__(self, other):
        if self._name != str(other):
            raise ValueError('names do not match: {} and {}'.format(self._name, other))
        for p in other.processes():
            self._r[p] += float(other[p])

    def callback(self, fct):
        self._r.setCallback(fct)

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


def normalize(cuts, lumi, limit=None):
    weights = None
    processed = None

    ntuplized = None
    analyzed = None

    for cut in cuts:
        if str(cut).lower() == "dataset processed":
            processed = cut
        elif str(cut).lower() == "dataset event weights":
            weights = cut
        elif str(cut).lower() == "ntuple":
            ntuplized = cut
        elif str(cut).lower() == "ntuple analyzed":
            analyzed = cut
        elif processed and weights and ntuplized and analyzed:
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
            if ntuplized[proc] == 0 or analyzed[proc] == 0:
                logging.warning("0 event count for {}".format(proc))
                fraction = 1.
            elif (not limit) or analyzed[proc] < limit:
                fraction = 1.
            else:
                fraction = analyzed[proc] / float(ntuplized[proc])
                logging.warning("scaling {} by {} to compensate for partially analyzed dataset".format(proc, 1. / fraction))
            dsetnorm[proc] = cuts[-1][proc] * scale / fraction
            luminorm[proc] = cuts[-1][proc] * scale / fraction * lumi * p.cross_section / float(p.events)
    cuts.append(dsetnorm)
    cuts.append(luminorm)


def cutflow(cuts, processes, relative=False, weighed=False, f=sys.stdout):
    expanded_proc = []
    procs = []
    for proc in processes:
        subs = [p for p in Process.expand(proc) if str(p) in cuts[0].processes()]
        if len(subs) > 0:
            expanded_proc.append(subs)
            procs.append(proc)

    cutdata = [[sum(float(cut[p]) for p in ps) for ps in expanded_proc] for cut in cuts]

    if weighed:
        for n, c in enumerate(reversed(cuts)):
            if not isinstance(c, StaticCut):
                break

        ratios = [a / (b if b != 0 else 1) for a, b in zip(cutdata[-1], cutdata[-(n + 1)])]
        cutdata = cutdata[:-n]
        for i in xrange(3, len(cutdata)):
            cutdata[i] = [a * b for a, b in zip(cutdata[i], ratios)]

    if relative:
        for i in xrange(1, len(cutdata)):
            cutdata[-i] = [(float(b) / a if a != 0 else 0)
                           for a, b in zip(cutdata[-(i + 1)], cutdata[-i])]

    print_cuts(cuts, procs, cutdata, expanded_proc, "Cut", f)
