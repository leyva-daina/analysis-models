#root -f "polynomial_fit.C(true,x0, xmin, xmax, binning, degree)"
root -l -b -q -f "polynomial_fit.C(true, 270, 560, 10, 1)"
root -l -b -q -f "polynomial_fit.C(true, 320, 800, 10, 1)"
root -l -b -q -f "polynomial_fit.C(true, 370, 1270, 20, 1)"
root -l -b -q -f "polynomial_fit.C(true, 500, 2000, 50, 1)"


root -l -b -q -f "polynomial_fit.C(true, 270, 560, 10, 2)"
root -l -b -q -f "polynomial_fit.C(true, 320, 800, 10, 2)"
root -l -b -q -f "polynomial_fit.C(true, 370, 1270, 20, 2)"
root -l -b -q -f "polynomial_fit.C(true, 500, 2000, 50, 2)"