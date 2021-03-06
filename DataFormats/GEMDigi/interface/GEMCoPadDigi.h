#ifndef GEMDigi_GEMCoPadDigi_h
#define GEMDigi_GEMCoPadDigi_h

/** \class GEMCoPadDigi
 *
 * Digi for GEM-CSC trigger copads
 *  
 * \author Sven Dildick
 *
 */

#include <DataFormats/GEMDigi/interface/GEMPadDigi.h>
#include <boost/cstdint.hpp>
#include <iosfwd>

class GEMCoPadDigi{

public:
  explicit GEMCoPadDigi(GEMPadDigi pad1, GEMPadDigi pad2);
  GEMCoPadDigi();

  bool operator==(const GEMCoPadDigi& digi) const;
  bool operator!=(const GEMCoPadDigi& digi) const;

  int pad(int l) const;
  int bx(int l) const;

  GEMPadDigi first() const {return first_;}
  GEMPadDigi second() const {return second_;}

  void print() const;

private:
  GEMPadDigi first_;
  GEMPadDigi second_;
};

std::ostream & operator<<(std::ostream & o, const GEMCoPadDigi& digi);

#endif

