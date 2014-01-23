#!/bin/sh

set -e

export VO_CMS_SW_DIR=/pscratch/osg/app/cmssoft/cms
source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc5_amd64_gcc462

cd /afs/crc.nd.edu/user/m/mwolf3/work/ttH/releases/CMSSW_5_3_11_patch6
cmsenv
roaster -v $*
