import os
import re
import yaml

from WMCore.Configuration import Configuration
from ttH.TauRoast.datasets import datasets

version = "v22"

def configs():
    for label, path in datasets:
        mask = None
        params = []
        if '2015' in label:
            mask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt'
            params = ['data=True', 'globalTag=76X_dataRun2_v12']

        config = Configuration()

        config.section_("General")
        config.General.requestName = label
        config.General.workArea = os.path.join(os.path.dirname(__file__), 'crabby_tau_' + version)

        config.section_("JobType")
        config.JobType.pluginName  = 'Analysis'
        config.JobType.psetName    = os.path.join(os.path.dirname(__file__), 'ntuplize.py')
        config.JobType.pyCfgParams = params

        config.section_("Data")
        config.Data.inputDataset = path
        config.Data.splitting = 'EventAwareLumiBased'
        config.Data.unitsPerJob = 1000000
        if mask:
            config.Data.lumiMask = mask

        config.section_("Site")
        config.Site.storageSite = 'T2_CH_CERN'

        yield config
