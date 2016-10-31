MC Generation
=============

Pulling in basic steps from MCM
-------------------------------

We need the basic setup scripts to generate `LHE` inputs for the `GenSim`
step.  Go to the MCM page, select the `Requests` tab and click on
`Navigation` to search for the dataset to clone.  This should bring you to
a page like this one::

    https://cms-pdmv.cern.ch/mcm/requests?page=0&prepid=HIG-RunIISummer15wmLHEGS-00482

Now we can click the second icon from the left, which will download a setup
script like this

.. code-block:: shell

    #!/bin/bash
    source  /afs/cern.ch/cms/cmsset_default.sh
    export SCRAM_ARCH=slc6_amd64_gcc481
    if [ -r CMSSW_7_1_25/src ] ; then
     echo release CMSSW_7_1_25 already exists
    else
    scram p CMSSW CMSSW_7_1_25
    fi
    cd CMSSW_7_1_25/src
    eval `scram runtime -sh`

    export X509_USER_PROXY=$HOME/private/personal/voms_proxy.cert
    curl -s --insecure https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_fragment/HIG-RunIISummer15wmLHEGS-00482 --retry 2 --create-dirs -o Configuration/GenProduction/python/HIG-RunIISummer15wmLHEGS-00482-fragment.py
    [ -s Configuration/GenProduction/python/HIG-RunIISummer15wmLHEGS-00482-fragment.py ] || exit $?;

    scram b
    cd ../../
    cmsDriver.py Configuration/GenProduction/python/HIG-RunIISummer15wmLHEGS-00482-fragment.py \
      --fileout file:HIG-RunIISummer15wmLHEGS-00482.root \
      --mc --eventcontent RAWSIM,LHE \
      --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring \
      --datatier GEN-SIM,LHE --conditions MCRUN2_71_V1::All --beamspot Realistic50ns13TeVCollision \
      --step LHE,GEN,SIM --magField 38T_PostLS1 \
      --python_filename HIG-RunIISummer15wmLHEGS-00482_1_cfg.py --no_exec -n 42 || exit $? ;

Repeat this for the `DR` step to see the following lines

.. code-block:: shell

    cmsDriver.py step1 \
      --filein "dbs:/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer15wmLHEGS-MCRUN2_71_V1-v1/GEN-SIM" \
      --fileout file:HIG-RunIISpring16DR80-01830_step1.root \
      --pileup_input "dbs:/Neutrino_E-10_gun/RunIISpring15PrePremix-PU2016_80X_mcRun2_asymptotic_v14-v2/GEN-SIM-DIGI-RAW" \
      --mc --eventcontent PREMIXRAW --datatier GEN-SIM-RAW --conditions 80X_mcRun2_asymptotic_v14 \
      --step DIGIPREMIX_S2,DATAMIX,L1,DIGI2RAW,HLT:25ns10e33_v2 --nThreads 4 --datamix PreMix --era Run2_2016 \
      --python_filename HIG-RunIISpring16DR80-01830_1_cfg.py --no_exec \
      --customise Configuration/DataProcessing/Utils.addMonitoring -n 960 || exit $? ;

    cmsDriver.py step2 \
      --filein file:HIG-RunIISpring16DR80-01830_step1.root \
      --fileout file:HIG-RunIISpring16DR80-01830.root \
      --mc --eventcontent AODSIM,DQM --runUnscheduled --datatier AODSIM,DQMIO \
      --conditions 80X_mcRun2_asymptotic_v14 \
      --step RAW2DIGI,RECO,EI,DQM:DQMOfflinePOGMC --nThreads 4 --era Run2_2016 \
      --python_filename HIG-RunIISpring16DR80-01830_2_cfg.py --no_exec \
      --customise Configuration/DataProcessing/Utils.addMonitoring -n 960 || exit $? ;

Note that this step is happening in ``CMSSW_8_0_14``.

And for the `MiniAOD` step (note again the used release, which is equivalent to the `DR` step)

.. code-block:: shell

  cmsDriver.py step1 \
    --filein "dbs:/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISpring16DR80-premix_withHLT_80X_mcRun2_asymptotic_v14-v1/AODSIM" \
    --fileout file:HIG-RunIISpring16MiniAODv2-02983.root \
    --mc --eventcontent MINIAODSIM --runUnscheduled --datatier MINIAODSIM \
    --conditions 80X_mcRun2_asymptotic_v14 \
    --step PAT --era Run2_2016 \
    --python_filename HIG-RunIISpring16MiniAODv2-02983_1_cfg.py --no_exec \
    --customise Configuration/DataProcessing/Utils.addMonitoring -n 960 || exit $? ;

Setup for FastSim
-----------------

Note the following from the `FastSim TWiki`_ about `runTheMatrix.py` workflows:

    **FullSim:**

    * do not run harvesting (the 4th command printed by runTheMatrix.py)
    * do not run the ALCA step (the 5th command printed by runTheMatrix.py)
    * in the 2nd command printed by runTheMatrix.py

      *  replace the DIGI:pdigi_valid with DIGI (do not produce the truth collection 'trackingParticles')

    * in the 3rd command

      * remove ,EI,VALIDATION from the -s option
      * replace --eventcontent RECOSIM,DQM with --eventcontent AODSIM
      * replace --datatier GEN-SIM-DIGI-RECO,DQMIO with --datatier AODSIM

.. _FastSim TWiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFastSimulationExamples

Now with this in mind, set up the environment as done in the first script.
Execute the ``cmsDriver.py`` command, adding a ``--fast`` option to the
command line to activate `FastSim`.  This should produce a first output
file with `LHEGS` content.
