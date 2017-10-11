#!/bin/sh

set -e

max=500000

dasgoclient -query 'file dataset=/ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM' > test/tauid_tth.txt
dasgoclient -query 'file dataset=/TTJets_SingleLeptFromT_TuneCUETP8M2T4_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM' > test/tauid_ttjets.txt

cmsRun test/tauid_ntuplize.py maxEvents=$max outputFile=tth.root inputFiles_load=test/tauid_tth.txt
cmsRun test/tauid_ntuplize.py maxEvents=$max outputFile=ttjets.root inputFiles_load=test/tauid_ttjets.txt

python scripts/plot_tauid {tth,ttjets}.root
