import hashlib
import math
import os
import ROOT as r
import sys
import time

from collections import namedtuple

Config = namedtuple('Config', ['channel', 'taus', 'leptons'])
config = None


def setup_processes(config):
    """
    Add two processes: `collisions_fake` and `background_total`, both of
    which are the sum of all background processes.
    """
    from ttH.TauRoast.processing import CombinedProcess
    procs = [p for p in config['plot'] if (not p.startswith('ttH')) and
                                          (not p.startswith('collisions'))]
    CombinedProcess(
        name="collisions_fake",
        fullname="Collisions",
        limitname="data_obs",
        subprocesses=procs
    )
    CombinedProcess(
        name="background_total",
        fullname="Total Background",
        limitname="bkgsum",
        subprocesses=procs
    )


def setup(cfg):
    global config

    channel = cfg['channel']
    config = Config(
        channel=channel,
        taus=channel.lower().count("t"),
        leptons=channel.lower().count("l")
    )

    setup_processes(cfg)


def load_python(mode):
    import imp
    from glob import glob

    datadir = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data')
    if mode:
        magic = os.path.join(datadir, mode, '*.py')
        for n, fn in enumerate(glob(magic)):
            imp.load_source("{0}{1}".format(mode, n), fn)
    else:
        magic = os.path.join(datadir, 'plots', '*.py')
        for n, file in enumerate(glob(magic)):
            imp.load_source("plots{0}".format(n), file)
        magic = os.path.join(datadir, 'procs', '*.py')
        for n, file in enumerate(glob(magic)):
            imp.load_source("procs{0}".format(n), file)


def vectorize(things, cls):
    res = r.std.vector(cls)()
    for thing in things:
        if hasattr(thing, 'raw'):
            res.push_back(thing.raw())
        else:
            res.push_back(thing)
    return res


def R(p4):
    return math.sqrt(p4.Eta() ** 2 + p4.Phi() ** 2)


def dR(one, two):
    return R(one.p4() - two.p4())


def btag(jet):
    return jet.csv() > 0.80

__mva = None


def mva():
    global __mva
    if __mva is None:
        from variation import principal
        if principal is None:
            return -666.
        __mva = principal
    return __mva.evaluate()

r.gInterpreter.Declare("""
        #include "DataFormats/Math/interface/deltaR.h"
        template<typename T, typename U> double dR(T t, U u) {
            return reco::deltaR(t, u);
        }

        template<typename T> int len(T t) { return t.size(); }
        template<typename T> bool btag(T j, bool loose=false) { return j.csv() > (loose ? 0.46 : 0.80); }
        template<typename T> int btags(T js, bool loose=false) {
            int res = 0;
            for (const auto& j: js)
                res += btag(j, loose);
            return res;
        }
        template<typename T> T tags(T js) {
            T res;
            for (const auto& j: js)
                if (btag(j))
                    res.push_back(j);
            return res;
        }
        template<typename T> T notags(T js) {
            T res;
            for (const auto& j: js)
                if (!btag(j))
                    res.push_back(j);
            return res;
        }
        template<typename S, typename T> float metLD(float met, S js, T ls) {
            superslim::LorentzVector mht;
            for (const auto& j: js)
                if (j.pt() >= 25.)
                    mht += j.p4();
            for (const auto& l: ls)
                mht += l.p4();
            return 0.00397 * met + 0.00265 * mht.pt();
        }
""")


def code2cut(name, code):
    stub = hashlib.sha1(code + "\n{0}".format(time.time())).hexdigest()[:7]
    chunck = """
        bool fct_{f}(const superslim::Event& event,
                const std::vector<superslim::Tau>& taus,
                const std::vector<superslim::Tau>& all_taus,
                superslim::Tau::id tau_id,
                const std::vector<superslim::Lepton>& leptons,
                const std::vector<superslim::Lepton>& all_leptons,
                superslim::Lepton::id lepton_id,
                const std::vector<superslim::Jet>& jets,
                const superslim::LorentzVector& met) {{ return {c}; }}
        auto cut_{f} = fastlane::Cut("{n}", &fct_{f});
    """.format(n=name, f=stub, c=code)
    if not r.gInterpreter.Declare(chunck):
        raise RuntimeError("Can't compile:\n" + chunck)
    return getattr(r, 'cut_' + stub)


def code2leaf(name, typename, code):
    stub = hashlib.sha1(name + code).hexdigest()[:7]
    chunck = """
        #include "ttH/TauRoast/interface/Fastlane.h"

        void fct_{f}(const superslim::Event& event,
                const std::vector<superslim::Tau>& taus,
                const std::vector<superslim::Tau>& all_taus,
                const std::vector<superslim::Lepton>& leptons,
                const std::vector<superslim::Lepton>& electrons,
                const std::vector<superslim::Lepton>& muons,
                const std::vector<superslim::Lepton>& all_leptons,
                const std::vector<superslim::Jet>& jets,
                const superslim::LorentzVector& met,
                std::unordered_map<std::string, double>& weights,
                {t}& result) {{
            {c};
        }}
        fastlane::Leaf<{t}> leaf_{f}("{n}", &fct_{f});
    """.format(n=name, f=stub, c=code, t=typename)
    if not r.gInterpreter.Declare(chunck):
        raise RuntimeError("Can't compile\n" + chunck)
    return getattr(r, 'leaf_' + stub)


def print_cuts(cuts, columns, cutdata=None, sum_columns=None, what="", f=sys.stdout):
    if not sum_columns:
        sum_columns = [[c] for c in columns]

    if not cutdata:
        cutdata = [[sum(float(cut[c]) for c in cs) for cs in sum_columns] for cut in cuts]

    namelength = max(len(unicode(cut)) for cut in cuts)
    fieldlengths = []
    for col, subcols in zip(columns, sum_columns):
        val = max(sum(cut[c] for c in subcols) for cut in cuts)
        length = max(len(col), len("{:,.2f}".format(float(val))))
        fieldlengths.append(length)

    header = u"{{:{0}}}".format(namelength) \
        + u"".join(u"   {{:{0}}}".format(fl) for fl in fieldlengths) \
        + u"\n"
    body = u"{{:{0}}}".format(namelength) \
        + "".join("   {{:{0},.2f}}".format(fl) for fl in fieldlengths) \
        + "\n"

    f.write(header.format(what, *columns))
    f.write("-" * namelength + "".join("   " + "-" * fl for fl in fieldlengths) + "\n")
    for cut, data in zip(cuts, cutdata):
        f.write(body.format(cut, *data))
