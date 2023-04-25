#ifndef Analysis_Models_RooPoly2WithShift_h
#define Analysis_Models_RooPoly2WithShift_h 1

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"
#include "RooPolynomial.h"


namespace analysis {
  namespace models {

    class RooPoly2WithShift : public RooAbsPdf {
    public:
      inline RooPoly2WithShift() = default;
      RooPoly2WithShift(const char *name, const char *title,
                    RooAbsReal& x,
                    RooAbsReal& linear,
                    RooAbsReal& quadratic,
                    RooAbsReal& x0);
      RooPoly2WithShift(const RooPoly2WithShift& other, const char* name=0) ;
      virtual TObject* clone(const char* newname) const;
      inline virtual ~RooPoly2WithShift() = default;

    protected:
      double evaluate() const ;

      RooRealProxy x_ ;
      RooRealProxy linear_ ;
      RooRealProxy quadratic_ ;
      RooRealProxy x0_ ;

    private:
      ClassDef(RooPoly2WithShift,1)
    };

  }
}

#endif  // Analysis_Models_RooExpGausExp_h
