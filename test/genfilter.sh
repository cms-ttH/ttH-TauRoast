set -e

MAXEVENTS=40000

mkdir -p test/genfilter/ttjets{,_filtered}
rm -f test/genfilter/ttjets{,_filtered}/*.root

cmsRun test/ntuplize.py inputFiles_load=test/genfilter.files outputFile=test/genfilter/ttjets_filtered/test.root channels=ttl maxEvents=$MAXEVENTS genFilter=true genFilterWithFakes=true &
cmsRun test/ntuplize.py inputFiles_load=test/genfilter.files outputFile=test/genfilter/ttjets/test.root channels=ttl maxEvents=$MAXEVENTS &

wait
