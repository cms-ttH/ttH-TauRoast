# vim: ts=4:sw=4:et:sta:fileencoding=utf-8
import codecs
from copy import deepcopy
from glob import glob
import logging
import math
import os
import yaml
import re
import ROOT as r
import shutil
import sys

r.gROOT.SetBatch()
r.gSystem.Load("libttHTauRoast")

try:
    from ROOT import roast
except:
    sys.stderr.write("Failed to import 'roast'!\n")
    sys.exit(1)

class Loader(yaml.Loader):
    """Allow to nest yaml files inside each other.  Use `!include` to
    reference a file (or list thereof) to be loaded and put in place.
    Shell globs as per python standard library are allowed.

    See http://stackoverflow.com/questions/528281/how-can-i-include-an-yaml-file-inside-another
    for the basic idea."""
    def __init__(self, stream):
        super(Loader, self).__init__(stream)

        self._root = os.path.dirname(stream.name)
        Loader.add_constructor('!include', Loader._include)

    def _include(self, node):
        names = []

        try:
            names.append(self.construct_scalar(node))
        except:
            names += self.construct_sequence(node)

        filenames = []
        for fn in names:
            fns = glob(os.path.join(self._root, fn))
            if len(fns) == 0:
                fns = glob(os.path.join(os.environ["LOCALRT"], "src", "ttH", "TauRoast", "data", fn))
            filenames += fns

        values = {}
        for fn in filenames:
            with open(fn, 'r') as f:
                values.update(yaml.load(f, Loader))
        return values

def extract_info(config, cut, label):
    """Extract information about a jet multiplicity `cut` and convert it into
    a legend-type string, using the `label` as name."""
    cfg = filter(lambda c: c['name'] == cut, config['physics']['cuts'])[0]
    text = ""
    if 'max' not in cfg:
        text += "#geq "
    text += str(cfg['min'])
    if 'max' in cfg and cfg['max'] != cfg['min']:
        text += '-' + str(cfg['max']) + ' ' + label + 's'
    elif cfg['min'] != 1:
        text += ' ' + label + 's'
    else:
        text += ' ' + label
    return text

def train_mva(config, processes, module):
    cfg = config['analysis']['final mva']

    mvadir = config['paths']['mva output'].format(m='final mva')

    if not os.path.exists(mvadir):
        logging.debug("creating directory %s", mvadir)
        os.makedirs(mvadir)

    vars = []
    err = False
    for varcfg in cfg['variables']:
        try:
            vars.append(roast.MVABase.Var(*varcfg))
        except:
            logging.error("undefined variable %s", name)
            err = True
    if err:
        sys.exit(1)

    mva = roast.MVABase(mvadir, vectorize(vars), module.Get(), 1)

    sig = get_process(cfg['signal'], processes)
    bkg = vectorize(
            map(lambda p: get_process(p, processes), cfg['background']),
            'roast::Process*')
    mva.CreateTrainingSample(sig, bkg)

    m = r.std.map('string,string')()
    for k, v in cfg['methods'].items():
        m[k] = v

    mva.TrainMVA(m)

def book_mva(config, processes, module):
    cfg = config['analysis']['final mva']
    mvadir = config['paths']['mva output'].format(m='final mva')

    vars = []
    err = False
    for varcfg in cfg['variables']:
        try:
            vars.append(roast.MVABase.Var(*varcfg))
        except:
            logging.error("undefined variable %s", varcfg[0])
            err = True

    for method, opts in cfg['methods'].items():
        mva = roast.MVABase(mvadir, vectorize(vars), module.Get(), 1)
        if mva.BookMVA(method):
            roast.register_mva('Final' + method, mva);

def print_mva_ranking(config):
    mvadir = config['paths']['mva output'].format(m='final mva')
    logfile = os.path.join(mvadir, 'tmva.log')

    first = True
    prefix = None

    with open(logfile) as f:
        for l in f.readlines():
            if 'Ranking result' in l:
                prefix = l.split()[1]
                if not first:
                    print
                else:
                    first = False
                print '# ' + prefix
            elif prefix:
                if l.split()[1] == prefix:
                    idx = l.index(':') + 2
                    print l[idx:-1]
                else:
                    prefix = None

def split_combined_processes(config, ps):
    if 'split fakes' in config['analysis'] and config['analysis']['split fakes']:
        for (name, cfg) in config['processes'].items():
            if not 'combine' in cfg:
                continue

            for mode in ("real", "fake"):
                newname = name + "_" + mode
                newcfg = deepcopy(cfg)
                newcfg['niceName'] += " (" + mode + ")"
                newcfg['combine'] = map(lambda s: s + '_' + mode, newcfg['combine'])
                config['processes'][newname] = newcfg

