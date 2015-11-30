# A ttH→ττ ntuple analyzing package

## TODO

Open issues:

* Scale factors/weights

  * top pt
  * lepton

* Plotting

  * systematics
  * `s/sqrt(s+b)` instead of ratio?

## Preface

Every command is to be used at your own risk.  Look at configuration files
and paths and modify them as needed!

The following lines are expected in the shell configuration to ensure that
CCL programs can be used:

    cctools=cctools-028-47f7c4ea-cvmfs-40cf5bba
    export PYTHONPATH=$PYTHONPATH:/afs/crc.nd.edu/group/ccl/software/$cctools/x86_64/redhat6/lib/python2.6/site-packages
    export PATH=/afs/crc.nd.edu/group/ccl/software/$cctools/x86_64/redhat6/bin:$PATH

## Installation

First, create a CMSSW release and source the software environment:

    cmsrel CMSSW_7_4_7
    cd CMSSW_7_4_7/src
    cmsenv
    git cms-init

Afterwards, clone this repository in the CMSSW source area:

    git clone git@github.com:cms-ttH/ttH-TauRoast.git ttH/TauRoast

and add some CMS dependencies for TLL:

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

    gzip -d EgammaAnalysis/ElectronTools/data/PHYS14/*.gz

or add some CMS dependencies for TTL:

    git cms-merge-topic --unsafe ikrav:egm_id_7.4.12_v1

then compile:

    scram b -j32

## Creating Ntuples

See the parameter sets in `ttH/TauRoast/test`, e.g., `nutplize.py`.

A [lobster][lobster] configuration is in `ttH/TauRoast/test/lobster.yaml`,
to produce the latest and greatest ntuple.

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

### Producing meaningful output

To evaluate ntuples with a given configuration including systematics,
modify the following steps, which will use [Makeflow][makeflow] to process
data in a distributed fashion (removing the need to deal with Condor et al
directly):

    mk_configurations data/ttl.yaml ~/www/ttH/ttl/v8_2015-12-25/
    cd ~/www/ttH/ttl/v8_2015-12-25/
    makeflow -T wq -M taus_FTW --wrapper ./w.sh --wrapper-input w.sh

Where `taus_FTW` is a reference and can be freely chosen.  In a new
terminal, issue this command to start workers processing the steps:

    work_queue_pool -T condor -M taus_FTW -w 0 -W 100

Again, `taus_FTW` can be modified, but has to match with what was passed to
the `makeflow` command.

To create limits, now create a new release (thank you CMS folks):

    cd /somewhere/where/I/create/releases
    export SCRAM_ARCH=slc6_amd64_gcc481
    scram project CMSSW CMSSW_7_1_5
    cd CMSSW_7_1_5/src
    cmsenv
    git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
    git clone https://github.com/cms-analysis/CombineHarvester.git CombineHarvester
    git clone git@github.com:cms-ttH/ttH-TauRoast.git ttH/TauRoast
    scram b -j 16

And then run the following command to create a datacard and run a limit:

    mk_datacard -d HT ~/www/ttH/ttl/v8_2015-12-25/_ttl/limits.root
    combine -M Asymptotic -m 125 limits/125/ttH_ttl.txt

Again, fix all paths to desired values.

### Calculating processed lumis

Use [BrilCalc][brilcalc].  A pain to setup, but it works.

### Calculating pileup weights

See the [twiki][pileup] for reference, together with the [PPD twiki][ppd].
Use the wrapper:

    update_pileup <data processing json>

# Other ttH groups

Link to work from other ttH groups

## Lepton+Jets

### Sync

Run the following to test the sync status with the LJ analysis (for the TTL
channel):

    cmsRun test/ntuplize_lj_sync.py \
        outputFile=test/leptonplusjets/ttH2bb_125_pow/test.root \
        inputFiles=/store/mc/RunIISpring15MiniAODv2/ttHTobb_M125_13TeV_powheg_pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/DA1B6FD6-C46D-E511-9C7B-00A0D1EE29B8.root
    roaster -a --debug-cuts data/lj_sync.yaml

Detailed event lists are present in the configured output directories for
the last two steps.

### Twiki

* [Analysis Twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/TTbarHiggsRun2LeptonPlusJets)
* [Reference Analysis](https://twiki.cern.ch/twiki/bin/view/CMS/TTbarHbbRun2ReferenceAnalysis)

## Multilepton

### Sync

### Twiki

* [Analysis Twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/TTH-Multileptons)

# History

## v13

* Run with LJ sync config, too see where the e/μ mismodelling comes from

### v13.1

* Apply tighter jet cuts

## v12

* Synced with ttH Lepton+Jets

  * new electron ID
  * lower μ pt cuts

### v12.1

* Better τ gen-matching resulting in higher efficiency

## v11

### v11.1

* τ raw isolation w/ 3 hits
* τ generator level visible pt

### v11.2

* More powheg samples

[lobster]: http://lobster.crc.nd.edu
[makeflow]: http://ccl.cse.nd.edu/software/manuals/makeflow.html
[brilcalc]: http://cms-service-lumi.web.cern.ch/cms-service-lumi/brilwsdoc.html#brilcalc
[pileup]: https://twiki.cern.ch/twiki/bin/view/CMS/PileupJSONFileforData#2015_Pileup_JSON_Files
[ppd]: https://twiki.cern.ch/twiki/bin/viewauth/CMS/PdmVPileUpDescription#Startup2015
