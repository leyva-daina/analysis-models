/*
 * plotTanBetaLimits.cpp
 *
 *  Created on: 9 Mar 2017
 *  Author: shevchen
 *
 *  Macro to calculate and plot MSSM limits
 *  from the output of the combine tool.
 *  Comparison of 13 TeV and 7+8 TeV combination
 *  can be performed as well.
 */

#include <iostream>
#include <string>
#include <vector>

//ROOT includes
#include <TFile.h>

#include "Analysis/Models/interface/LHCXSGLimits.h"
#include "Analysis/Models/interface/LHCXSGScenarious.h"
#include "Analysis/Models/interface/Limit.h"
#include "Analysis/Models/interface/namespace_mssmhbb.h"

HbbStyle style;

using namespace std;
using namespace analysis::mssmhbb;
using namespace mssmhbb;

std::string getBanchmarkPath(AvailableScenarios scenario);
void setFolderNamesAccordingToBlinding(string& input, string & output);

int plotTanBetaLimits_theounc(){
  cout<<endl<<"Use this macro to compute the up (down) uncertainties coming from the pdf definition, alpha_s measurements and the uncertainty on factorization and renormalizaiton scale"<<endl;
  cout<<"This is handled in a rather sub-optimal way, only to facilitate providing results for Run 2 Legacy analysis promptly"<<endl;
  cout<<endl<<"To get the corresponding up(down) systematics, go to the file mssm_xs_tools.h and edit fom L435 according to which uncertainty you want to provide, taken from L477 forward"<<endl;
  cout<<"Remember to compile the changes before running (and change it back to normal when done)"<<endl;
  
  bool compareto2016 = true;
  
  PublicationStatus status = WORKINPROGRESS;
  style.setTDRstyle(status);
  //Prefix to the output
  string output_prefix = "13TeV_2017FH";
  

  //paths with results
  //string path2016_solo 	 = "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Combine/AnalysisLimits/ActualLimits";
  //string path2016_combined = "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Combine/AnalysisLimits/ActualLimits/Hbb_limits_full_alluncert_theo";

  string path2018_solo 	 = "/nfs/dust/cms/user/leyvaped/Analyses/MSSM/FullRun2/Combine/April_2023/CMSSW_11_3_4/src/Analysis/Combine/Run2018/Inputs_Unc/datacards/Hbb.limits";
  string path2017SL_solo = "/nfs/dust/cms/user/leyvaped/Analyses/MSSM/FullRun2/Combine/April_2023/CMSSW_11_3_4/src/Analysis/Combine/Run2018/Inputs_Unc/model-interpretation/files2017SL/Datacards_SL/Hbb.limits";
  string path2017FH_solo = "/nfs/dust/cms/user/leyvaped/Analyses/MSSM/FullRun2/Combine/April_2023/CMSSW_11_3_4/src/Analysis/Combine/Run2018/Inputs_Unc/model-interpretation/files2017FH/Datacards_FH/Hbb.limits";
 
  //ouptut folder
  string output = "/nfs/dust/cms/user/leyvaped/Analyses/MSSM/FullRun2/Combine/April_2023/CMSSW_11_3_4/src/Analysis/Combine/Run2018/Inputs_Unc/model-interpretation/Results_theounc/";

  //need? setFolderNamesAccordingToBlinding(path2016_solo,output);
  string boson = "three";

  std::vector<AvailableScenarios> scenarious = {MHMODP_200};//{MHMODP_200,MH125,NEGMU1,NEGMU2,NEGMU3};
  //when using hMSSM and mhmod make sure to change mssm_xs_tools.h from L434 to adapt the tools to old rootfiles
  
  for(const auto& scenario : scenarious){
    std::cout<<"Scenario: "<<AvailableScenariosToString(scenario)<<std::endl;
    string output_mod 			= output + AvailableScenariosToString(scenario) + "/";
    //string path2016_solo_mod 	= path2016_solo + AvailableScenariosToString(scenario) + "/Hbb_limits_full_alluncert_theo";

    CheckOutputDir(output_mod);

    //benchmark scenario path
    LHCXSGLimits limits(mssmhbb::blinded,boson,getBanchmarkPath(scenario));
    
    if (scenario == HMSSM || scenario == MHMODP_200) limits.compareWithPrevious("HIG-16-018");
    limits.setScenario(scenario);
    limits.setXMin(300);
    limits.setXMax(1800);
    //limits.setXMin(125);
    //limits.setXMax(700);
    //if (scenario == HMSSM) limits.setXMax(1800);
    //if (scenario == MH125) limits.setXMax(1800);
    if(scenario != HMSSM)limits.drawExcludedRegion(3);	// Region NOT compatible with H(125)
    else limits.drawExcludedRegion(0);

    string benchmark_name = AvailableScenariosToString(scenario);
    string output_name = output_mod + boson + "_" + benchmark_name + "_" + output_prefix;
          
    // combined results UNCOMMENT if combination is performed
    //vector<Limit> GxBR_combined = limits.ReadCombineLimits(path2016_combined);
    //string benchmark_ref = "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Analysis/Tools/data/mhmodp_mu200_13TeV.root";
    //vector<Limit> mssm_limits_combined = limits.GetMSSMLimits(GxBR_combined,benchmark,"",false,benchmark_ref,30);
    //limits.Write(mssm_limits_combined, "/nfs/dust/cms/user/asmusspa/private/CMSSW_10_2_13/src/Combine/AnalysisLimits/ActualLimits/Plots/combined_limits.txt");

    //solo 2016 13 TeV
    
    TLegend legenda1(0.2,0.64,0.56,0.91); //neg mu scenarios
    TLegend legenda2(0.62,0.18,0.92,0.45); //mh125

    limits.ReadCombineLimits(path2017FH_solo);
    limits.Write(output_name);

    if (scenario == HMSSM || scenario == MHMODP_200) limits.LimitPlotter("16CMS", output_name,"54.54(2018)",string(HbbStyle::axisTitleMA().Data()),"tan#beta",false);
    else  if (scenario == MH125) limits.LimitPlotter(legenda2, output_name,"54.54(2018)",string(HbbStyle::axisTitleMA().Data()),"tan#beta",false);
    else  limits.LimitPlotter(legenda1, output_name,"54.54(2018)",string(HbbStyle::axisTitleMA().Data()),"tan#beta",false);
    
    
  }
	
  cout<<endl<<endl<<endl<<"-------------------------------------------------------------------------------------------------------------"<<endl;
  cout<<"IMPORTANT NOTE"<<endl;
  cout<<endl<<"Use this macro to compute the up (down) uncertainties coming from the pdf definition, alpha_s measurements and the uncertainty on factorization and renormalizaiton scale"<<endl;
  cout<<"This is handled in a rather sub-optimal way, only to facilitate providing results for Run 2 Legacy analysis promptly"<<endl;
  cout<<endl<<"To get the corresponding up(down) systematics, go to the file mssm_xs_tools.h and edit fom L435 according to which uncertainty you want to provide, taken from L477 forward"<<endl;
  cout<<"Remember to compile the changes before running (and change it back to normal when done)"<<endl;
  cout<<"For more information contact daina.leyva.pernia@desy.de"<<endl;
  cout<<"-------------------------------------------------------------------------------------------------------------"<<endl;
  
return 0;
}

