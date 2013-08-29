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

    mkdir -p TTHTauTau
    git clone https://github.com/matz-e/TTHTauTau-Roast.git TTHTauTau/Roast
    scram b -j32

## Basic usage

Configuration files are stored in `TTHTauTau/Roast/data`,
with `generic.yaml` being the main configuration file.
Start a test analysis with:

    roaster -atfpv -n 12345 TTHTauTau/Roast/data/generic.yaml

See the full list of options with `roaster -h`.

## Extending

To add new variables, several places need to be changed:

* The `Branches` class:  Do this by running `setup_branches` on a ROOT file
  containing the desired variable branches.  (optional)
* Add new access functions to `TTHTauTau/Roast/src/Accessor.cc` - before
  automatically generated ones.

For new weights, additional modifications to `Weights.cc` will be in order.
These variables can be used in cuts and MVA definitions.
