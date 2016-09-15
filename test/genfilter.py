import FWCore.ParameterSet.Config as cms

# from PhysicsTools.HepMCCandAlgos.genParticles_cfi import genParticles
# from RecoJets.Configuration.GenJetParticles_cff import genParticlesForJetsNoNu
# from RecoJets.Configuration.RecoGenJets_cff import ak4GenJetsNoNu

process = cms.Process("Filter")

process.options = cms.untracked.PSet(
    wantSummary=cms.untracked.bool(True),
    numberOfThreads=cms.untracked.uint32(2),
    numberOfStreams=cms.untracked.uint32(0),
    SkipEvent=cms.untracked.vstring('ProductNotFound')
)
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(10000))

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 2000

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.GlobalTag.globaltag = "80X_mcRun2_asymptotic_2016_miniAODv2_v1"

process.source = cms.Source(
    "PoolSource",
    fileNames=cms.untracked.vstring(

        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/04FB4BAA-3A33-E611-BC64-008CFA197A90.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/08341A1D-2633-E611-B9D5-44A84225CABC.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/0AAB94EE-3E32-E611-BB69-0090FAA57D04.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/0C89BF72-5032-E611-B140-0025905A6138.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/160AF855-3132-E611-BAF1-BCAEC50971F2.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/1ED08BE7-5132-E611-8235-00259021A2AA.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/2059D8D0-1733-E611-AB3D-000F530E47A4.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/321CA628-FF32-E611-994A-B083FED42FE4.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/32B8E028-3C33-E611-988D-0CC47A1DF7F8.root',
        '/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/00000/36A1D1A7-5332-E611-9C38-008CFA1113A0.root',

    )
)

process.njets = cms.EDFilter(
    "GenJetFilter",
    genTag=cms.InputTag('slimmedGenJets'),
    nJets=cms.int32(4),
    minPt=cms.double(20),
    maxEta=cms.double(2.5)
)

process.electron = cms.EDFilter(
    "GenLeptonFilter",
    genTag=cms.InputTag('prunedGenParticles'),
    minPt=cms.double(30),
    maxEta=cms.double(2.5),
    pdgId=cms.int32(11),
    parentPdgId=cms.int32(24)
)

process.muon = cms.EDFilter(
    "GenLeptonFilter",
    genTag=cms.InputTag('prunedGenParticles'),
    minPt=cms.double(25),
    maxEta=cms.double(2.5),
    pdgId=cms.int32(13),
    parentPdgId=cms.int32(24)
)

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.epath = cms.Path(
    process.njets *
    process.electron
)

process.mpath = cms.Path(
    process.njets *
    process.muon
)

process.output = cms.OutputModule(
    "PoolOutputModule",
    fileName=cms.untracked.string('filtered.root'),
    outputCommands=cms.untracked.vstring(['keep *']),
    SelectEvents=cms.untracked.PSet(SelectEvents=cms.vstring('epath', 'mpath')),
    dataset=cms.untracked.PSet(filterName=cms.untracked.string(''))
)

process.end = cms.EndPath(process.output)
