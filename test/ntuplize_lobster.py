from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, StorageConfiguration, Workflow
from lobster.monitor.elk import ElkInterface

from ttH.TauRoast.datasets import datasets

version = "v32"

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
for path in datasets:
    _, major, minor, _ = path.split('/')
    label = (major + '___' + minor).replace('-', '_')
    mask = None
    params = []
    category = mc

    if 'Run2016' in label and path.endswith('MINIAOD'):
        mask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-277148_13TeV_PromptReco_Collisions16_JSON.txt'
        params += ['data=True', 'globalTag=80X_dataRun2_Prompt_ICHEP16JEC_v0']
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
    label='tau_{}'.format(version),
    workdir='/tmpscratch/users/matze/ttH/{}'.format(version),
    plotdir='/afs/crc.nd.edu/user/m/mwolf3/www/lobster/ttH/{}'.format(version),
    elk=ElkInterface('elk.crc.nd.edu', 9200, 'elk.crc.nd.edu', 5601, 'tth_{}'.format(version)),
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
