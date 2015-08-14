# A ttH→ττ ntuple analyzing package

## Installation

First, create a CMSSW release and source the software environment:

    cmsrel CMSSW_7_4_7
    cd CMSSW_7_4_7/src
    cmsenv
    git cms-init

[PyYaml](http://pyyaml.org/wiki/PyYAML) is a pre-requisite.
Install it locally after executing `cmsenv` using the python setuptools:

    wget https://bootstrap.pypa.io/get-pip.py; python get-pip.py --user
    export PATH=$PATH:$HOME/.local/bin
    # actual yaml installation
    pip install --user PyYAML

Afterwards, clone this repository in the CMSSW source area:

    git clone git@github.com:cms-ttH/ttH-TauRoast.git ttH/TauRoast

and add some CMS dependencies:

    git remote add cmg-central https://github.com/CERN-PH-CMG/cmg-cmssw.git
    git fetch cmg-central

    cat <<EOF >.git/info/sparse-checkout
    /.gitignore/
    /CMGTools/TTHAnalysis/data/
    /EgammaAnalysis/ElectronTools/
    EOF

    git checkout -b CMGTools-from-CMSSW_7_4_7 cmg-central/CMGTools-from-CMSSW_7_4_7
    git clone git@github.com:cms-ttH/MiniAOD.git
    git clone git@github.com:cms-ttH/ttH-LeptonID.git ttH/LeptonID
    git clone git@github.com:veelken/SVfit_standalone.git TauAnalysis/SVfitStandalone

    gzip -d EgammaAnalysis/ElectronTools/data/PHYS14/*.gz

then compile:

    scram b -j32

## Creating Ntuples

See the parameter sets in `ttH/TauRoast/test`, e.g., `nutplize.py`.

## Basic usage

Configuration files are stored in `ttH/TauRoast/data`,
with `ttl.yaml` being the main configuration file.
Start a test analysis with:

    roaster -afpv data/ttl.yaml

**after changing relevant paths in the configuration file**.

### Options

Always use `-h` to find a comprehensive overview.

| Option | Effect                                                 |
| ---    | ---                                                    |
| `-a`   | analyze datasets (produces `TMVA` compatible output)   |
| `-f`   | fill histograms (produces `combine` compatible output) |
| `-p`   | save plots                                             |