std::string getBanchmarkPath(AvailableScenarios scenario){
  string output = "/nfs/dust/cms/user/leyvaped/Analyses/MSSM/FullRun2/Combine/April_2023/CMSSW_11_3_4/src/Analysis/Combine/Run2018/Inputs_Unc/model-interpretation/models_mssm/";
  switch (scenario){
  case MHMODP_200:
    output += "mhmodp_mu200_13TeV.root";
    break;
  case LIGHT_STOP:
    output += "lightstopmod_13TeV.root";
    break;
  case LIGHT_STAU:
    output += "lightstau1_13TeV.root";
    break;
  case HMSSM:
    output += "hMSSM_13TeV.root";
    break;
  case TAU_PHOBIC:
    output += "tauphobic_13TeV.root";
    break;
  case MH125:
    output += "mh125_13.root";
    break;
  case NEGMU1:
    output += "mh125_muneg_1_13.root";
    break;
  case NEGMU2:
    output += "mh125_muneg_2_13.root";
    break;
  case NEGMU3:
    output += "mh125_muneg_3_13.root";
    break;
  default: 
    break;
  }
  return output;
}

void setFolderNamesAccordingToBlinding(string& input, string & output){
  if(!mssmhbb::blinded) {
    input += "unblinded/";
    output += "unblinded/";
  }
  else {
    input += "blinded/";
    output += "blinded/";
  }
}
