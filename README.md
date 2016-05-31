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

First, create a CMSSW release and source the software environment:

    cmsrel CMSSW_7_6_3_patch2
    cd CMSSW_7_6_3_patch2/src
    cmsenv
    git cms-init

Afterwards, clone these repositories in the CMSSW source area:

    git clone -b CMSSW_7_6_3 git@github.com:cms-ttH/MiniAOD.git
    git clone git@github.com:cms-ttH/ttH-LeptonID.git ttH/LeptonID
    git clone git@github.com:cms-ttH/ttH-TauRoast.git ttH/TauRoast

and add some CMS dependencies for TTL:

    git cms-addpkg PhysicsTools/JetMCAlgos/
    cd PhysicsTools/JetMCAlgos/plugins/
    rm GenHFHadronMatcher.cc
    wget https://twiki.cern.ch/twiki/pub/CMSPublic/GenHFHadronMatcher/GenHFHadronMatcher.cc
    wget https://twiki.cern.ch/twiki/pub/CMSPublic/GenHFHadronMatcher/GenTtbarCategorizer.cc
    cd -
    cd PhysicsTools/JetMCAlgos/python/
    wget https://twiki.cern.ch/twiki/pub/CMSPublic/GenHFHadronMatcher/GenTtbarCategorizer_cfi.py.txt
    mv GenTtbarCategorizer_cfi.py.txt GenTtbarCategorizer_cfi.py
    cd -

then compile:

    scram b -j32

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

    sh test/ntuplize_lj_sync.sh
    roaster -a --debug-cuts data/lj_sync.yaml

Detailed event lists are present in the configured output directories for
the last two steps.

### Twiki

* [Analysis Twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/TTbarHiggsRun2LeptonPlusJets)
* [Reference Analysis (2015)](https://twiki.cern.ch/twiki/bin/view/CMS/TTbarHbbRun2ReferenceAnalysis)
* [Reference Analysis (2016)](https://twiki.cern.ch/twiki/bin/viewauth/CMS/TTbarHbbRun2ReferenceAnalysis_StartOf2016)

## Multilepton

### Sync

### Twiki

* [Analysis Twiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/TTH-Multileptons)

# History

## v29

* Tighter skim for TLL

## v28

* New output structure with FWLite ntuples

## v27

* ¿Various improvements?

## v26

* As v25, but done right with Lobster

## v25

* More τ selection bugfixes, with CRAB

## v24

* Same as v23, but processed with CRAB

## v23

* Bugfixes for τ selection

## v22

* Same as v21, but processed with CRAB

## v21

* First 76X ntuple version

## v20

* Restrict to two τs again

## v19

* Use silver JSON

## v18

* Actually allow more than 2 b-tags

## v17

* Same as v16, but run with CRAB, to see if Lobster is processing datasets
  wrong

## v16

* Yet more sync stuff
* Ran with 0-2 τ per combination to compare to lepton+jets

## v15

* Sync data sample setup with the ttH→bb group
* Fixes JER/JES for data

## v14

* Run with new globaltags
* Includes new JER/JES
* Switch e MVA ID over to yet another producer

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
