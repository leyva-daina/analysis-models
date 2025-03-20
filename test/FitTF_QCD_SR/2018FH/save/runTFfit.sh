# run with source runTFfit.sh  > runTFfit_Chebychev.log 

# Define the range of functname values
#functnames=("Chebychev" "Bernstein" "Power")
functnames=("Power" )

# Iterate through functnames and call the polynomial_fit.C script
for functname in "${functnames[@]}"; do
    root -l -b -q -f "polynomial_fit.C(true, 270, 560, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 270, 560, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 270, 560, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "polynomial_fit.C(true, 320, 800, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 320, 800, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 320, 800, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "polynomial_fit.C(true, 370, 1270, 20, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 370, 1270, 20, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 370, 1270, 20, 3, \"$functname\", 0)"

    root -l -b -q -f "polynomial_fit.C(true, 500, 2000, 50, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 500, 2000, 50, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 500, 2000, 50, 3, \"$functname\", 0)"

done