def split_processes(config, ps):
    split_fct = {}

    # split signal by decay modes
    for p in get_signals(ps):
        cfg = config['processes'][p.GetShortName()]

        if 'split' not in cfg:
            continue

        for (mode, val) in cfg['split'].items():
            newname = p.GetShortName() + "_" + mode
            logging.info("creating new split process %s", newname)
            split_fct[newname] = [roast.InclusiveSignalSplitter(val)]

            splitp = roast.Process(p)
            splitp.SetShortName(newname)
            splitp.SetNiceName(p.GetNiceName() + " (" + mode + ")")
            ps.append(splitp)

            if p.GetShortName() in config['analysis']['process']:
                config['analysis']['process'].append(newname)
            # if p.GetShortName() in config['analysis']['plot']:
                # config['analysis']['plot'].append(newname)

    if 'split fakes' in config['analysis'] and config['analysis']['split fakes']:
        for p in get_montecarlo(ps):
            name = p.GetShortName()

            for (mode, splitter) in [("real", roast.FakeSplitter(True)),
                    ("fake", roast.FakeSplitter(False))]:
                newname = p.GetShortName() + "_" + mode
                logging.info("creating new split process %s", newname)

                splitp = roast.Process(p)
                splitp.SetShortName(newname)
                splitp.SetNiceName((p.GetNiceName() + " (" + mode + ")").replace(") (", ", "))
                ps.append(splitp)

                if name in split_fct:
                    split_fct[newname] = split_fct[name] + [splitter]
                else:
                    split_fct[newname] = [splitter]

                if name in config['analysis']['process']:
                    config['analysis']['process'].append(newname)

    config['analysis']['split'] = split_fct

def save_limit_histograms(config, ps):
    """Save histograms as defined in `config` and present in `ps` to a ROOT
    file."""
    var = config['analysis']['limit variable']
    (filename, histname) = config['paths']['limit output'].split(':')

    file = r.TFile(filename, 'RECREATE')
    if not file.IsOpen():
        logging.error("can't open file '{0}' for writing!".format(filename))
        raise SystemError

    logging.info("writing limit histograms to '{0}'".format(filename))

    for (name, cfg) in config['processes'].items():
        if 'limit alias' not in cfg:
            continue

        alias = cfg['limit alias']
        process = []

        try:
            process.append((alias, get_process(name, ps)))
        except LookupError:
            pass

        if 'split' in cfg:
            for split in cfg['split'].keys():
                newalias = '_'.join([alias, split])
                newname = '_'.join([name, split])
                try:
                    process.append((newalias, get_process(newname, ps)))
                except LookupError:
                    pass

        for (alias, proc) in process:
            file.WriteObject(proc.GetHistogram(var).GetHisto(), histname.format(p=alias))

def combine_processes(config, ps, collisions=False):
    cfgs = filter(lambda (k, v): 'combine' in v, config['processes'].items())
    for (alias, cfg) in cfgs:
        if collisions != (alias == "Collisions"):
            continue

        to_combine = cfg['combine']
        res = None

        for procname in to_combine:
            try:
                p = get_process(procname, ps)
                if not res:
                    res = roast.Process(p)
                else:
                    res.Add(p)
            except LookupError, e:
                logging.error("could not combine process {0} into {1}".format(procname, alias))

        if res:
            res.SetShortName(alias)
            res.SetNiceName(cfg['niceName'])
            res.SetLabelForLegend(cfg['labelForLegend'])
            res.SetColor(cfg['color'])

            ps.append(res)

get_signals = lambda ps: filter(lambda p: p.IsSignal(), ps)
get_backgrounds = lambda ps: filter(lambda p: p.IsBackground(), ps)
get_montecarlo = lambda ps: filter(lambda p: p.IsMC(), ps)

def get_collisions(ps):
    res = filter(lambda p: p.IsCollisions() and p.GetShortName() == "Collisions", ps)
    if len(res) != 1:
        if len(res) > 1:
            logging.critical("more than one collision process found")
        else:
            logging.critical("no collisions present")
        raise LookupError
    return res[0]

def get_process(name, processes):
    res = filter(lambda p: p.GetShortName() == name, processes)
    if len(res) != 1:
        if len(res) > 1:
            logging.critical("more than one process with name %s", name)
        else:
            logging.critical("no process with name %s found", name)
        raise LookupError
    return res[0]

