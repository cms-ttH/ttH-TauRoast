set -e

mkdir -p test/sync_multilepton/ttH
rm -f test/sync_multilepton/ttH/*.root

cmsRun test/ntuplize.py inputFiles=/store/mc/RunIISpring16MiniAODv1/ttHToNonbb_M125_13TeV_powheg_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/50000/0ADF7BAE-0914-E611-B788-0025905A6068.root takeAll=true outputFile=test/sync_multilepton/ttH/test.root channels=tll maxEvents=-1 &

wait

scripts/roaster -avv data/sync_multilepton.yaml

mkdir -p "test/sync_multilepton/sync/me"
python "test/sync_multilepton.py"
# scripts/cmp_trees ~/www/ttH/sync/tau_llr syncNtuple_ttH.root:syncTree:LLR test/sync_tau/out/ntuple.root:ttH:ND
# scripts/cmp_trees ~/www/ttH/sync/tau_tallinn/ syncNtuple_ttH.root:syncTree:Tallinn test/sync_tau/out/ntuple.root:ttH:ND
