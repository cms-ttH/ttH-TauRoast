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
   scramv1 project CMSSW CMSSW_8_0_18
   cd CMSSW_8_0_18/src
   set +o xtrace
   eval $(scramv1 runtime -sh)
   set -o xtrace
   git cms-init > /dev/null

   git clone -b Framework-JEC git@github.com:cms-ttH/MiniAOD.git
   git clone git@github.com:cms-ttH/ttH-LeptonID.git ttH/LeptonID
   git clone git@github.com:cms-ttH/ttH-TauRoast.git ttH/TauRoast

   # git cms-addpkg PhysicsTools/JetMCAlgos/
   # cd PhysicsTools/JetMCAlgos/plugins/
   # rm GenHFHadronMatcher.cc
   # wget https://twiki.cern.ch/twiki/pub/CMSPublic/GenHFHadronMatcher/GenHFHadronMatcher.cc
   # wget https://twiki.cern.ch/twiki/pub/CMSPublic/GenHFHadronMatcher/GenTtbarCategorizer.cc
   # cd -
   # cd PhysicsTools/JetMCAlgos/python/
   # wget https://twiki.cern.ch/twiki/pub/CMSPublic/GenHFHadronMatcher/GenTtbarCategorizer_cfi.py.txt
   # mv GenTtbarCategorizer_cfi.py.txt GenTtbarCategorizer_cfi.py
   # cd -

   scram b -j 8
) > setup.log


cat <<EOF
========================================================================
Output is in setup.log
========================================================================
EOF
