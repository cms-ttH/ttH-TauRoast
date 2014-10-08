# A ttH→ττ ntuple analyzing package

## Installation

First, create a CMSSW release and source the software environment:

    export SCRAM_ARCH=slc6_amd64_gcc472
    cmsrel CMSSW_5_3_21
    cd CMSSW_5_3_21/src
    cmsenv

[PyYaml](http://pyyaml.org/wiki/PyYAML) and [pyparsing]() are pre-requisites.
Install them locally after executing `cmsenv` using the python setuptools:

    wget https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py -O - | python - --user
    export PATH=$PATH:$HOME/.local/bin
    easy_install --prefix ~/.local PyYAML pyparsing

In case the last command fails, additional directories may have to be
created with `mkdir -p`.

Afterwards, clone this repository in the CMSSW source area:

    git clone https://github.com/cms-ttH/ttH-TauRoast.git ttH/TauRoast

and add some CMS dependencies:

    git clone git@github.com:veelken/SVfit_standalone.git TauAnalysis/SVfitStandalone

then compile:

    scram b -j32

## Basic usage

Configuration files are stored in `ttH/TauRoast/data`,
with `generic.yaml` being the main configuration file.
Start a test analysis with:

    roaster -atfpvy -n 12345 $LOCALRT/src/ttH/TauRoast/data/generic_ttl.yaml

**after changing relevant paths in the configuration file**.

### Configuration

All relevant setup is performed in `ttH/TauRoast/data/generic.yaml`.  A
short rundown of the sections:

#### Analysis

Contains the analysis parameters.  Relevant are the sections `process` (a
list of physics processes to analyze), `plot` (which processes to plot),
`final mva` (contains the MVA used for limit setting).

Available processes with their cross-section and other options can be found
in `data/processes.yaml`.  Plots are defined in the files starting with
`data/histograms`.

#### Physics

More parameters: `flags` is a list of weights to be used to make MC and
data match up, while `cuts` specifies the requirements on an event to be
considered.

#### Paths

Where things come from and go to.  The `root` is used to store all
information about the analysis, including output plots.  The latter are
specified by `stack format`.  Incoming data is found in `ntuples`.

### Options

See the full list of options with `roaster -h`.

* `-a`: analyze the input data.  This saves pointers to which events
  satisfy all requirements listed in `physics: cuts`.

* `-t`: train the MVA for limit output histograms

* `-f`: fill histograms of events passing `physics: cuts`

* `-p`: save output histograms to disk

* `-v`: be verbose.  Print more about the status of the analysis.

* `-y`: give an overview of how many events passed each requirement in
  `physics: cuts` and how the weights in `physics: flags` affects the event
  yield.

* `-n`: limit the analysis to at most the number of events specified.

* `--list-vars`: list all variables that can be used in `physics: cuts` and
  for plotting.

## Extending

To add new variables, several places need to be changed:

* The `TauAnalysis` package, to add new branches to the ntuples
* The `Branches` class:  Do this by running `setup_branches` on a ROOT file
  containing the desired variable branches.
* Add new access functions to `ttH/TauRoast/src/Accessor.cc` - before
  automatically generated ones - if needed.

These variables can be used in cuts, MVA definitions, and plots.
For new weights, additional modifications to `Weights.cc` might be in order.
