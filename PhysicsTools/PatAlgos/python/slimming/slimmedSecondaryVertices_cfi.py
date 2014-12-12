import FWCore.ParameterSet.Config as cms

slimmedSecondaryVertices = cms.EDProducer("PATSecondaryVertexSlimmer",
    src = cms.InputTag("inclusiveSecondaryVertices"),
    generalTracks = cms.InputTag("generalTracks"),
    packedPFCandidates = cms.InputTag("packedPFCandidates"),
    lostTracksCandidates = cms.InputTag("lostTracks"),
)
