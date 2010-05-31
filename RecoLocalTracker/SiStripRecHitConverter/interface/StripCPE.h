#ifndef RecoLocalTracker_SiStripRecHitConverter_StripCPE_H
#define RecoLocalTracker_SiStripRecHitConverter_StripCPE_H

#include "RecoLocalTracker/ClusterParameterEstimator/interface/StripClusterParameterEstimator.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "CondFormats/SiStripObjects/interface/SiStripLorentzAngle.h"
#include "CondFormats/SiStripObjects/interface/SiStripConfObject.h"
#include "CondFormats/SiStripObjects/interface/SiStripLatency.h"
#include <ext/hash_map>
class StripTopology;

class StripCPE : public StripClusterParameterEstimator 
{
public:

  StripClusterParameterEstimator::LocalValues localParameters( const SiStripCluster&) const; 
  StripClusterParameterEstimator::LocalValues localParameters( const SiStripCluster& cl, const GeomDetUnit&) const {return localParameters(cl);}
  
  StripCPE( edm::ParameterSet & conf, 
	    const MagneticField*, 
	    const TrackerGeometry*, 
	    const SiStripLorentzAngle*,
	    const SiStripConfObject*,
	    const SiStripLatency*);    
  LocalVector driftDirection(const StripGeomDetUnit* det) const;
  void clearCache() {m_Params.clear();}

 protected:  

  edm::ParameterSet conf_;
  const TrackerGeometry * geom_;
  const MagneticField * magfield_ ;
  const SiStripLorentzAngle* LorentzAngleMap_;

  struct Param {
    Param() : topology(0) {}
    StripTopology const * topology;
    LocalVector drift;
    float thickness, pitch_rel_err2, maxLength, lfp,lbp;
    int nstrips;
    SiStripDetId::SubDetector subdet;
    float coveredStrips(const LocalVector&, const LocalPoint&) const;
  };
  Param const & param(const uint32_t detid) const;

private:

  Param & fillParam(Param & p, const GeomDetUnit *  det);
  typedef  __gnu_cxx::hash_map< unsigned int, Param> Params;  
  Params m_Params;
  std::vector<std::pair<float,float> > late;

};
#endif
