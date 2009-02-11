//*-- AUTHOR : Ilse Koenig
//*-- Created : 21/10/2004

/////////////////////////////////////////////////////////////
//
//  CbmContFact
//
//  Base class of all factories for the parameter containers 
//
/////////////////////////////////////////////////////////////

#include "CbmContFact.h"

#include "CbmRuntimeDb.h"
#include "TObjString.h"

#include <iostream>

using std::cout;

ClassImp(CbmContainer)
ClassImp(CbmContFact)

CbmContainer::CbmContainer() { contexts=0; }
  // Default constructor

CbmContainer::CbmContainer(const char* name, const char* title,
                       const char* defContext)
           : TNamed(name, title) {
  // Constructor
  // Arguments:  name       = name of the corresponding parameter container
  //             title      = title of this parameter container
  //             defContext = default context of this parameter container  
  contexts=new TList;
  addContext(defContext);
  actualContext="";
}

CbmContainer::~CbmContainer() {
  // Destructor deletes the list of accepted contexts
  if (contexts) {
    contexts->Delete();
    delete contexts;
  }
}

void CbmContainer::addContext(const char* name) {
  // Adds a context to the list of accepted contexts 
  TObjString* c=new TObjString(name);
  contexts->Add(c);  
}

Bool_t CbmContainer::setActualContext(const char* c) {
  // The function sets the actual context for the container, if it is in the list of
  // accepted contexts. When the actual context was already set before, it prints a warning
  // and ignores the second setting.
  // The function returns kFALSE, when the context is not in the list. 
  if (contexts->FindObject(c)) {
    if (actualContext.IsNull()) actualContext=c;
    else Warning("addContext",
                 "Actual context of parameter container %s already defined as %s",
                 GetName(),actualContext.Data());
    return kTRUE;
  }
  return kFALSE;
}

const char* CbmContainer::getDefaultContext() {
  // Returns the default context
  return ((TObjString*)contexts->At(0))->String().Data();
}

void CbmContainer::print() {
  // prints the name, title of the container together with the actual context set
  // or all possible contexts, when the actual context was not set
  cout<<fName<<"\t"<<fTitle<<"\n";
  if (!actualContext.IsNull()) cout<<"     actual context:  "<<actualContext<<"\n";
  else {
    TIter next(contexts);
    Int_t i=0;
    TObjString* c;
    cout<<"  all contexts:"<<"\n";
    while ((c=(TObjString*)next())) {
      if (c->String().IsNull()) cout<<"     \"\""; 
      else cout<<"     "<<c->String();
      if (i==0) cout<<"\t default";
      cout<<"\n";
      i++;
    }
  }
}

TString CbmContainer::getConcatName() {
  // Returns the name of the parameter container used in the constructor and the
  // runtime database.
  // When the parameter container supportes different contexts (not only an empty string)
  // and the actual context set is not the default context, the new name of the parameter
  // container is concatinated as
  //      original container name  +  _  +  actualcontext  
  TString cn=fName;
  if (!actualContext.IsNull() && actualContext!=((TObjString*)contexts->At(0))->String()) {
    cn+="_";
    cn+=actualContext;
  }
  return cn;
}

const char* CbmContainer::getContext() {
  // return the actual context, if set, or the default context
  if (!actualContext.IsNull()) return actualContext.Data();
  else return getDefaultContext();
}

//------------------------------------------------------------------

CbmContFact::CbmContFact() : TNamed() {
  // Constructor creates a list to store objects of type CbmContainer
  containers=new TList;
}

CbmContFact::~CbmContFact() {
  // Destructor deletes the container list and its elements
  containers->Delete();
  delete containers;
}

Bool_t CbmContFact::addContext(const char* name) {
  // Set the actual context in all containers, which accept this context
  CbmContainer* c=0;
  Bool_t found=kFALSE;
  TIter next(containers);
  while ((c=(CbmContainer*)next())) {
    if (c->setActualContext(name)) found=kTRUE;
  }
  return found;
}

CbmParSet* CbmContFact::getContainer(const char* name) {
  // Returns the pointer to the parameter container in the runtime database
  // If this parameter container does not yet exit, it calls the function
  // createContainer(CbmContainer*), which is implemented in the derived classes
  // and calls the corresponding constructor. Then the pointer it added in the
  // runtime database.
  CbmContainer* c=(CbmContainer*)(containers->FindObject(name));

  CbmParSet* cont=0;
  if (c) {
    TString cn=c->getConcatName();
    CbmRuntimeDb* rtdb = CbmRuntimeDb::instance();
    if (!(cont=rtdb->findContainer(c->getConcatName().Data()))) {
      if (strlen(c->getActualContext())==0) c->setActualContext(c->getDefaultContext());
      cont=createContainer(c);
      if (cont) rtdb->addContainer(cont);
    }
  }
  return cont;
}

void CbmContFact::print() {
  // Loops over all containers in the list and calls their print() function
  cout<<"---------------------------------------------------------------------------"<<"\n";
  cout<<GetName()<<":  "<<GetTitle()<<"\n";
  cout<<"---------------------------------------------------------------------------"<<"\n";
  CbmContainer* c;
  TIter next(containers);
  while ((c=(CbmContainer*)next())) c->print();
}  