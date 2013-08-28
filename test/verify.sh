#!/bin/sh

scram b

if [ "$(basename $PWD)" == "test" ]; then
    dir=$(basename $PWD)
else
    dir=$PWD
fi

roaster -atfpvn 12345 $dir/data/generic.yaml 2>&1 | tee $dir/test/verify.log
roaster -y $dir/data/generic.yaml 2>&1 | tee $dir/test/verify.yield

cp $dir/test/tmp/final\ mva/TMVAClassification_BDTG.weights.xml $dir/test/verify.weights
