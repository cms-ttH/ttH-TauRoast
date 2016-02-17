import hashlib
import math
import os
import ROOT as r
import time

from collections import namedtuple

Config = namedtuple('Config', ['taus', 'leptons', 'generator'])
config = None

def setup(channel, generator):
    global config
    config = Config(taus=channel.lower().count("t"), leptons=channel.lower().count("l"), generator=generator)

def load_python():
    import imp
    from glob import glob

    from ttH.TauRoast.processing import Process

    datadir = os.path.join(os.environ["LOCALRT"], 'src', 'ttH', 'TauRoast', 'data')
    magic = os.path.join(datadir, 'plots', '*.py')
    for n, file in enumerate(glob(magic)):
        imp.load_source("plots{0}".format(n), file)
    magic = os.path.join(datadir, 'procs', '*.py')
    for n, file in enumerate(glob(magic)):
        imp.load_source("procs{0}".format(n), file)

    global config
    for p in Process.procs():
        if str(p).startswith('ttH') and str(p).endswith(config.generator):
            p.copy(lambda s: s.replace('_' + config.generator, ''))

def R(p4):
    return math.sqrt(p4.Eta()**2 + p4.Phi()**2)

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
        template<typename T> double R(T t) {
            return std::sqrt(t.Eta() * t.Eta() + t.Phi() * t.Phi());
        }
        template<typename T, typename U> double dR(T t, U u) {
            return R(t.p4() - u.p4());
        }

        template<typename T> int len(T t) { return t.size(); }
        template<typename T> bool btag(T j) { return j.csv() > 0.80; }
        template<typename T> int btags(T js) {
            int res = 0;
            for (const auto& j: js)
                res += btag(j);
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
""")

def code2cut(name, code):
    stub = hashlib.sha1(code + "\n{0}".format(time.time())).hexdigest()[:7]
    chunck = """
        bool fct_{f}(const superslim::Event& event,
                const std::vector<superslim::Tau>& taus,
                const std::vector<superslim::Lepton>& leptons,
                const std::vector<superslim::Jet>& jets,
                const superslim::LorentzVector& met) {{ return {c}; }}
        auto cut_{f} = fastlane::Cut("{n}", &fct_{f});
    """.format(n=name, f=stub, c=code)
    if not r.gInterpreter.Declare(chunck):
        raise RuntimeError
    return getattr(r, 'cut_' + stub)

def code2leaf(name, typename, code):
    stub = hashlib.sha1(code).hexdigest()[:7]
    chunck = """
        void fct_{f}(const superslim::Event& event,
                const std::vector<superslim::Tau>& taus,
                const std::vector<superslim::Lepton>& leptons,
                const std::vector<superslim::Jet>& jets,
                const superslim::LorentzVector& met,
                std::unordered_map<std::string, double>& weights,
                {t}& result) {{ {c}; }}
        fastlane::Leaf<{t}> leaf_{f}("{n}", &fct_{f});
    """.format(n=name, f=stub, c=code, t=typename)
    if not r.gInterpreter.Declare(chunck):
        raise RuntimeError
    return getattr(r, 'leaf_' + stub)
