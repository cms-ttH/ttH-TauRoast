set -e

mkdir -p test/sync_multilepton/ttH
rm -f test/sync_multilepton/ttH/*.root

cmsRun test/ntuplize.py inputFiles=/store/mc/RunIISpring16MiniAODv1/ttHToNonbb_M125_13TeV_powheg_pythia8/MINIAODSIM/PUSpring16RAWAODSIM_80X_mcRun2_asymptotic_2016_v3-v1/50000/0ADF7BAE-0914-E611-B788-0025905A6068.root takeAll=true outputFile=test/sync_multilepton/ttH/test.root channels=tll maxEvents=-1 &

wait

scripts/roaster -afvv data/sync_multilepton.yaml

mkdir -p "test/sync_multilepton/sync/me"
python "test/sync_multilepton.py"

if [ ! -f test/sync_multilepton/sync/mu_names ]; then
   cat<<EOF > test/sync_multilepton/sync/mu_names
nEvent
pt
eta
phi
E
pdgID
charge
jetNDauChargedMVASel
miniIso
miniIsoCharged
miniIsoNeutral
jetPtRel
jetCSV
jetPtRatio
sip3D
dxy
dz
segmentCompatibility
leptonMVA
EOF
fi

if [ ! -f test/sync_multilepton/sync/ele_names ]; then
   cat<<EOF > test/sync_multilepton/sync/ele_names
nEvent
pt
eta
phi
E
pdgID
charge
jetNDauChargedMVASel
miniIso
miniIsoCharged
miniIsoNeutral
jetPtRel
jetCSV
jetPtRatio
sip3D
dxy
dz
eleMVA
leptonMVA
EOF
fi

if [ ! -f test/sync_multilepton/sync/jet_names ]; then
   cat<<EOF > test/sync_multilepton/sync/jet_names
nEvent
pt
eta
phi
E
CSV
METpt
METphi
EOF
fi

scripts/cmp_trees --names test/sync_multilepton/sync/mu_names ~/www/ttH/sync/me_vs_thomas/mu test/sync_multilepton/sync/llr/mu_80X.txt:LLR test/sync_multilepton/sync/me/mu.txt:ND
scripts/cmp_trees --names test/sync_multilepton/sync/ele_names ~/www/ttH/sync/me_vs_thomas/ele test/sync_multilepton/sync/llr/ele_80X.txt:LLR test/sync_multilepton/sync/me/ele.txt:ND
scripts/cmp_trees --names test/sync_multilepton/sync/jet_names ~/www/ttH/sync/me_vs_thomas/jet test/sync_multilepton/sync/llr/jet_80X.txt:LLR test/sync_multilepton/sync/me/jet.txt:ND
