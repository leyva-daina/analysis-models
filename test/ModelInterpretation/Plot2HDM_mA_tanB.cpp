/*
 * plotTanBetaLimits.cpp
 *
 *  Created on: 9 Mar 2017
 *  Author: shevchen
 *
 *  Macro to calculate and plot 2HDM limits
 *  as a function of mA and tanBeta for particluar
 *  value of cos(beta-alpha)
 */

 // Modified by leyva-daina on Dec 2023

#include <iostream>
#include <string>
#include <vector>

#include <TCanvas.h>
#include <TH2.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TPad.h>
#include <TFile.h>

#include "Analysis/Models/interface/THDMLimits.h"
#include "Analysis/Models/interface/Limit.h"
#include "Analysis/Models/interface/HbbStyleClass.h"
#include "Analysis/Models/interface/namespace_mssmhbb.h"

HbbStyle style;

using namespace std;
using namespace analysis::mssmhbb;
using namespace mssmhbb;

int Plot2HDM_mA_tanB(){
	std::cout<<std::endl<<"Entering Plot2HDM_cosBA_tanB"<<std::endl;
    PublicationStatus status = PUBLIC;
	style.setTDRstyle(status);

	//Prefix to the output
	string output_prefix = "13TeV_limits";
	//ATLAS results
	string Azh_result = "HIG-18-005";//"1502.04478";//"ATLAS-CONF-2017-055" //FIXME this should say CMS

	//paths with results of the combine tool
  	string path2018FH  	 = "/afs/desy.de/user/l/leyvaped/public/paths_limits_theoryUncertainty_UNBLINDED/limits_afs.txt";
	//string path2018FH = "/nfs/dust/cms/user/leyvaped/Analyses/MSSM/FullRun2/Combine/April_2023/CMSSW_11_3_4/src/Analysis/Combine/Run2018/Inputs_Unc/model-interpretation/files2017SL/Datacards_SL/Hbb.limits";
	//string path2018FH = "/nfs/dust/cms/user/leyvaped/Analyses/MSSM/FullRun2/Combine/April_2023/CMSSW_11_3_4/src/Analysis/Combine/Run2018/Inputs_Unc/model-interpretation/files2017SL/Datacards_SL/Hbb.limits";
	//value of cos(beta-alpha)
	double cB_A = 0.1;
	//Details of the 2HDM produciton
	string thdm_production = "FullRun_100PerJob_AllTypesAndBosons";
	// type of the 2hdm: type2 or type3 (=flipped); mhmodp_200/hMSSM should also work //edit (Daina) used the other macro for mhmod and hMSSM
	string thdm_type = "type2"; string banch_name;
	if(thdm_type == "type2")  banch_name = "type2";
	if(thdm_type == "flipped") banch_name = "type3";
	else if (thdm_type == "lepton_specific") banch_name = "type4";

	AvailableScenarios scenario = AvailableScenariosFromString(thdm_type);
	string thdm_scans = "/afs/desy.de/user/l/leyvaped/public/paths_limits_theoryUncertainty_UNBLINDED/SusHi/Histograms3D_" + banch_name + "_mA_mH_mh.root";

	//higgs boson: H/A/both
	string boson = "both";
	string output = "./Results_2HDM_Combination_Unblinded_publication/";
	CheckOutputDir(output);
	
	
	THDMLimits limits(false,boson,125,1800,5,100);
	//limits.compareWithPrevious("HIG-18-005");
	limits.setScenario(scenario);
	limits.SetHiggsBoson(boson);
	limits.ReadCombineLimits(path2018FH);
	auto thdmLim = limits.Get2HDM_1D_Limits(thdm_scans,cB_A,"z");
	for(const auto& l: limits.getGxBrLimits()) cout<<"GxBR M: "<<l.getX()<<" exp = "<<l.getMedian()<<" +1G = "<<l.getPlus1G()<<" -1G = "<<l.getMinus1G()<<" +2G = "<<l.getPlus2G()<<" -2G = "<<l.getMinus2G()<<" Obs = "<<l.getObserved()<<endl;
	for(const auto& l: thdmLim) cout<<"M: "<<l.getX()<<" exp = "<<l.getMedian()<<" +1G = "<<l.getPlus1G()<<" -1G = "<<l.getMinus1G()<<" +2G = "<<l.getPlus2G()<<" -2G = "<<l.getMinus2G()<<" Obs = "<<l.getObserved()<<endl;
	//limits.compareWithPrevious(Azh_result);
	//limits.compareWithPrevious("HIG-18-005");

	// 2HDM tanBeta vs mA limits for cos(beta-alpha) = cB_A
	//TLegend leg_2HDM_tB(0.65,0.17,0.92,0.44);
	TLegend leg_2HDM_tB(0.35,0.74,0.93,0.87);
	output += thdm_type + "_" + boson + "_bosons_tanB_mA" + output_prefix;
	//limits.LimitPlotter(leg_2HDM_tB,output,"36.0 fb^{-1}",string(HbbStyle::axisTitleMA().Data()),"tan#kern[-0.3]{ }#beta",true);
	//output += "vs_AZh_" + Azh_result;
	//output += "_zoomed";
	leg_2HDM_tB.Clear();
	limits.setXMin(125); limits.setXMax(1800);limits.setYMin(5); limits.setYMax(100);
	limits.LimitPlotter(leg_2HDM_tB,output,"36.0 fb^{-1}",string(HbbStyle::axisTitleMA().Data()),"tan#kern[-0.3]{ }#beta",true);

	return 0;
}
