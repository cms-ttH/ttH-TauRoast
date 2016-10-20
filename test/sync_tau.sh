set -e

ntuplize=1
trees=1

while getopts nt o; do
   case "$o" in
      n) ntuplize=0;;
      t) trees=0;;
      [?]) print >&2 "usage: $0 [-nt]"
           exit 1;;
   esac
done
shift $(($OPTIND-1))

syncdir=$HOME/www/ttH/sync/80X
ttHfile=/store/mc/RunIISpring16MiniAODv2/ttHJetToNonbb_M125_13TeV_amcatnloFXFX_madspin_pythia8_mWCutfix/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/10000/00CD6E0F-003B-E611-9817-002590DE6E8A.root

if [ $ntuplize -gt 0 ]; then
   mkdir -p test/sync_tau/{ttH,ttH_skim,ttjets}
   rm -f test/sync_tau/{ttH,ttH_skim,ttjets}/*.root

   cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch/$ttHfile takeAll=true outputFile=test/sync_tau/ttH/test.root reHLT=true &> test/sync_tau/ttH.log &
   cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch/$ttHfile outputFile=test/sync_tau/ttH_skim/test.root reHLT=true &> test/sync_tau/ttH_skim.log &
   # cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch//store/mc/RunIIFall15MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext3-v1/00000/00DF0A73-17C2-E511-B086-E41D2D08DE30.root takeAll=true outputFile=test/sync_tau/ttjets/test.root &
   # cmsRun test/ntuplize.py inputFiles=file:0C6DA13E-38C8-E511-8F6E-00259055220A.root takeAll=true outputFile=test/sync_tau/ttH/test.root channels=tll maxEvents=-1 &
   # cmsRun test/ntuplize.py inputFiles=file:00DF0A73-17C2-E511-B086-E41D2D08DE30.root takeAll=true outputFile=test/sync_tau/ttjets/test.root channels=tll maxEvents=-1 &

   wait
fi

if [ $trees -gt 0 ]; then
   scripts/roaster -af data/sync.yaml
fi

scripts/cmp_trees -n 100 $syncdir/tau_llr /afs/cern.ch/work/t/tstreble/public/syncNtuple_ttH_Htautau/syncNtuple_ttH_80X.root:syncTree:LLR test/sync_tau/out/ntuple.root:syncTree:ND
scripts/cmp_trees -n -1 $syncdir/tau_llr_event /afs/cern.ch/work/t/tstreble/public/syncNtuple_ttH_Htautau/syncNtuple_event_ttH_80X.root:syncTree_1l2tau:LLR test/sync_tau/out/ntuple.root:syncTree_1l2tau:ND
# scripts/cmp_trees $syncdir/tau_cornell/ $syncdir/syncNtuple_event.root:syncTree:Cornell test/sync_tau/out/ntuple.root:ttH:ND
