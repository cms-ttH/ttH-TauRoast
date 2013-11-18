# A ttH→ττ ntuple analyzing package

## Installation

[PyYaml](http://pyyaml.org/wiki/PyYAML) and [pyparsing]() are pre-requisites.
Install them locally after executing `cmsenv`:

    cd /tmp
    wget -O - http://pyyaml.org/download/pyyaml/PyYAML-3.10.tar.gz|tar xzf -
    cd PyYAML-3.10/
    python setup.py install --user
    cd ..
    rm -rf PyYAML-3.10/

    wget -O - "http://downloads.sourceforge.net/project/pyparsing/pyparsing/pyparsing-2.0.1/pyparsing-2.0.1.tar.gz?r=&ts=1384527918&use_mirror=hivelocity"|tar xzf -
    cd pyparsing-2.0.1/
    python setup.py install --user
    cd ..
    rm -rf pyparsing-2.0.1/

Afterwards, clone this repository in the CMSSW source area:

    mkdir -p ttH
    git clone https://github.com/cms-ttH/ttH-TauRoast.git ttH/TauRoast
    scram b -j32

To obtain a nice webfolder display for plot output directories, clone the
[ttH multi-lepton](https://github.com/cms-ttH/ttHMultileptonAnalysis) code:

    cd $LOCALRT/src
    git clone https://github.com/cms-ttH/ttHMultileptonAnalysis.git

## Basic usage

Configuration files are stored in `ttH/TauRoast/data`,
with `generic.yaml` being the main configuration file.
Start a test analysis with:

    roaster -atfpv -n 12345 $LOCALRT/src/ttH/TauRoast/data/generic.yaml

See the full list of options with `roaster -h`.

## Extending

To add new variables, several places need to be changed:

* The `TauAnalysis` package, to add new branches to the ntuples
* The `Branches` class:  Do this by running `setup_branches` on a ROOT file
  containing the desired variable branches.
* Add new access functions to `ttH/TauRoast/src/Accessor.cc` - before
  automatically generated ones - if needed.

These variables can be used in cuts, MVA definitions, and plots.
For new weights, additional modifications to `Weights.cc` might be in order.
