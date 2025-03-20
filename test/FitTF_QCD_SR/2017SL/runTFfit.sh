# run with source runTFfit.sh  > runTFfit_Chebychev.log 

# Define the range of functname values
#functnames=("Chebychev" "Bernstein" "Power")
functnames=("Chebychev" )

# Iterate through functnames and call the polynomial_fit.C script
for functname in "${functnames[@]}"; do
    root -l -b -q -f "polynomial_fit.C(true, 120, 300, 5, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 120, 300, 5, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 120, 300, 5, 3, \"$functname\", 0)"

    root -l -b -q -f "polynomial_fit.C(true, 180, 460, 5, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 180, 460, 5, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 180, 460, 5, 3, \"$functname\", 0)"

    root -l -b -q -f "polynomial_fit.C(true, 240, 800, 5, 1, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 240, 800, 5, 2, \"$functname\", 0)"
    root -l -b -q -f "polynomial_fit.C(true, 240, 800, 5, 3, \"$functname\", 0)"

done

