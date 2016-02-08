from lobster import cmssw
from lobster.core import *

from ttH.TauRoast.datasets import datasets

version = "v19"

storage = StorageConfiguration(
        output=[
            "hdfs:///store/user/matze/ttH/" + version,
            "file:///hadoop/store/user/matze/ttH/" + version,
            "root://ndcms.crc.nd.edu//store/user/matze/ttH/" + version,
            "srm://T3_US_NotreDame/store/user/matze/ttH/" + version,
            "chirp://earth.crc.nd.edu:9666/ttH/" + version
        ]
)

processing = Category(
        name='processing',
        cores=1,
        runtime=900,
        memory=1500
)

workflows = []
for label, path in datasets:
    mask = None
    params = []
    if label.endswith('2015D_PR') or label.endswith('2015D_Oct05'):
        mask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt'
        params = ['data=True', 'globalTag=74X_dataRun2_v5']
    workflows.append(Workflow(
        label=label,
        dataset=cmssw.Dataset(
            dataset=path,
            events_per_task=100000,
            lumi_mask=mask
        ),
        category=processing,
        cmssw_config='ntuplize.py',
        arguments=params,
        merge_size='3.5G'
    ))

config = Config(
        label='tau',
        workdir='/tmpscratch/users/matze/tau_' + version,
        plotdir='/afs/crc.nd.edu/user/m/mwolf3/www/lobster/tau_' + version,
        storage=storage,
        workflows=workflows,
        advanced=AdvancedOptions(log_level=1)
)
