#include <string>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Common/interface/RefToPtr.h"

namespace pat {
    class PATSecondaryVertexSlimmer : public edm::EDProducer {
        public:
            explicit PATSecondaryVertexSlimmer(const edm::ParameterSet&);
            ~PATSecondaryVertexSlimmer();

            virtual void produce(edm::Event&, const edm::EventSetup&);
        private:
            edm::EDGetTokenT<std::vector<reco::Vertex> > src_;
            edm::EDGetTokenT<std::vector<reco::Track> > tracks_;
            edm::EDGetTokenT<edm::Association<pat::PackedCandidateCollection> > map_;
            edm::EDGetTokenT<edm::Association<pat::PackedCandidateCollection> > map2_;
    };
}

pat::PATSecondaryVertexSlimmer::PATSecondaryVertexSlimmer(const edm::ParameterSet& iConfig) :
    src_(consumes<std::vector<reco::Vertex> >(iConfig.getParameter<edm::InputTag>("src"))),
    tracks_(consumes<std::vector<reco::Track> >(iConfig.getParameter<edm::InputTag>("generalTracks"))),
    map_(consumes<edm::Association<pat::PackedCandidateCollection> >(iConfig.getParameter<edm::InputTag>("packedPFCandidates"))),
    map2_(consumes<edm::Association<pat::PackedCandidateCollection> >(iConfig.getParameter<edm::InputTag>("lostTracksCandidates")))
{
  produces< reco::VertexCompositePtrCandidateCollection >();
}

pat::PATSecondaryVertexSlimmer::~PATSecondaryVertexSlimmer() {}

void pat::PATSecondaryVertexSlimmer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    edm::Handle<std::vector<reco::Vertex> > vertices;
    iEvent.getByToken(src_, vertices);
    std::auto_ptr<reco::VertexCompositePtrCandidateCollection > outPtr(new reco::VertexCompositePtrCandidateCollection);
 
    edm::Handle<edm::Association<pat::PackedCandidateCollection> > pf2pc;
    iEvent.getByToken(map_,pf2pc);
    edm::Handle<edm::Association<pat::PackedCandidateCollection> > pf2pc2;
    iEvent.getByToken(map2_,pf2pc2);

    edm::Handle<std::vector<reco::Track> > tracks;
    iEvent.getByToken(tracks_, tracks);

    outPtr->reserve(vertices->size());
    for (unsigned int i = 0, n = vertices->size(); i < n; ++i) { 
	    const reco::Vertex &v = (*vertices)[i];
	    outPtr->push_back(reco::VertexCompositePtrCandidate(0,v.p4(),v.position(), v.error(), v.chi2(), v.ndof()));

	    for(reco::Vertex::trackRef_iterator  it=v.tracks_begin(); it != v.tracks_end(); it++) {
		    if(v.trackWeight(*it)>0.5) {
          reco::TrackRef myRef(tracks, (*it).key());
			    if((*pf2pc)[myRef].isNonnull() && (*pf2pc)[myRef]->numberOfHits() > 0) {
				    outPtr->back().addDaughter(reco::CandidatePtr(edm::refToPtr((*pf2pc)[myRef]) ));
			    }
			    else {
				    if((*pf2pc2)[myRef].isNonnull()) {
					    outPtr->back().addDaughter(reco::CandidatePtr(edm::refToPtr((*pf2pc2)[myRef]) ));
				    }	
				    else { std::cout << "HELPME" << std::endl;}	
			    }
		    }
	    }
    }

    iEvent.put(outPtr);
}

using pat::PATSecondaryVertexSlimmer;
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PATSecondaryVertexSlimmer);
