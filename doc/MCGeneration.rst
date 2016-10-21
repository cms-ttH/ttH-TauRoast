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
script like this::

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
    cmsDriver.py Configuration/GenProduction/python/HIG-RunIISummer15wmLHEGS-00482-fragment.py --fileout file:HIG-RunIISummer15wmLHEGS-00482.root --mc --eventcontent RAWSIM,LHE --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1,Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM,LHE --conditions MCRUN2_71_V1::All --beamspot Realistic50ns13TeVCollision --step LHE,GEN,SIM --magField 38T_PostLS1 --python_filename HIG-RunIISummer15wmLHEGS-00482_1_cfg.py --no_exec -n 42 || exit $? ;
