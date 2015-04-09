from WMCore.Configuration import Configuration

config = Configuration()

config.section_("General")

config.section_("JobType")
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'validate.py'

config.section_("Data")
config.Data.inputDataset = '/ttHJetToTT_M125_13TeV_amcatnloFXFX_madspin_pythia8/RunIIWinter15GS-MCRUN2_71_V1-v1/GEN-SIM'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 100000

config.section_("Site")
config.Site.storageSite = 'T3_US_NotreDame'
