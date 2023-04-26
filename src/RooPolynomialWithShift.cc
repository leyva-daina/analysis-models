#include <cmath>
#include "Riostream.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "TMath.h"
#include "Analysis/Models/interface/RooPolynomialWithShift.h"

using namespace analysis::models;


ClassImp(RooPolynomialWithShift)


RooPolynomialWithShift::RooPolynomialWithShift(const char* name, const char* title,
                                               RooAbsReal& x, const RooArgList& coefList, RooAbsReal& x0, Int_t lowestOrder) :
  RooAbsPdf(name, title),
  _x("x", "x", this, x),
  _coefList("coefList","List of coefficients",this),
  _x0("x0", "x0", this, x0),
  _lowestOrder(lowestOrder) {

  // Check lowest order
  if (_lowestOrder<0) {
    coutE(InputArguments) << "RooPolynomial::ctor(" << GetName()
           << ") WARNING: lowestOrder must be >=0, setting value to 0" << std::endl ;
    _lowestOrder=0 ;
  }
 
  RooFIter coefIter = coefList.fwdIterator() ;
  RooAbsArg* coef ;
  while((coef = (RooAbsArg*)coefIter.next())) {
    if (!dynamic_cast<RooAbsReal*>(coef)) {
      coutE(InputArguments) << "RooPolynomial::ctor(" << GetName() << ") ERROR: coefficient " << coef->GetName()
             << " is not of type RooAbsReal" << std::endl ;
      R__ASSERT(0) ;
    }
    _coefList.add(*coef) ;
  }  
}

RooPolynomialWithShift::RooPolynomialWithShift(const RooPolynomialWithShift& other, const char* name) :
  RooAbsPdf(other, name),
  _x("x", this, other._x),
  _coefList("coefList",this,other._coefList),
  _x0("x0", this, other._x0),
  _lowestOrder(other._lowestOrder) {
}


TObject* RooPolynomialWithShift::clone(const char* newname) const {
  return new RooPolynomialWithShift(*this, newname);
}


double RooPolynomialWithShift::evaluate() const {
  // Calculate and return value of polynomial
 
  const unsigned sz = _coefList.getSize();
  const int lowestOrder = _lowestOrder;
  if (!sz) return lowestOrder ? 1. : 0.;
  _wksp.clear();
  _wksp.reserve(sz);
  {
    const RooArgSet* nset = _coefList.nset();
    RooFIter it = _coefList.fwdIterator();
    RooAbsReal* c;
    while ((c = (RooAbsReal*) it.next())) _wksp.push_back(c->getVal(nset));
  }
  const Double_t x = _x-_x0;
  Double_t retVal = _wksp[sz - 1];
  for (unsigned i = sz - 1; i--; ) retVal = _wksp[i] + x * retVal;
  return retVal * std::pow(x, lowestOrder) + (lowestOrder ? 1.0 : 0.0);
}



