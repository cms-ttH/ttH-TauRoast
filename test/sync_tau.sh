set -e

mkdir -p test/sync_tau/{ttH,ttjets}
rm -f test/sync_tau/{ttH,ttjets}/*.root

syncdir=$HOME/www/ttH/sync/80X
ttHfile=/store/mc/RunIISpring16MiniAODv2/ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/10000/00CD6E0F-003B-E611-9817-002590DE6E8A.root

cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch/$ttHfile takeAll=true outputFile=test/sync_tau/ttH/test.root reHLT=true &
# cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch//store/mc/RunIIFall15MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext3-v1/00000/00DF0A73-17C2-E511-B086-E41D2D08DE30.root takeAll=true outputFile=test/sync_tau/ttjets/test.root &
# cmsRun test/ntuplize.py inputFiles=file:0C6DA13E-38C8-E511-8F6E-00259055220A.root takeAll=true outputFile=test/sync_tau/ttH/test.root channels=tll maxEvents=-1 &
# cmsRun test/ntuplize.py inputFiles=file:00DF0A73-17C2-E511-B086-E41D2D08DE30.root takeAll=true outputFile=test/sync_tau/ttjets/test.root channels=tll maxEvents=-1 &

wait

scripts/roaster -avv data/sync.yaml
scripts/cmp_trees $syncdir/tau_llr $syncdir/syncNtuple_event_ttH_80X.root:syncTree:LLR test/sync_tau/out/ntuple.root:ttH:ND
# scripts/cmp_trees $syncdir/tau_cornell/ $syncdir/syncNtuple_event.root:syncTree:Cornell test/sync_tau/out/ntuple.root:ttH:ND
