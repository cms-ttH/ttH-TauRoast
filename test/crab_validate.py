from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")

config.section_("JobType")
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'validate.py'

config.section_("Data")
config.Data.inputDataset = '/ttHJetToTT_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2_ext3-v1/MINIAODSIM'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 100000
config.Data.ignoreLocality = True

config.section_("Site")
config.Site.storageSite = 'T3_US_NotreDame'
