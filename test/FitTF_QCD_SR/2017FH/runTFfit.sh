# run with source runTFfit.sh  > runTFfit_Chebychev.log 

# Define the range of functname values
#functnames=("Chebychev" "Bernstein" "Power")
functnames=("Chebychev" )

# Iterate through functnames and call the polynomial_fit.C script
for functname in "${functnames[@]}"; do
    root -l -b -q -f "polynomial_fit.C(true, 240, 560, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 240, 560, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 240, 560, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "polynomial_fit.C(true, 280, 800, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 280, 800, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 280, 800, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "polynomial_fit.C(true, 400, 1300, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 400, 1300, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 400, 1300, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "polynomial_fit.C(true, 600, 2000, 50, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 600, 2000, 50, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 600, 2000, 50, 3, \"$functname\", 0)"

done

