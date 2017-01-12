from lobster import cmssw
from lobster.core import AdvancedOptions, Category, Config, StorageConfiguration, Workflow
# from lobster.monitor.elk import ElkInterface

from ttH.TauRoast.datasets import datasets, mctag

version = "v36"
tag = "all"

globaltag_mc = "80X_mcRun2_asymptotic_2016_TrancheIV_v7"
globaltag_data = "80X_dataRun2_2016SeptRepro_v6"
globaltag_data_h = "80X_dataRun2_Prompt_v15"

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
    cores=1,
    runtime=90 * 60,
    disk=400,
    memory=1300
)

tth = Category(
    name='ttH',
    cores=1,
    runtime=90 * 60,
    disk=400,
    memory=1300
)

mc = Category(
    name='mc',
    cores=1,
    runtime=90 * 60,
    disk=400,
    memory=1300
)

workflows = []
for path in datasets(tag):
    _, major, minor, _ = path.split('/')
    minor = mctag.sub('', minor)
    label = (major + '_' + minor).replace('-', '_')
    mask = None
    params = ['globalTag=' + globaltag_mc, 'channels=ttl']
    category = mc

    if '/Run2016' in path and path.endswith('MINIAOD'):
        mask = lumimask
        params = ['data=True', 'globalTag=' + globaltag_data, 'channels=ttl']
        category = data
    elif label.startswith('ttH'):
        if 'tranche3' not in label:
            params += ['reHLT=True']
        category = tth

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
        arguments=params,
        sandbox=[
            cmssw.Sandbox(release='/afs/crc.nd.edu/user/m/mwolf3/work/ttH/releases/CMSSW_8_0_21'),
            cmssw.Sandbox(release='/afs/crc.nd.edu/user/m/mwolf3/work/ttH/releases/CMSSW_8_0_21_rh7')
        ]
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
