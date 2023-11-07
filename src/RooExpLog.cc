#include <cmath>
#include "Riostream.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "TMath.h"
#include "Analysis/Models/interface/RooExpLog.h"


using namespace analysis::models;


ClassImp(RooExpLog)


RooExpLog::RooExpLog(const char *name, const char *title,
                             RooAbsReal& x,
                             RooAbsReal& steepness,
                             RooAbsReal& shift) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  steepness_("steepness", "steepness", this, steepness),
  shift_("shift", "shift", this, shift) {
}


RooExpLog::RooExpLog(const RooExpLog& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  steepness_("steepness", this, other.steepness_),
  shift_("shift", this, other.shift_) {
}


TObject* RooExpLog::clone(const char* newname) const {
  return new RooExpLog(*this, newname);
}


double RooExpLog::evaluate() const {
	double exponent = (steepness_*(x_)+shift_)*log(x_);
  double result = exp(exponent);
	return result;
}



