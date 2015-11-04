import os
import re
import yaml

from WMCore.Configuration import Configuration

def apply_matching(config):
    if 'task defaults' not in config:
        return config
    defaults = config['task defaults']
    matching = defaults.get('matching', [])
    configs = []

    for cfg in config['tasks']:
        label = cfg['label']

        for match in matching:
            if re.search(match['label'], label):
                for k, v in match.items():
                    if k == 'label':
                        continue
                    if k not in cfg:
                        cfg[k] = v
        for k, v in defaults.items():
            if k == 'matching':
                continue
            if k not in cfg:
                cfg[k] = v

        configs.append(cfg)

    config['tasks'] = configs
    del config['task defaults']

    return config

def dict2config(hash):
    config = Configuration()

    config.section_("General")
    config.General.requestName = hash['label']
    config.General.workArea = os.path.join(os.path.dirname(__file__), 'crabby_tau_v10')

    config.section_("JobType")
    config.JobType.pluginName  = 'Analysis'
    config.JobType.psetName    = os.path.join(os.path.dirname(__file__), hash['cmssw config'])
    if 'parameters' in hash:
        config.JobType.pyCfgParams = hash['parameters']

    config.section_("Data")
    config.Data.inputDataset = hash['dataset']
    config.Data.splitting = 'EventAwareLumiBased'
    config.Data.unitsPerJob = 1000000
    if 'lumi mask' in hash:
        config.Data.lumiMask = hash['lumi mask']

    config.section_("Site")
    config.Site.storageSite = 'T2_CH_CERN'

    return config

def configs():
    with open(os.path.join(os.path.dirname(__file__), 'lobster.yaml')) as f:
        yml = yaml.load(f)

    yml = apply_matching(yml)
    for cfg in yml['tasks']:
        yield dict2config(cfg)
