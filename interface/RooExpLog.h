#ifndef Analysis_Models_RooExpLog_h
#define Analysis_Models_RooExpLog_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "RooPolynomial.h"
#include "RooRealVar.h"


namespace analysis {
  namespace models {

    class RooExpLog : public RooAbsPdf {
    public:
      inline RooExpLog() = default;
      RooExpLog(const char *name, const char *title,
                             RooAbsReal& x,
                             RooAbsReal& steepness,
                             RooAbsReal& shift) ;
      RooExpLog(const RooExpLog& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
    //Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName);
    //Double_t analyticalIntegral(Int_t code, const char* rangeName);
    protected:
      RooRealProxy x_ ;
      RooRealProxy steepness_ ;
      RooRealProxy shift_ ;


      double evaluate() const;


    private:
      ClassDef(RooExpLog,1)
    };

  }
}

#endif  // Analysis_Models_RooExpGausExp_h
