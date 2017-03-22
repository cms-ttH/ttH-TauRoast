from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, StorageConfiguration, Workflow
# from lobster.monitor.elk import ElkInterface

from ttH.TauRoast.datasets import datasets, mctag

version = "v45"
tag = "all"

globaltag_mc = "80X_mcRun2_asymptotic_2016_TrancheIV_v8"
globaltag_data = "80X_dataRun2_2016SeptRepro_v7"
globaltag_data_h = "80X_dataRun2_Prompt_v16"

lumimask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'

storage = StorageConfiguration(
    output=[
        "hdfs://eddie.crc.nd.edu:19000/store/user/matze/ttH/{}".format(version),
        "file:///hadoop/store/user/matze/ttH/{}".format(version),
        "root://deepthought.crc.nd.edu//store/user/matze/ttH/{}".format(version),
        # "chirp://eddie.crc.nd.edu:9094/store/user/matze/ttH/{}".format(version),
        "gsiftp://T3_US_NotreDame/store/user/matze/ttH/{}".format(version),
        "srm://T3_US_NotreDame/store/user/matze/ttH/{}".format(version)
    ]
)

data = Category(
    name='data',
    cores=2,
    runtime=90 * 60,
    disk=400,
    memory=2000
)

tth = Category(
    name='ttH',
    cores=2,
    runtime=90 * 60,
    disk=400,
    memory=2000
)

mc = Category(
    name='mc',
    cores=2,
    runtime=90 * 60,
    disk=400,
    memory=2000
)

workflows = []
for path in datasets(tag):
    _, major, minor, _ = path.split('/')
    minor = mctag(minor)
    label = (major + '_' + minor).replace('-', '_')
    mask = None
    params = ['globalTag=' + globaltag_mc, 'channels=ttl']
    category = mc

    if '/Run2016' in path and path.endswith('MINIAOD'):
        if '/Run2016H' in path:
            gt = globaltag_data_h
        else:
            gt = globaltag_data
        mask = lumimask
        params = ['data=True', 'globalTag=' + gt, 'channels=ttl']
        category = data
        if '/Run2016G' in path or '/Run2017H' in path:
            params += ['hip=false']
    elif label.startswith('ttH'):
        category = tth

    if 'amcatnlo' in path:
        if label.startswith('ttH'):
            params += ['sample=ttH']
        if label.startswith('TTW'):
            params += ['sample=ttW']
        if label.startswith('TTZ'):
            params += ['sample=ttZ']

    workflows.append(Workflow(
        label=label,
        dataset=cmssw.Dataset(
            dataset=path,
            events_per_task=150000,
            lumi_mask=mask
        ),
        category=category,
        merge_size='3g',
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
        bad_exit_codes=[127, 169],
        log_level=1,
        xrootd_servers=[
            'ndcms.crc.nd.edu',
            'cmsxrootd.fnal.gov'
        ]
    )
)