def normalize_processes(config, processes):
    lumi = config['physics']['lumi']

    try:
        coll = get_collisions(processes)
        ntuple = coll.GetCutFlow().GetCuts()[1].events_passed
        if coll.GetNOEanalyzed() != ntuple:
            logging.debug("Collision: analyzed %f - in ntuple %f", coll.GetNOEanalyzed(), ntuple)
            lumi *= coll.GetNOEanalyzed() / float(ntuple)
        coll.GetCutFlow().RegisterCutFromLast("Lumi norm", 1)
        coll.BuildNormalizedCutFlow()
    except:
        pass

    logging.debug("lumi to normalize to: %f", lumi)

    for p in processes:
        if p.IsCollisions():
            continue

        est_lumi = lumi * p.GetCrossSection() * p.GetBranchingRatio()
        if p.GetNOEinNtuple() == 0:
            scale = 0
        else:
            logging.debug("%s: analyzed %f - in ntuple %f", p.GetShortName(), p.GetNOEanalyzed(), p.GetNOEinNtuple())
            raw_events = p.GetNOEinDS() * p.GetNOEanalyzed() / float(p.GetNOEinNtuple())
            scale = est_lumi / raw_events

        p.ScaleHistograms(scale)
        p.GetCutFlow().RegisterCutFromLast("Lumi norm", scale)
        p.BuildNormalizedCutFlow()

def get_event_count(proc):
    histo = proc.GetHistogram("Events")
    return (histo.GetBinContent(1), histo.GetBinError(1))

def print_yields(config, processes):
    procs = map(lambda n: get_process(n, processes), config['analysis']['plot'])

    out = codecs.getwriter("UTF-8")(sys.stdout)

    out.write("{0:^15}  {1:^24}\n".format("Sample", "Events"))
    out.write("-" * 41 + "\n")

    bkg_sum = 0
    err_sum = 0
    for bkg in get_backgrounds(procs):
        (yld, err) = get_event_count(bkg)
        bkg_sum += yld
        err_sum += err**2

    for sig in get_signals(procs):
        (yld, err) = get_event_count(sig)
        out.write(u"{0:15}  {1:15.2f} ± {2:6.2f}\n".format(sig.GetShortName(), yld, err))
        out.write(u"{0:15}  {1:15.2f}\n".format("s / sqrt{s + b}", yld / math.sqrt(yld + bkg_sum)))
        out.write("-" * 41 + "\n")

    for bkg in get_backgrounds(procs):
        (yld, err) = get_event_count(bkg)
        out.write(u"{0:15}  {1:15.2f} ± {2:6.2f}\n".format(bkg.GetShortName(), yld, err))

    out.write("-" * 41 + "\n")
    out.write(u"{0:15}  {1:15.2f} ± {2:6.2f}\n".format("Total bkg.", bkg_sum, math.sqrt(err_sum)))

    try:
        col = get_collisions(procs)
        (yld, err) = get_event_count(col)
        ratio = yld / bkg_sum
        ratio_err = ratio * math.sqrt(err_sum / bkg_sum**2 + err**2 / yld**2)
        out.write(u"{0:15}  {1:15.2f} ± {2:6.2f}\n".format("Data / MC", ratio, ratio_err))
        out.write("-" * 41 + "\n")
        out.write(u"{0:15}  {1:15.2f} ± {2:6.2f}\n".format(col.GetShortName(), yld, err))
    except:
        pass

def print_cutflow(config, processes, relative=False, normalized=False):
    procs = map(lambda n: get_process(n, processes), config['analysis']['plot'])
    if normalized:
        cuts = map(lambda p: p.GetNormalizedCutFlow().GetCuts(), procs)
    else:
        cuts = map(lambda p: p.GetCutFlow().GetCuts(), procs)

    out = sys.stdout

    out.write(" " * 20)
    for p in procs:
        out.write("{0:^15}".format(p.GetShortName()))
    out.write("\n")

    if relative:
        last_cuts = [None] * len(cuts)
        for i in range(len(cuts[0])):
            out.write("{0:20}".format(cuts[0][i].name))
            for (n, c) in enumerate(cuts):
                logging.debug("%s", c[i].name)

                if last_cuts[n]:
                    val = c[i].events_passed * 100.0 / last_cuts[n]
                else:
                    val = 100.0

                last_cuts[n] = c[i].events_passed
                out.write("{0:15.2f}".format(val))
            out.write("\n")
    else:
        for i in range(len(cuts[0])):
            out.write("{0:20}".format(cuts[0][i].name))
            for c in cuts:
                logging.debug("%s", c[i].name)
                out.write("{0:15}".format(c[i].events_passed))
            out.write("\n")

    bkg_sum = 0.
    for p in procs:
        if p.IsBackground():
            bkg_sum += p.GetCutFlow().GetCuts()[-1].events_passed

    out.write("{0:20}".format("s / sqrt{s + b}"))
    for p in procs:
        if p.IsSignal():
            count = p.GetCutFlow().GetCuts()[-1].events_passed
            sig = count / math.sqrt(count + bkg_sum)
            out.write("{0:15}".format(sig))
        else:
            out.write(" " * 15)
    out.write("\n")

    out.write(" " * 20)
    for p in procs:
        out.write("{0:^15}".format(p.GetShortName()))
    out.write("\n")

