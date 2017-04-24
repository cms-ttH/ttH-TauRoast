# A ttH→ττ ntuple analyzing package

## Preface

Every command is to be used at your own risk.  Look at configuration files
and paths and modify them as needed!

The following lines are expected in the shell configuration to ensure that
CCL programs can be used:

    cctools=cctools-028-47f7c4ea-cvmfs-40cf5bba
    export PYTHONPATH=$PYTHONPATH:/afs/crc.nd.edu/group/ccl/software/$cctools/x86_64/redhat6/lib/python2.6/site-packages
    export PATH=/afs/crc.nd.edu/group/ccl/software/$cctools/x86_64/redhat6/bin:$PATH

## Installation

Run `setup.sh` to get a new release with all the necessary packages.  For a
fresh installation:

    curl https://raw.githubusercontent.com/cms-ttH/ttH-TauRoast/master/setup.sh|sh -

To create a release to create datacards and run limits, use

    curl https://raw.githubusercontent.com/cms-ttH/ttH-TauRoast/master/setup_limit.sh|sh -

Note that both setup scripts may fail due to concurrent `scram` usage, in
which case `scram build -j 8` or similar has to be re-issued.

## Creating Ntuples

See the parameter sets in `ttH/TauRoast/test`, e.g., `nutplize.py`.

A [Lobster][lobster] configuration is in
`ttH/TauRoast/test/ntuplize_lobster.py`,
to produce the latest and greatest ntuple.

Similarly, for CRAB, a configuration can be found in
`ttH/TauRoast/test/ntuplize_crabby.py`.  It has to be called by the CRAB
wrapper `crabby`, provided by this package.

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

### Limits

Run the following command to create a datacard and run a limit:

    roaster -d --discriminant NumInclusiveJets data/ttl_ichep.yaml
    combine -M Asymptotic -m 125 limits/125/ttH_inclusive.txt

Again, fix all paths to desired values.

### Calculating processed lumis

Use [BrilCalc][brilcalc].  A pain to setup, but it works.

### Calculating pileup weights

See the [twiki][pileup] for reference, together with the [PPD twiki][ppd].
Use the wrapper:

    update_pileup <data processing json>

### Ntuple versioning

See [here](Changelog.md).

[lobster]: http://lobster.crc.nd.edu
[brilcalc]: http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html#brilcalc
[pileup]: https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData#2015_Pileup_JSON_Files
[ppd]: https://twiki.cern.ch/twiki/bin/viewauth/CMS/PdmVPileUpDescription#Startup2015
