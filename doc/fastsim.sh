scram p CMSSW CMSSW_7_1_25
cd CMSSW_7_1_25/src
curl -s --insecure https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_fragment/HIG-RunIISummer15wmLHEGS-00482 --retry 2 --create-dirs -o Configuration/GenProduction/python/HIG-RunIISummer15wmLHEGS-00482-fragment.py
eval `scram runtime -sh`
scram b
cd ../..
cmsDriver.py Configuration/GenProduction/python/HIG-RunIISummer15wmLHEGS-00482-fragment.py \
   --fileout file:HIG-RunIISummer15wmLHEGS-00482_fast.root \
   --mc --eventcontent RAWSIM,LHE \
   --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring \
   --datatier GEN-SIM,LHE --conditions MCRUN2_71_V1::All --beamspot Realistic50ns13TeVCollision \
   --step LHE,GEN,SIM --magField 38T_PostLS1 \
   --fast \
   --python_filename HIG-RunIISummer15wmLHEGS-00482_1_cfg_fast.py --no_exec -n 50

scram p CMSSW CMSSW_8_0_20
cd CMSSW_8_0_20/src
curl -s --insecure https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_fragment/HIG-RunIISummer15wmLHEGS-00482 --retry 2 --create-dirs -o Configuration/GenProduction/python/HIG-RunIISummer15wmLHEGS-00482-fragment.py
eval `scram runtime -sh`
scram b
cd ../..

# =============
# PU Production
# =============

cmsDriver.py MinBias_13TeV_pythia8_TuneCUETP8M1_cfi \
   --conditions auto:run2_mc \
   --fast \
   -n 10 \
   --era Run2_2016 \
   --eventcontent FASTPU \ --relval 100000,1000 \
   -s GEN,SIM,RECOBEFMIX \ --datatier GEN-SIM-RECO \ --beamspot Realistic50ns13TeVCollision \
   --fileout file:pu_fast.root \
   --python_filename pu_fast.py --no_exec

cmsDriver.py SingleNuE10_cfi \
   --fileout file:premix_fast.root \
   --pileup_input file:pu_fast.root \
   --pileup AVE_35_BX_25ns \
   --mc --eventcontent PREMIX --datatier GEN-SIM-DIGI-RAW --conditions auto:run2_mc \
   --step GEN,SIM,RECOBEFMIX,DIGIPREMIX,L1,DIGI2RAW --era Run2_2016 \
   --python_filename premix_fast.py --no_exec \
   --fast

# ================
# Using private PU
# ================

cmsDriver.py Configuration/GenProduction/python/HIG-RunIISummer15wmLHEGS-00482-fragment.py \
   --python_filename all_fast.py \
   --fileout file:all_fast.root \
   --pileup_input file:premix_fast.root \
   --mc \
   --eventcontent AODSIM \
   --fast \
   --customise SimGeneral/DataMixingModule/customiseForPremixingInput.customiseForPreMixingInput \
   --datatier AODSIM \
   --conditions auto:run2_mc \
   --beamspot Realistic50ns13TeVCollision \
   --step LHE,GEN,SIM,RECOBEFMIX,DIGIPREMIX_S2,DATAMIX,L1,DIGI2RAW,L1Reco,RECO,HLT:@fake1 \
   --datamix PreMix \
   --era Run2_25ns \
   --no_exec \

# cmsDriver.py step1 \
#    --filein "file:HIG-RunIISummer15wmLHEGS-00482_fast.root" \
#    --fileout file:HIG-RunIISpring16DR80-01830_step1_fast.root \
#    --pileup_input "file:premix_fast.root" \
#    --mc --eventcontent PREMIXRAW --datatier GEN-SIM-RAW --conditions 80X_mcRun2_asymptotic_v14 \
#    --step RECOBEFMIX,DIGIPREMIX_S2,DATAMIX,L1,DIGI2RAW,HLT:25ns10e33_v2 --nThreads 4 --datamix PreMix --era Run2_2016 \
#    --python_filename HIG-RunIISpring16DR80-01830_1_cfg_fast.py --no_exec \
#    --fast \
#    --customise SimGeneral/DataMixingModule/customiseForPremixingInput.customiseForPreMixingInput -n 960

# cmsDriver.py step2 \
#    --filein file:HIG-RunIISpring16DR80-01830_step1_fast.root \
#    --fileout file:HIG-RunIISpring16MiniAODv2-02983_fast.root \
#    --conditions 80X_mcRun2_asymptotic_v14 \
#    --mc --eventcontent MINIAODSIM --runUnscheduled --datatier MINIAODSIM \
#    --step RAW2DIGI,RECO,PAT --nThreads 4 --era Run2_2016 \
#    --python_filename HIG-RunIISpring16MiniAODv2-02983_1_cfg_fast.py --no_exec \
#    --fast \
#    --customise Configuration/DataProcessing/Utils.addMonitoring -n 960
