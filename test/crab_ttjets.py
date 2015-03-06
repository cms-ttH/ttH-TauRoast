from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")

config.section_("JobType")
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'ntuplize.py'

config.section_("Data")
config.Data.inputDataset = '/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 100000

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
