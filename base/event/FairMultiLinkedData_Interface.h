/**
 * FairMultiLinkedData_Interface.h
 *
 * \date Dec 7, 2009
 * \author T.Stockmanns <t.stockmanns@fz-juelich.de>
 */

#ifndef FairMultiLinkedData_Interface_H_
#define FairMultiLinkedData_Interface_H_

#include "TObject.h"                    // for TObject

#include "FairLink.h"                   // for FairLink
#include "FairMultiLinkedData.h"
#include "FairRootManager.h"

#include "Riosfwd.h"                    // for ostream
#include "Rtypes.h"                     // for Int_t, Bool_t, kFALSE, etc
#include "TString.h"                    // for TString
#include "TRef.h"

#include <iostream>                     // for operator<<, ostream, cout
#include <set>                          // for set
#include <vector>                       // for vector


class FairMultiLinkedData_Interface : public  TObject
{
  public:
    FairMultiLinkedData_Interface();///< Default constructor
    FairMultiLinkedData_Interface(FairMultiLinkedData& links, Bool_t persistanceCheck = kTRUE);///< Constructor
    FairMultiLinkedData_Interface(TString dataType, std::vector<Int_t> links, Int_t fileId = -1, Int_t evtId = -1,Bool_t persistanceCheck = kTRUE, Bool_t bypass = kFALSE, Float_t mult = 1.0);///< Constructor
    FairMultiLinkedData_Interface(Int_t dataType, std::vector<Int_t> links, Int_t fileId = -1, Int_t evtId = -1, Bool_t persistanceCheck = kTRUE, Bool_t bypass = kFALSE, Float_t mult = 1.0);///< Constructor
    FairMultiLinkedData_Interface(const FairMultiLinkedData_Interface& toCopy);

    virtual ~FairMultiLinkedData_Interface() {
    	delete(fLink);
    };

    FairMultiLinkedData_Interface& operator=(const FairMultiLinkedData_Interface& rhs);

    virtual std::set<FairLink>  GetLinks() const;           		///< returns stored links as FairLinks
    virtual Int_t           	GetNLinks() const;                	///< returns the number of stored links
    virtual FairLink        	GetLink(Int_t pos) const;         	///< returns the FairLink at the given position
    virtual FairMultiLinkedData GetLinksWithType(Int_t type) const; ///< returns all FairLinks with the corresponding type
    virtual FairLink            GetEntryNr() const;
    virtual FairMultiLinkedData* 		GetPointerToLinks() const {	return fLink;}

    virtual std::vector<FairLink> GetSortedMCTracks();

    virtual void SetLinks(FairMultiLinkedData links);           ///< Sets the links as vector of FairLink
    virtual void SetLink(FairLink link);      					///< Sets the Links with a single FairLink
    virtual void SetInsertHistory(Bool_t val);
    virtual void SetEntryNr(FairLink val);
    virtual void SetPointerToLinks(FairMultiLinkedData* links) {fLink = links;}

    virtual void AddLinks(FairMultiLinkedData links, Float_t mult = 1.0);		///< Adds a List of FairLinks (FairMultiLinkedData_Interface) to fLinks
    virtual void AddLink(FairLink link);      									///< Adds a FairLink link at the end of fLinks. If multi is kTRUE a link is allowed more than once otherwise
    virtual void AddInterfaceData(FairMultiLinkedData_Interface* data);



    virtual void ResetLinks();


    std::ostream& Print(std::ostream& out = std::cout) const {

		if (GetPointerToLinks() != 0)
			GetPointerToLinks()->Print(out);
		return out;
    }                                                     ///< Output

    friend std::ostream& operator<< (std::ostream& out, FairMultiLinkedData_Interface& data) {
      data.Print(out);
      return out;
    }                                                     ///< Output

  protected:

    Int_t fVerbose; //!
    Bool_t fInsertHistory; //!
    FairMultiLinkedData* fLink;

    FairMultiLinkedData* CreateFairMultiLinkedData();

    ClassDef(FairMultiLinkedData_Interface, 5);
};

/**\fn virtual void FairMultiLinkedData_Interface::SetLinks(Int_t type, std::vector<Int_t> links)
 * \param type as Int_t gives one type of source data for all indices
 * \param links as vector<Int_t> gives the vector of indices at which position in the TClonesArray the source data is stored
 */

/**\fn virtual void FairMultiLinkedData_Interface::SetLinks(std::vector<Int_t> type, std::vector<Int_t> links)
 * \param type as vector<Int_t> gives the type of source data (TClonesArray)
 * \param links as vector<Int_t> gives the vector of indices at which position in the TClonesArray the source data is stored
 */


#endif /* FairMultiLinkedData_Interface_H_ */
