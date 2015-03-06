from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")

config.section_("JobType")
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'ntuplize.py'

config.section_("Data")
config.Data.inputDataset = '/TTbarH_M-125_13TeV_amcatnlo-pythia8-tauola/Phys14DR-PU20bx25_tsg_PHYS14_25_V1-v2/MINIAODSIM'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 100000

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
