# Analysis/Models

**Mini-framework to study possible parameterizations of background and signal**

* [Installation](#Installation)
* [Usage](#Usage)
   * [Example](#Example)

## Installation

In a CMSSW workarea

```bash
cd CMSSW_X_Y_Z/src
cmsenv
git clone https://github.com/desy-cms/analysis-models.git Analysis/Models
scram b -j4
hash -r
```

## Usage

After the installation of the package you should have the main executable called `FitModel`. The available options can be found by running 

```bash
FitModel -h
```

resulting in 

```
Allowed arguments:

Optional arguments:
  -h [ --help ]                         Produce help message.
  -v [ --verbose ] arg (=0)             More verbose output.
  -p [ --profile ]                      Create profile likelihoods of the fit 
                                        parameters.
  -l [ --list_parameters ]              List parameters of the chosen fit 
                                        models and exit.
  -i [ --input_file ] arg               ROOT file from which input histograms 
                                        are retrieved.
  -t [ --input_tree_file ] arg          ROOT file from which input tree is 
                                        retrieved. If this parameter is not 
                                        given a binned fit is performed using 
                                        'input_file'.
  --input_data arg                      Name of data leaf in the input tree
  --input_weight arg                    Name of weight leaf in the input tree
  --input_tree arg                      Name of the input tree
  -o [ --output_directory ] arg (=fitmodel_output/)
                                        Directory where the output is stored.
  --fit_min arg                         Lower bound of the fit range.
  --fit_max arg                         Upper bound of the fit range.
  --nbins arg                           Number of bins in the fit range.
  --weighted                            If events are weighted
  -m [ --modify_param ] arg             Modify parameters as follows: "name: 
                                        [start=<value>,] [min=<value>,] 
                                        [max=<value>,] [constant,] [floating]"

Required arguments:
  --type arg                            signal or background
  --model arg                           Name of the model (novosibirsk, 
                                        novopsprod, novoeffprod, 
                                        novoefffixprod, novopshighMpol4, 
                                        crystalball, crystalpsprod, 
                                        crystaleffprod, cbeffprod, expeffprod, 
                                        doublecb, dijetv1, dijetv1psprod, 
                                        dijetv2, dijetv2psprod, dijetv3, 
                                        dijetv3effprod, dijetv3logprod, 
                                        dijetv4, dijetv4logprod, expgausexp, 
                                        gausexp, doublegausexp, triplegausexp, 
                                        gausexppsprod, gausexpeffprod, 
                                        doublegausexpeffprod, expbwexp, bukin, 
                                        bukinpsprod, bernstein, chebychev, 
                                        berneffprod, bernefffixprod, 
                                        bernpsprod, chebeffprod, breitwigner, 
                                        mynovosibirsk, mynovopsprod, 
                                        extnovosibirsk, extnovopsprod, 
                                        extnovoeffprod, extnovoefffixprod, 
                                        extnovologprod, extnovoextlogprod, 
                                        extnovohypertanprod, relbreitwigner, 
                                        doublegausexp, quadgausexp, 
                                        supernovosibirsk, supernovoeffprod, 
                                        superdijet, superdijeteffprod, 
                                        superdijetlinearprod).
```

### Example

Whether this is a signal or a background model, it has to be defined via the option `--type`.

The name of the input tree, the varables corresponding to the data to be fitted and the event weights have to be given via parameters `--input_tree`, `--input_data` and `--input_weight`.

For a `--type background` model, fitting an extended novosibirsk function times a gaussian error function (extnovoeffprod) in the range `--fit_min 100` to `--fit_max 500` the command should be

```bash
FitModel -t background_data.root \
--input_tree mssmhbb \
--input_data mbb \
--input_weight weight \
-o results/background_extnovoeff \
--model extnovoeffprod \
--fit_min 100 \
--fit_max 500 \
--nbins 80 \
-m "peak: start=150, min=50, max=300" \
-m "width: start=54" \
-m "tail: start=-0.4" \
-m "par4: start=-0.0006" \
-m "turnon_novoeff: start=90, min=10, max=650" \
-m "slope_novoeff: start=0.02" \
--type background
```

where the number of bins of the output histogram is `--nbins 80`, and the initial parameters of the function and validity regions of the parameters are defined by the options `-m "par: start=x", min=y, max=z"`.

The results are stored in the `results/background_extnovoeff` directory defined by the `-o [ --output_directory ]` option
