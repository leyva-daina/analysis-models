#ifndef Analysis_Models_RooExpPol_h
#define Analysis_Models_RooExpPol_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "RooPolynomial.h"
#include "RooRealVar.h"


namespace analysis {
  namespace models {

    class RooExpPol : public RooAbsPdf {
    public:
      RooExpPol(const char *name, const char *title,
                             RooAbsReal& x, const RooArgList& coefList, RooAbsReal& x0, Int_t lowestOrder=1) ;
      RooExpPol(const RooExpPol& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;

    protected:
      RooRealProxy _x;
      RooListProxy _coefList ;
      RooRealProxy _x0 ;
      Int_t _lowestOrder = 1;
      mutable std::vector<double> _wksp; //! do not persist

      double evaluate() const;


    private:
      ClassDef(RooExpPol,1)
    };

  }
}

#endif  // Analysis_Models_RooExpGausExp_h
