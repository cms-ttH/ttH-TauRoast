# A ttH→ττ ntuple analyzing package

## Installation

[PyYaml](http://pyyaml.org/wiki/PyYAML) is a pre-requisite.  Install it
locally after executing `cmsenv`:

    cd /tmp
    wget -O - http://pyyaml.org/download/pyyaml/PyYAML-3.10.tar.gz|tar xzf -
    cd PyYAML-3.10/
    python setup.py install --user
    cd ..
    rm -rf PyYAML-3.10/

Afterwards, clone this repository in the CMSSW source area:

    mkdir -p ttH
    git clone https://github.com/cms-ttH/ttH-TauRoast.git ttH/TauRoast
    scram b -j32

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
