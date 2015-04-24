# Quit on errors
set -e

# Random throw-away directory to work in
dir=$(mktemp -d)
echo Creating release in $dir
cd $dir

# Release setup
export SCRAM_ARCH=slc6_amd64_gcc481
scramv1 project CMSSW CMSSW_7_2_4
cd CMSSW_7_2_4/src
eval $(scramv1 runtime -sh)

# External python dependencies
wget https://bitbucket.org/pypa/setuptools/raw/bootstrap/ez_setup.py -O - | python - --user
export PATH=$PATH:$HOME/.local/bin
easy_install --prefix ~/.local PyYAML

# TauRoast + 2 dependencies
git clone https://github.com/cms-ttH/MiniAOD.git
git clone https://github.com/veelken/SVfit_standalone.git TauAnalysis/SVfitStandalone
git clone https://github.com/cms-ttH/ttH-TauRoast.git ttH/TauRoast

scram b -j32

# Produce ntuple from MC
cmsRun ttH/TauRoast/test/ntuplize.py

mkdir -p test_ntuple/ttH_125
mv test.root test_ntuple/ttH_125

# Create configuration and run it
cat <<EOF >test.yaml
# vim: set sw=4 sta et :

ntupledir: test_ntuple
outdir: test_output

lumi: 10000

plot:
  - ttH_125

cuts:
  - "τ₁ LT pt": "taus[0].leadingTrackPt() > 5"
  - "τ₂ LT pt": "taus[1].leadingTrackPt() > 5"
  - "ΔR(ττ)  > 0.5": "dR(taus[0], taus[1]) > 0.5"
  - "ΔR(τ₁l) > 0.25": "dR(taus[0], leptons[0]) > 0.25"
  - "ΔR(τ₂l) > 0.25": "dR(taus[1], leptons[0]) > 0.25"
  - "Opposite charge for τ": "taus[0].charge() * taus[1].charge() < 0"

weights:
  - Generator
EOF

roaster -fp test.yaml

echo You might want to delete $dir
