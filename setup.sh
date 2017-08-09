#!/bin/sh

cat <<EOF
========================================================================
This script is creating a working directory for ntuplizing and plotting
the ttH→ττ analysis in 80X

Output is in setup.log
========================================================================
EOF

(
   set -e
   set -o xtrace

   export SCRAM_ARCH=slc6_amd64_gcc530
   scramv1 project CMSSW CMSSW_8_0_26_patch1
   cd CMSSW_8_0_26_patch1/src
   set +o xtrace
   eval $(scramv1 runtime -sh)
   set -o xtrace
   git cms-init > /dev/null

   git cms-merge-topic cms-met:METRecipe_8020
   git cms-merge-topic ikrav:egm_id_80X_v2
   git cms-merge-topic gpetruc:badMuonFilters_80X_v2

   git clone -b CMSSW_8_0_24_v1_sync git@github.com:cms-ttH/MiniAOD.git
   git clone git@github.com:cms-ttH/ttH-LeptonID.git ttH/LeptonID
   git clone git@github.com:cms-ttH/ttH-TauRoast.git ttH/TauRoast

   curl --create-dirs -o LLR/NtupleProducer/plugins/MuonRefPruner.cc https://raw.githubusercontent.com/LLRCMS/LLRHiggsTauTau/6d4d486beb11efc85d4d3d4184c4e00e85c1261f/NtupleProducer/plugins/MuonRefPruner.cc
   cat <<EOB >LLR/NtupleProducer/plugins/BuildFile.xml
<use name="FWCore/Framework"/>
<use name="FWCore/MessageLogger"/>
<use name="FWCore/ParameterSet"/>
<use name="FWCore/Utilities"/>
<use name="DataFormats/MuonReco"/>
<use name="DataFormats/PatCandidates"/>
EOB

   scram b -j 8

   pip install --user --upgrade --global-option=build_ext --global-option="-L/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/python/2.7.11-ikhhed/lib/" root_numpy

   cd $CMSSW_BASE/external/$SCRAM_ARCH/
   git clone -b egm_id_80X_v1 https://github.com/ikrav/RecoEgamma-ElectronIdentification.git data/RecoEgamma/ElectronIdentification/data
) > setup.log


cat <<EOF
========================================================================
Output is in setup.log
========================================================================
EOF
