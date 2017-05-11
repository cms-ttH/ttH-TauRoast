set -e

ntuplize=1
trees=1
comparison=1

while getopts nt o; do
   case "$o" in
      c) comparison=0;;
      n) ntuplize=0;;
      t) trees=0;;
      [?]) print >&2 "usage: $0 [-nt]"
           exit 1;;
   esac
done
shift $(($OPTIND-1))

syncdir=$HOME/www/ttH/sync/80X
ttHfile=/store/mc/RunIISummer16MiniAODv2/ttHToNonbb_M125_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/00D10AF2-76BE-E611-8EFB-001E67457DFA.root

if [ $ntuplize -gt 0 ]; then
   mkdir -p test/sync_tau/{ttH,ttH_skim,ttjets}
   rm -f test/sync_tau/{ttH,ttH_skim,ttjets}/*.root

   cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch/$ttHfile takeAll=true outputFile=test/sync_tau/ttH/test.root &> test/sync_tau/ttH.log &
   cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch/$ttHfile outputFile=test/sync_tau/ttH_skim/test.root &> test/sync_tau/ttH_skim.log &
   # cmsRun test/ntuplize.py inputFiles=root://cms-xrd-global.cern.ch//store/mc/RunIIFall15MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext3-v1/00000/00DF0A73-17C2-E511-B086-E41D2D08DE30.root takeAll=true outputFile=test/sync_tau/ttjets/test.root &
   # cmsRun test/ntuplize.py inputFiles=file:0C6DA13E-38C8-E511-8F6E-00259055220A.root takeAll=true outputFile=test/sync_tau/ttH/test.root channels=tll maxEvents=-1 &
   # cmsRun test/ntuplize.py inputFiles=file:00DF0A73-17C2-E511-B086-E41D2D08DE30.root takeAll=true outputFile=test/sync_tau/ttjets/test.root channels=tll maxEvents=-1 &

   wait
fi

if [ $trees -gt 0 ]; then
   scripts/roaster -amf data/sync.yaml
   scripts/merge_trees
fi

if [ $comparison -gt 0 ]; then
   # scripts/cmp_trees -n 100 $syncdir/tau_llr /afs/cern.ch/work/t/tstreble/public/syncNtuple_ttH_Htautau/syncNtuple_ttH_80X_Summer16.root:syncTree:LLR test/sync_tau/out/ntuple.root:syncTree:ND &
   tree=syncTree_1l2tau_SR
   scripts/cmp_trees -n -1 $syncdir/tau_llr_event_signal /afs/cern.ch/work/t/tstreble/public/syncNtuple_ttH_Htautau/syncNtuple_event_ttH_80X_Summer16.root:$tree:LLR test/sync_tau/out/ntuple_mva.root:$tree:ND &
   scripts/cmp_trees -n -1 $syncdir/tau_tallinn_event_signal /afs/cern.ch/user/a/atiko/public/tthtotautau/sync/Summer16/sync_ntuple_split.root:$tree:Tallinn test/sync_tau/out/ntuple_mva.root:$tree:ND &
   tree=syncTree_1l2tau_Fake
   scripts/cmp_trees -n -1 $syncdir/tau_llr_event_fake /afs/cern.ch/work/t/tstreble/public/syncNtuple_ttH_Htautau/syncNtuple_event_ttH_80X_Summer16.root:$tree:LLR test/sync_tau/out/ntuple_mva.root:$tree:ND &
   # scripts/cmp_trees -n -1 $syncdir/tau_tallinn_event_fake /afs/cern.ch/user/a/atiko/public/tthtotautau/sync/Summer16/sync_ntuple_split.root:$tree:Tallinn test/sync_tau/out/ntuple_mva.root:$tree:ND &
   # scripts/cmp_trees -n 100 $syncdir/tau_cornell/  /afs/cern.ch/work/z/ztao/public/ttHTT_syncNtuple/80X/Summer16/syncNtuple.root:syncTree:Cornell test/sync_tau/out/ntuple.root:syncTree:ND &

   wait
fi
