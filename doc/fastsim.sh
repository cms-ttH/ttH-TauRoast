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
eval `scram runtime -sh`
scram b
cd ../..

cmsDriver.py SingleNuE10_cfi \
   --fileout file:premix_fast.root \
   --pileup_input "dbs:/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISummer16FSPremix-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v4-v1/GEN-SIM-RECO" \
   --pileup AVE_35_BX_25ns \
   --mc --eventcontent PREMIX --datatier GEN-SIM-DIGI-RAW --conditions auto:run2_mc \
   --step GEN,SIM,RECOBEFMIX,DIGIPREMIX,L1,DIGI2RAW --era Run2_2016 \
   --python_filename premix_fast.py --no_exec \
   --fast

cmsDriver.py step1 \
   --filein "file:HIG-RunIISummer15wmLHEGS-00482_fast.root" \
   --fileout file:HIG-RunIISpring16DR80-01830_step1_fast.root \
   --pileup_input "file:premix_fast.root" \
   --mc --eventcontent PREMIXRAW --datatier GEN-SIM-RAW --conditions 80X_mcRun2_asymptotic_v14 \
   --step RECOBEFMIX,DIGIPREMIX_S2,DATAMIX,L1,DIGI2RAW,HLT:25ns10e33_v2 --nThreads 4 --datamix PreMix --era Run2_2016 \
   --python_filename HIG-RunIISpring16DR80-01830_1_cfg_fast.py --no_exec \
   --fast \
   --customise Configuration/DataProcessing/Utils.addMonitoring -n 960

cmsDriver.py step2 \
   --filein file:HIG-RunIISpring16DR80-01830_step1_fast.root \
   --fileout file:HIG-RunIISpring16MiniAODv2-02983_fast.root \
   --conditions 80X_mcRun2_asymptotic_v14 \
   --mc --eventcontent MINIAODSIM --runUnscheduled --datatier MINIAODSIM \
   --step RAW2DIGI,RECO,PAT --nThreads 4 --era Run2_2016 \
   --python_filename HIG-RunIISpring16MiniAODv2-02983_1_cfg_fast.py --no_exec \
   --fast \
   --customise Configuration/DataProcessing/Utils.addMonitoring -n 960
