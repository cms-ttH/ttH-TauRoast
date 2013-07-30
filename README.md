# A ttH→ττ ntuple analyzing package

## Installation

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

* The `Branches` class.  Do this by running `setup_branches` on a ROOT file
  containing the desired variable branches.
* Add new cuts to `TTHTauTau/Roast/src/CutFlow.cc`. (at the bottom of the
  file)
* Add new MVA variables to one of the `MVABase.{h,cc}` files in the package's
  header/source directories.
