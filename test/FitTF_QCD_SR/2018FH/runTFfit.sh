# run with source runTFfit.sh  > runTFfit_Chebychev.log 

# Define the range of functname values
#functnames=("Chebychev" "Power" "ExponentialPolynomial" "ExponentialLogarithm")
functnames=("Chebychev" )

# Iterate through functnames and call the FitTF.C script
for functname in "${functnames[@]}"; do
    root -l -b -q -f "FitTF.C(true, 270, 560, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(true, 270, 560, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(true, 270, 560, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "FitTF.C(true, 320, 800, 10, 1, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(true, 320, 800, 10, 2, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(true, 320, 800, 10, 3, \"$functname\", 0)"

    root -l -b -q -f "FitTF.C(true, 370, 1270, 20, 1, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(true, 370, 1270, 20, 2, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(true, 370, 1270, 20, 3, \"$functname\", 0)"

    root -l -b -q -f "FitTF.C(true, 500, 2000, 50, 1, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(true, 500, 2000, 50, 2, \"$functname\", 0)"
    root -l -b -q -f "FitTF.C(true, 500, 2000, 50, 3, \"$functname\", 0)"

done

# root -l -b -q -f "FitTF.C(true, 500, 2000, 50, 1, \"ExponentialLogarithm\", 0)" 
# root -l -b -q -f "FitTF.C(true, 270, 560, 10, 3, \"Power\", 0)" 
# root -l -b -q -f "FitTF.C(true, 370, 1270, 20, 2, \"ExponentialPolynomial\", 0)" 
# root -l -b -q -f "FitTF.C(true, 370, 1270, 20, 3, \"ExponentialPolynomial\", 0)" 