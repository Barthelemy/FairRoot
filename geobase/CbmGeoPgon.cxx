//*-- AUTHOR : Ilse Koenig
//*-- Modified : 11/11/2003 by Ilse Koenig
//*-- Modified : 26/11/2001 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
//
// CbmGeoPgon
//
// class for the GEANT shape PGON
// 
// The size of a PGON is defined by a variable number of 'points'.
//   point 0:   number of planes perpendicular to the z axis where the
//              dimension of the section is given;
//   point 1:   azimutal angle phi at which the volume begins,
//              opening angle dphi of the volume,
//              number of sides of the cross section between the phi-limits;
//   point 2ff: z coordinate of the section,
//              inner radius at position z,
//              outer radius at position z;
//
// The intrinsic coordinate system of a PGON, which sits in the CAVE and is
// not rotated, is identical with the laboratory system.
//
///////////////////////////////////////////////////////////////////////////////

#include "CbmGeoPgon.h"

#include "CbmGeoVolume.h"
#include "CbmGeoVector.h"

#include "TArrayD.h"

ClassImp(CbmGeoPgon)

CbmGeoPgon::CbmGeoPgon() {
  // constructor
  fName="PGON";
  nPoints=0;
  nParam=0;
}


CbmGeoPgon::~CbmGeoPgon() {
  // default destructor
  if (param) {
    delete param;
    param=0;
  }
  if (center) {
    delete center;
    center=0;
  }
  if (position) {
    delete position;
    position=0;
  }
}


Int_t CbmGeoPgon::readPoints(fstream* pFile,CbmGeoVolume* volu) {
  // reads the 'points' decribed above from ascii file and stores them in the
  // array 'points' of the volume
  // returns the number of points
  if (!pFile) return 0;
  Double_t x,y,z;
  const Int_t maxbuf=155;
  Text_t buf[maxbuf];
  pFile->getline(buf,maxbuf);
  Int_t n;
  sscanf(buf,"%i",&n);
  if (n<=0) return 0;
  nPoints=n+2;
  if (volu->getNumPoints()!=nPoints) volu->createPoints(nPoints);
  volu->setPoint(0,(Double_t)n,0.0,0.0);
  for(Int_t i=1;i<nPoints;i++) {
    pFile->getline(buf,maxbuf);
    sscanf(buf,"%lf%lf%lf",&x,&y,&z);
    volu->setPoint(i,x,y,z);
  }
  return nPoints;
}


Bool_t CbmGeoPgon::writePoints(fstream* pFile,CbmGeoVolume* volu) {
  // writes the 'points' decribed above to ascii file
  if (!pFile) return kFALSE;  
  Text_t buf[155];
  for(Int_t i=0;i<volu->getNumPoints();i++) {
    CbmGeoVector& v=*(volu->getPoint(i));
    if (i!=0) sprintf(buf,"%9.3f%10.3f%10.3f\n",v(0),v(1),v(2));
    else sprintf(buf,"%3i\n",(Int_t)v(0));
    pFile->write(buf,strlen(buf));
  }
  return kTRUE;
}


void CbmGeoPgon::printPoints(CbmGeoVolume* volu) {
  // prints volume points to screen
  for(Int_t i=0;i<volu->getNumPoints();i++) {
    CbmGeoVector& v=*(volu->getPoint(i));
    if (i!=0) printf("%9.3f%10.3f%10.3f\n",v(0),v(1),v(2));
    else printf("%3i\n",(Int_t)v(0));
  }
}


TArrayD* CbmGeoPgon::calcVoluParam(CbmGeoVolume* volu) {
  // calculates the parameters needed to create the shape PGON 
  Double_t fac=10.;
  nPoints=volu->getNumPoints();
  nParam=nPoints*3-2;
  if (param && param->GetSize()!=nParam) {
    delete param;
    param=0;
  }
  if (!param) param=new TArrayD(nParam);
  CbmGeoVector& v1=*(volu->getPoint(1));
  Int_t k=0;
  param->AddAt(v1(0),k++);
  param->AddAt(v1(1),k++);
  param->AddAt(v1(2),k++);
  param->AddAt((nPoints-2),k++);
  for(Int_t i=2;i<nPoints;i++) {
    CbmGeoVector& v=*(volu->getPoint(i));
    param->AddAt(v(0)/fac,k++);
    param->AddAt(v(1)/fac,k++);
    param->AddAt(v(2)/fac,k++);
  }
  return param;
} 


void CbmGeoPgon::calcVoluPosition(CbmGeoVolume*,
          const CbmGeoTransform& dTC,const CbmGeoTransform& mTR) {
  // calls the function posInMother(...) to calculate the position of the
  // volume in its mother 
  center->clear();
  posInMother(dTC,mTR);
}