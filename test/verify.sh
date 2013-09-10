#!/bin/sh

if [ "$(basename $PWD)" == "test" ]; then
    dir=$(basename $PWD)
else
    dir=$PWD
fi

chan=$1

roaster -atfpvn 12345 $dir/data/generic_$chan.yaml &> $dir/test/verify_$chan.log
roaster -y $dir/data/generic_$chan.yaml &> $dir/test/verify_$chan.yield

cp $dir/test/tmp_$chan/final\ mva/TMVAClassification_BDTG.weights.xml $dir/test/verify_$chan.weights

dff=$(git diff --name-status $dir/test/*$chan*)
if [ -z "$dff" ]; then
    echo No changes
else
    echo "$dff"
fi
