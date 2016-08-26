from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, StorageConfiguration, Workflow
# from lobster.monitor.elk import ElkInterface

from ttH.TauRoast.datasets import datasets, mctag

version = "v32"
tag = "all"

globaltag_mc = "80X_mcRun2_asymptotic_2016_miniAODv2_v1"
globaltag_data = "80X_dataRun2_Prompt_ICHEP16JEC_v0"

storage = StorageConfiguration(
    output=[
        "hdfs:///store/user/matze/ttH/{}".format(version),
        "file:///hadoop/store/user/matze/ttH/{}".format(version),
        "root://deepthought.crc.nd.edu//store/user/matze/ttH/{}".format(version),
        "chirp://eddie.crc.nd.edu:9094/store/user/matze/ttH/{}".format(version),
        "gsiftp://T3_US_NotreDame/store/user/matze/ttH/{}".format(version),
        "srm://T3_US_NotreDame/store/user/matze/ttH/{}".format(version)
    ]
)

data = Category(
    name='data',
    cores=2,
    runtime=75 * 60,
    disk=1000,
    memory=1200
)

tth = Category(
    name='ttH',
    cores=2,
    runtime=75 * 60,
    disk=1000,
    memory=1200
)

mc = Category(
    name='mc',
    cores=2,
    runtime=75 * 60,
    disk=1000,
    memory=1200
)

workflows = []
for path in datasets(tag):
    _, major, minor, _ = path.split('/')
    label = (major + '_' + minor.replace(mctag, '')).replace('-', '_')
    mask = None
    params = ['globalTag=' + globaltag_mc]
    category = mc

    if '/Run2016' in path and path.endswith('MINIAOD'):
        mask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-277148_13TeV_PromptReco_Collisions16_JSON.txt'
        params = ['data=True', 'globalTag=' + globaltag_data]
        category = data
    elif label.startswith('ttH'):
        category = tth

    workflows.append(Workflow(
        label=label,
        dataset=cmssw.Dataset(
            dataset=path,
            events_per_task=50000,
            lumi_mask=mask
        ),
        category=category,
        pset='ntuplize.py',
        arguments=params
    ))

config = Config(
    label='tau_{}_{}'.format(version, tag),
    workdir='/tmpscratch/users/matze/ttH/{}_{}'.format(version, tag),
    plotdir='/afs/crc.nd.edu/user/m/mwolf3/www/lobster/ttH/{}_{}'.format(version, tag),
    # elk=ElkInterface('elk.crc.nd.edu', 9200, 'elk.crc.nd.edu', 5601, 'ttH_{}_{}'.format(version, tag).lower()),
    storage=storage,
    workflows=workflows,
    advanced=AdvancedOptions(
        log_level=1,
        xrootd_servers=[
            'ndcms.crc.nd.edu',
            'cmsxrootd.fnal.gov'
        ]
    )
)
