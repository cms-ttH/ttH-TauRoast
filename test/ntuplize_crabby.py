import os
import random

from WMCore.Configuration import Configuration
from ttH.TauRoast.datasets import datasets, mctag

version = "v0025"
# tag = "crabby"
tag = "all"

globaltag_mc = "80X_mcRun2_asymptotic_2016_miniAODv2_v1"
globaltag_data = "80X_dataRun2_Prompt_ICHEP16JEC_v0"

lumimask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'


def configs():
    dsets = list(set(datasets(tag)))
    for path in random.sample(dsets, len(dsets)):
        _, major, minor, _ = path.split('/')
        minor = mctag(minor)
        label = (major + '_' + minor).replace('-', '_')
        mask = None
        params = ['globalTag=' + globaltag_mc]

        if '/Run2016' in path and path.endswith('MINIAOD'):
            mask = lumimask
            params = ['data=True', 'globalTag=' + globaltag_data]

        config = Configuration()

        config.section_("General")
        config.General.requestName = version + "_" + label
        config.General.workArea = os.path.join(os.path.dirname(__file__), 'crabby_tau_' + version)
        # config.General.instance = 'jibbers-crabst.cern.ch'
        config.General.instance = 'vocms035.cern.ch'
        config.General.transferLogs = True
        config.General.transferOutputs = True

        config.section_("JobType")
        config.JobType.pluginName = 'Analysis'
        config.JobType.psetName = os.path.join(os.path.dirname(__file__), 'ntuplize.py')
        config.JobType.pyCfgParams = params

        config.section_("Data")
        config.Data.inputDataset = path
        config.Data.publication = True
        config.Data.splitting = 'Automatic'
        hours = 60 ** 2
        config.Data.unitsPerJob = 8 * hours
        if mask:
            config.Data.lumiMask = mask

        config.section_("Site")
        config.Site.storageSite = 'T2_CH_CERN'

        yield config
