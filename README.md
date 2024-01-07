# Analysis/Models Model-Interpretation branch

** This is an update on the MSSM Hbb Analysis-Models mini-framework (see [analysis-models](https://github.com/desy-cms/analysis-models)) to include MSSM and 2HDM model interpretations. The latter is an adaptation of the codes from [ShevchenkoRostyslav](https://github.com/ShevchenkoRostyslav/analysis/tree/myCodes/Analysis/MssmHbb/macros/limits) and [pasmuss](https://github.com/pasmuss/analysis-combine/tree/master/AnalysisLimits/PlottingTools).

* [Installation](#Installation)
* [Usage](#Usage)
   * [MSSM](#Interpretations in MSSM scenarios)

## Installation

In a CMSSW workarea

```bash
cd CMSSW_X_Y_Z/src
cmsenv
git clone https://github.com/leyva-daina/analysis-models.git Analysis/Models
scram b -j4
hash -r
```

## Usage
See main [analysis-models](https://github.com/desy-cms/analysis-models) documentation for instructions on parameterizations of background for MSSM Hbb analysis.

See below instructions to generate model interpretations on MSSM and 2HDM scenarios:

### Interpretations in MSSM scenarios
13 TeV cross sections are provided by the [LHCHWG](https://twiki.cern.ch/twiki/bin/view/LHCPhysics/LHCHWGMSSMNeutral#ROOT_histograms_2018_and_beyond) in rootfiles, one per benchmark scenario.
To translate your model-independant limits in the cross-section times BR to the tanB vs. mA parameter space use macro [plotTanBetaLimits.cpp]().

### Interpretations in 2HDM scenarios
Cross sections for type 2 and flipped scenarios, obtained with the SUSHi framework at 13 TeV, can be found in /afs/desy.de/user/l/leyvaped/public/2HDM/
To translate your model-independant limits in the cross-section times BR to the tanB vs. mA (tanB vs cos(b-a)) parameter space use macro [Plot2HDM_mA_tanB.cpp]() ([Plot2HDM_cosBA_tanB.cpp]()).
