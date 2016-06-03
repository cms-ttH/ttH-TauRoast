from lobster import cmssw
from lobster.core import *

from ttH.TauRoast.datasets import *

version = "v30"

storage = StorageConfiguration(
        output=[
            "hdfs:///store/user/matze/ttH/{}".format(version),
            "file:///hadoop/store/user/matze/ttH/{}".format(version),
            "root://deepthought.crc.nd.edu//store/user/matze/ttH/{}".format(version),
            "chirp://earth.crc.nd.edu:9666/ttH/{}".format(version),
            "srm://T3_US_NotreDame/store/user/matze/ttH/{}".format(version)
        ]
)

data = Category(
        name='data',
        cores=2,
        runtime=75 * 60
)

tth = Category(
        name='ttH',
        cores=2,
        runtime=75 * 60
)

mc = Category(
        name='mc',
        cores=2,
        runtime=75 * 60
)

workflows = []
datasets = sum(
        [
            datasets_ttH,
            datasets_ttjets,
            datasets_other,
            datasets_2015D
        ], []
)
for label, path in datasets:
    mask = None
    params = []
    category = mc

    if '2015' in label:
        mask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt'
        params += ['data=True', 'globalTag=76X_dataRun2_v12']
        category = data
    elif label.startswith('ttH'):
        category = tth

    workflows.append(Workflow(
        label=label,
        dataset=cmssw.Dataset(
            dataset=path,
            events_per_task=10000,
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