def vectorize(items, cls=None):
    """Create a vector of class `cls` and fill it with the object in the
    list `items`."""
    if cls is None:
        cls = type(items[0])
    res = r.std.vector(cls)()
    for i in items:
        res.push_back(i)
    return res

def save(items, name, file):
    """Save a list of objects to a `file` with the key `name`."""
    dirname = os.path.dirname(file)
    if not os.path.isdir(dirname):
        logging.debug("creating directory %s", dirname)
        os.makedirs(dirname)
    f = r.TFile(file, "RECREATE")
    f.WriteObject(vectorize(items, "roast::Process*"), name)
    f.Close()

def deep_replace(obj, s, r):
    """Recursively performs replacements on strings, substituting `r` for `s`.
    This works only for objects composed of lists, tuples, dicts, and simple
    data types.

    >>> deep_replace({'foo': ['bar', 'ham', 'spam', {'org': 1}]}, 'o', 'a')
    {'faa': ['bar', 'ham', 'spam', {'arg': 1}]}

    This method tries to preserve the type of the replacement, if possible:

    >>> deep_replace('foo', 'foo', 10)
    10
    """
    if isinstance(obj, list) or isinstance(obj, tuple):
        obj = map(lambda o: deep_replace(o, s, r), obj)
    elif isinstance(obj, dict):
        obj = dict(map(lambda o: deep_replace(o, s, r), obj.items()))
    elif isinstance(obj, str):
        if obj == s:
            obj = str(r)
        else:
            obj = obj.replace(s, str(r))
    else:
        obj = deepcopy(obj)
    return obj

def expand_histogram_config(config, channel=None):
    """Looks for a key 'replace'  with a nested list of replacements and
    applies them to the contents.

    >>> expand_histogram_config({r'$0_Label': {'replace': [['T', 't'], ['L', 'l']], 'axis labels': ['$1 this', 'Num']}})
    {'L_Label': {'axis labels': ['l this', 'Num']}, 'T_Label': {'axis labels': ['t this', 'Num']}}
    """
    res = {}
    for (key, cfg) in config.items():
        if not 'replace' in cfg:
            res[key] = cfg
            continue

        for replacements in cfg['replace']:
            (newkey, newcfg) = (key, cfg)
            for (n, repl) in enumerate(replacements):
                (newkey, newcfg) = deep_replace((newkey, newcfg), '${0}'.format(n), repl)
            del newcfg['replace']

            if 'channels' in newcfg and channel not in newcfg['channels']:
                continue
            res[newkey] = newcfg
    return res

def load_config(filename, basedir, overrides):
    with open(filename, 'r') as f:
        config = yaml.load(f, Loader)

    if overrides:
        for setting in overrides:
            try:
                (path, value) = setting.split("=", 1)
                obj = config
                keys = path.split('.')
                while len(keys) > 1 and keys[0] in obj:
                    obj = obj[keys.pop(0)]
                if len(keys) != 1:
                    raise KeyError
                obj[keys[0]] = yaml.load(value)
            except ValueError:
                logging.error("cannot override path %s without a value", setting)
            except KeyError:
                logging.error("cannot override path %s", path)

    channel = config['analysis']['channel']

    histcfg = expand_histogram_config(config['histograms'], channel)
    for (name, cfg) in histcfg.items():
        if 'channels' in cfg and channel not in cfg['channels']:
            del histcfg[name]
    config['histograms'] = histcfg

    config['processes']['Collisions'] = config['processes']['Collisions'][channel]

    if 'root' in config['paths']:
        basedir = config['paths']['root']
        for (k, v) in config['paths'].items():
            if k == 'root':
                continue
            config['paths'][k] = v.replace('{root}', basedir)
    return config

def backup_config(configfile, config, outdir):
    if not os.path.exists(outdir):
        os.makedirs(outdir)
    if os.path.normpath(os.path.join(outdir, 'config_original.yaml')) != \
            os.path.normpath(os.path.join(os.environ['PWD'], configfile)):
        shutil.copy(configfile, os.path.join(outdir, 'config_original.yaml'))
    with open(os.path.join(outdir, 'config_expanded.yaml'), 'w') as f:
        f.write(yaml.dump(config))

def load(name, file):
    """Load an object with key `name` from `file`, which can either be a
    filename or ROOT file object."""
    if isinstance(file, r.TFile):
        f = file
    else:
        f = r.TFile(file)
    obj = r.std.vector("roast::Process*")()
    f.GetObject(name, obj)
    f.Close()
    items = []
    for o in obj:
        items.append(o)
    return items
