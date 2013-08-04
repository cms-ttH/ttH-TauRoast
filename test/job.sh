#!/bin/sh

set -e

export VO_CMS_SW_DIR=/pscratch/osg/app/cmssoft/cms
source $VO_CMS_SW_DIR/cmsset_default.sh
export SCRAM_ARCH=slc5_amd64_gcc462

cd /afs/crc.nd.edu/user/m/mwolf3/work/ttH/CMSSW_5_3_2_patch5
cmsenv
roaster -v $*
