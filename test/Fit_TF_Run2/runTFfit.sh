# run with source runTFfit.sh  > runTFfit_Chebychev.log 

# Define the range of functname values
#functnames=("Chebychev" "Power" "ExponentialPolynomial" "ExponentialLogarithm")
functnames=("Chebychev" )

# Iterate through functnames and call the FitTF.C script
for functname in "${functnames[@]}"; do
    root -l -b -q -f "FitTF.C(false, 270, 560, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(false, 270, 560, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(false, 270, 560, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "FitTF.C(false, 320, 800, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(false, 320, 800, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(false, 320, 800, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "FitTF.C(false, 370, 1270, 20, 1, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(false, 370, 1270, 20, 2, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(false, 370, 1270, 20, 3, \"$functname\", 0)"

    root -l -b -q -f "FitTF.C(false, 500, 2000, 50, 1, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(false, 500, 2000, 50, 2, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(false, 500, 2000, 50, 3, \"$functname\", 0)"

done
