set -e

mkdir -p test/sync_tau/{ttH,ttjets}

# cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch//store/mc/RunIIFall15MiniAODv2/ttHJetToTT_M125_13TeV_amcatnloFXFX_madspin_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v3/60000/0C6DA13E-38C8-E511-8F6E-00259055220A.root takeAll=true outputFile=test/sync_tau/ttH/test.root &
# cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch//store/mc/RunIIFall15MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext3-v1/00000/00DF0A73-17C2-E511-B086-E41D2D08DE30.root takeAll=true outputFile=test/sync_tau/ttjets/test.root &
cmsRun test/ntuplize.py inputFiles=file:0C6DA13E-38C8-E511-8F6E-00259055220A.root takeAll=true outputFile=test/sync_tau/ttH/test.root channels=tll &
cmsRun test/ntuplize.py inputFiles=file:00DF0A73-17C2-E511-B086-E41D2D08DE30.root takeAll=true outputFile=test/sync_tau/ttjets/test.root channels=tll &

wait

scripts/roaster -avv data/sync.yaml
scripts/cmp_trees ~/www/ttH/sync/mutlilepton ttHtausNtuple.root:ttHtaus/eventTree:TLN test/sync_tau/out/ntuple.root:ttH:ND
