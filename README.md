# A ttH→ττ ntuple analyzing package

## Installation

First, create a CMSSW release and source the software environment:

    cmsrel CMSSW_7_2_4
    cd CMSSW_7_2_4/src
    cmsenv

[PyYaml](http://pyyaml.org/wiki/PyYAML) is a pre-requisite.
Install it locally after executing `cmsenv` using the python setuptools:

    # if not installed system-wide
    wget https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py -O - | python - --user
    export PATH=$PATH:$HOME/.local/bin
    # actual yaml installation
    easy_install --prefix ~/.local PyYAML

In case the last command fails, additional directories may have to be
created with `mkdir -p`.

Afterwards, clone this repository in the CMSSW source area:

    git clone git@github.com:cms-ttH/ttH-TauRoast.git ttH/TauRoast

and add some CMS dependencies:

    git clone git@github.com:cms-ttH/MiniAOD.git
    git clone git@github.com:veelken/SVfit_standalone.git TauAnalysis/SVfitStandalone

then compile:

    scram b -j32

## Basic usage

Configuration files are stored in `ttH/TauRoast/data`,
with `ttl.yaml` being the main configuration file.
Start a test analysis with:

    roaster -fpv data/ttl.yaml

**after changing relevant paths in the configuration file**.

### Options

Always use `-h` to find a comprehensive overview.

| Option | Effect                                                                     |
| ---    | ---                                                                        |
| `-f`   | analyze dataset and fill histograms (produces `combine` compatible output) |
| `-p`   | save plots                                                                 |
