set -e

MAXEVENTS=400000

mkdir -p test/genfaketau/ttjets
rm -f test/genfaketau/ttjets/*.root

cmsRun test/ntuplize.py inputFiles_load=test/genfilter.files outputFile=test/genfaketau/ttjets/test.root channels=ttl maxEvents=$MAXEVENTS takeAll=true saveGenInfo=true
