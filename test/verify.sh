#!/bin/sh

if [ "$(basename $PWD)" == "test" ]; then
    dir=$(basename $PWD)
else
    dir=$PWD
fi

chan=$1

roaster -o paths.root=test/tmp_$chan -atfpvvn 12345 $dir/data/generic_$chan.yaml &> $dir/test/verify_$chan.log
res=$?
if [ $res != 0 ]; then
   tail $dir/test/verify_$chan.log
   exit $res
fi
roaster -o paths.root=test/tmp_$chan -y $dir/data/generic_$chan.yaml &> $dir/test/verify_$chan.yield

cp $dir/test/tmp_$chan/final\ mva/TMVAClassification_BDTG.weights.xml $dir/test/verify_$chan.weights

dff=$(git diff --name-status $dir/test/*$chan*)
if [ -z "$dff" ]; then
    echo No changes
else
    echo "$dff"
fi
