set -e

MAXEVENTS=40000

mkdir -p test/genfilter/ttjets{,_filtered}
rm -f test/genfilter/ttjets{,_filtered}/*.root

(
   set -e
   cmsRun test/genfilter.py inputFiles_load=test/genfilter.files outputFile=test/genfilter.root maxEvents=$MAXEVENTS
   cmsRun test/ntuplize.py inputFiles=file:test/genfilter_numEvent${MAXEVENTS}.root outputFile=test/genfilter/ttjets_filtered/test.root channels=ttl maxEvents=$MAXEVENTS
) &
cmsRun test/ntuplize.py inputFiles_load=test/genfilter.files outputFile=test/genfilter/ttjets/test.root channels=ttl maxEvents=$MAXEVENTS &

wait
