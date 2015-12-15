#!/bin/sh

files="
   root://cms-xrd-global.cern.ch///store/user/hmildner/el_skim_3loosejets.root
   root://cms-xrd-global.cern.ch///store/user/hmildner/mu_skim_3loosejets.root
   root://cms-xrd-global.cern.ch///store/mc/RunIISpring15MiniAODv2/ttHTobb_M125_13TeV_powheg_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/DA1B6FD6-C46D-E511-9C7B-00A0D1EE29B8.root
   root://cms-xrd-global.cern.ch///store/mc/RunIISpring15MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/0EE7E064-BE6F-E511-BB41-E4115BB4C4BC.root
"

for f in $files; do
   if [ -e "$(basename $f)" ]; then
      echo "Not copying $f"
   else
      xrdcp $f .
   fi
done

cmsRun test/ntuplize_lj_sync.py \
  outputFile=test/leptonplusjets/ttH2bb_125_pow/test.root \
  inputFiles=file:DA1B6FD6-C46D-E511-9C7B-00A0D1EE29B8.root &
cmsRun test/ntuplize_lj_sync.py \
  outputFile=test/leptonplusjets/tt_pow/test.root \
  inputFiles=file:0EE7E064-BE6F-E511-BB41-E4115BB4C4BC.root &
cmsRun test/ntuplize_lj_sync.py \
  data=true globalTag=74X_dataRun2_v5 \
  outputFile=test/leptonplusjets/data_SingleMu_2015D_Oct05/test.root \
  inputFiles=file:mu_skim_3loosejets.root &
cmsRun test/ntuplize_lj_sync.py \
  data=true globalTag=74X_dataRun2_v5 \
  outputFile=test/leptonplusjets/data_SingleE_2015D_Oct05/test.root \
  inputFiles=file:el_skim_3loosejets.root &
