#include <cmath>
#include "Riostream.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
#include "TMath.h"
#include "Analysis/Models/interface/RooPoly2WithShift.h"


using namespace analysis::models;


ClassImp(RooPoly2WithShift)


RooPoly2WithShift::RooPoly2WithShift(const char *name, const char *title,
                             RooAbsReal& x,
                             RooAbsReal& linear,
                             RooAbsReal& quadratic,
                             RooAbsReal& x0) :
  RooAbsPdf(name, title),
  x_("x", "x", this, x),
  linear_("linear", "linear", this, linear),
  quadratic_("quadratic", "quadratic", this, quadratic),
  x0_("x0", "x0", this, x0) {
}


RooPoly2WithShift::RooPoly2WithShift(const RooPoly2WithShift& other, const char* name) :
  RooAbsPdf(other, name),
  x_("x", this, other.x_),
  linear_("linear", this, other.linear_),
  quadratic_("quadratic", this, other.quadratic_),
  x0_("x0", this, other.x0_) {
}


TObject* RooPoly2WithShift::clone(const char* newname) const {
  return new RooPoly2WithShift(*this, newname);
}


double RooPoly2WithShift::evaluate() const {
	double result = 1+linear_*(x_-x0_)+quadratic_*(x_-x0_)*(x_-x0_);

	return result;
}



