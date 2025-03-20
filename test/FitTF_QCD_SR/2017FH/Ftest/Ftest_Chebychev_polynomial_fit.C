#include <iostream>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <RooChebychev.h>
#include <RooFitResult.h>

using namespace RooFit;

//void Ftest_Chebychev_polynomial_fit(const float &xmin = 240, const float &xmax = 560, const float &binning = 10) {
//void Ftest_Chebychev_polynomial_fit(const float &xmin = 280, const float &xmax = 800, const float &binning = 10) {
//void Ftest_Chebychev_polynomial_fit(const float &xmin = 400, const float &xmax = 1300, const float &binning = 10) {
void Ftest_Chebychev_polynomial_fit(const float &xmin = 600, const float &xmax = 2000, const float &binning = 50) {
    // Define your histogram and data here
    RooRealVar x("x", "x", xmin, xmax);
    TFile *f_mc = new  TFile("/nfs/dust/cms/user/leyvaped/Analyses/MSSM/FullRun2/Combine/April_2023/CMSSW_11_3_4/src/Analysis/Models/test/FitTF_QCD_SR/FinalTF/transfer_factors/with_online_btag_sf/btag_weight/mssmhbb_fh_2017_bweight_sr_x_cr_norebin_m_jet12.root", "old");
    TH1F *h_m12_sr = (TH1F *)f_mc->Get("m_jet12_h1")->Clone("m_jet12_sr");
    TH1F *h_m12_cr = (TH1F *)f_mc->Get("m_jet12_h2")->Clone("m_jet12_cr");
    h_m12_sr->Rebin(binning);
    h_m12_cr->Rebin(binning);

    //RooDataHist dh_m12_vr("dh_m12_vr", "dh_m12_vr", x, Import(*h_m12_vr));
    RooDataHist dh_m12_sr("dh_m12_sr", "dh_m12_sr", x, Import(*h_m12_sr));
    RooDataHist dh_m12_cr("dh_m12_cr", "dh_m12_cr", x, Import(*h_m12_cr));

    TH1F *h_m12_ratio = (TH1F *)h_m12_sr->Clone("h_m12_ratio");
    h_m12_ratio->Divide(h_m12_cr);
    RooDataHist dh_m12_ratio("dh_m12_ratio", "dh_m12_ratio", x, Import(*h_m12_ratio));

    RooAbsPdf* pol1 = nullptr; // Declare a pointer to the RooAbsPdf
    RooAbsPdf* pol2 = nullptr; // Declare a pointer to the RooAbsPdf
    RooAbsPdf* pol3 = nullptr; // Declare a pointer to the RooAbsPdf

    // Create variables to hold the results of different fits
    // RooFitResult *pol_fit = pol->chi2FitTo(dh_m12_ratio, Save());
    std::vector<RooFitResult*> fitResults;

    RooRealVar pol_cheb_d1_c1("pol_cheb_d1_c1", "1st deg term", 2.2619e-01 , -1, 1);

    RooRealVar pol_cheb_d2_c1("pol_cheb_d2_c1", "1st deg term 2nd order pol", 2.2619e-02 , -1, 1);
    RooRealVar pol_cheb_d2_c2("pol_cheb_d2_c2", "2nd deg term 2nd order pol",  1.0000e-03 , -1, 1);
    
    RooRealVar pol_cheb_d3_c1("pol_cheb_d3_c1", "1st deg term 3rd order pol", 2.2619e-02 , -1, 1);
    RooRealVar pol_cheb_d3_c2("pol_cheb_d3_c2", "2nd deg term 3rd order pol",  1.0000e-03 ,  -1, 1);
    RooRealVar pol_cheb_d3_c3("pol_cheb_d3_c3", "3rd deg term 3rd order pol",  1.0000e-03 ,  -1, 1);

    pol1 = new RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d1_c1));
    pol2 = new RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d2_c1, pol_cheb_d2_c2));
    pol3 = new RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d3_c1, pol_cheb_d3_c2, pol_cheb_d3_c3));

    fitResults.push_back(pol1->chi2FitTo(dh_m12_ratio, Save()));
    fitResults.push_back(pol2->chi2FitTo(dh_m12_ratio, Save()));
    fitResults.push_back(pol3->chi2FitTo(dh_m12_ratio, Save()));

    
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    // Plot after fit
    //-------------------------------------------------------------------------
    RooPlot *dh_ratio_frame1 = x.frame(Title(" "));
    dh_ratio_frame1->GetXaxis()->SetTitle("m_{12} [GeV]"); // Set the x-axis title
    dh_ratio_frame1->GetYaxis()->SetTitle("bbb/bbnb"); // Set the y-axis title
    dh_m12_ratio.plotOn(dh_ratio_frame1);

    pol1->plotOn(dh_ratio_frame1, VisualizeError(*fitResults[0], 2), FillColor(kYellow));
    pol1->plotOn(dh_ratio_frame1, VisualizeError(*fitResults[0], 1), FillColor(kGreen));
    dh_m12_ratio.plotOn(dh_ratio_frame1);
    pol1->plotOn(dh_ratio_frame1);

    // Things needed to calculate the F-test statistics
    int num_parameters1 = fitResults[0]->floatParsFinal().getSize();
    double  chi2_ndf_value1 = dh_ratio_frame1->chiSquare(num_parameters1);
    int ndfCheb1 = dh_m12_ratio.numEntries() - num_parameters1;
    double  chi2_value1 = chi2_ndf_value1*ndfCheb1;
    cout<<endl<<"num_par1  "<<num_parameters1<<endl;
    cout<<"chi2_ndf_value1 "<<chi2_ndf_value1<<endl;
    cout<<"ndfCheb1  "<<ndfCheb1<<endl;
    cout<<"chi2_value1 "<<chi2_value1<<endl;
    //-------------------------------------------------------------------------
    RooPlot *dh_ratio_frame2 = x.frame(Title(" "));
    dh_ratio_frame2->GetXaxis()->SetTitle("m_{12} [GeV]"); // Set the x-axis title
    dh_ratio_frame2->GetYaxis()->SetTitle("bbb/bbnb"); // Set the y-axis title
    dh_m12_ratio.plotOn(dh_ratio_frame2);

    pol2->plotOn(dh_ratio_frame2, VisualizeError(*fitResults[0], 2), FillColor(kYellow));
    pol2->plotOn(dh_ratio_frame2, VisualizeError(*fitResults[0], 1), FillColor(kGreen));
    dh_m12_ratio.plotOn(dh_ratio_frame2);
    pol2->plotOn(dh_ratio_frame2);

    // Things needed to calculate the F-test statistics
    int num_parameters2 = fitResults[1]->floatParsFinal().getSize();
    double  chi2_ndf_value2 = dh_ratio_frame2->chiSquare(num_parameters2);
    int ndfCheb2 = dh_m12_ratio.numEntries() - num_parameters2;
    double  chi2_value2 = chi2_ndf_value2*ndfCheb2;
    cout<<endl<<"num_par2  "<<num_parameters2<<endl;
    cout<<"chi2_ndf_value2 "<<chi2_ndf_value2<<endl;
    cout<<"ndfCheb2  "<<ndfCheb2<<endl;
    cout<<"chi2_value2 "<<chi2_value2<<endl;
    //-------------------------------------------------------------------------
    RooPlot *dh_ratio_frame3 = x.frame(Title(" "));
    dh_ratio_frame3->GetXaxis()->SetTitle("m_{12} [GeV]"); // Set the x-axis title
    dh_ratio_frame3->GetYaxis()->SetTitle("bbb/bbnb"); // Set the y-axis title
    dh_m12_ratio.plotOn(dh_ratio_frame3);

    pol3->plotOn(dh_ratio_frame3, VisualizeError(*fitResults[0], 2), FillColor(kYellow));
    pol3->plotOn(dh_ratio_frame3, VisualizeError(*fitResults[0], 1), FillColor(kGreen));
    dh_m12_ratio.plotOn(dh_ratio_frame3);
    pol3->plotOn(dh_ratio_frame3);

    // Things needed to calculate the F-test statistics
    int num_parameters3 = fitResults[2]->floatParsFinal().getSize();
    double  chi2_ndf_value3 = dh_ratio_frame3->chiSquare(num_parameters3);
    int ndfCheb3 = dh_m12_ratio.numEntries() - num_parameters3;
    double  chi2_value3 = chi2_ndf_value3*ndfCheb3;
    cout<<endl<<"num_par3  "<<num_parameters3<<endl;
    cout<<"chi2_ndf_value3 "<<chi2_ndf_value3<<endl;
    cout<<"ndfCheb3  "<<ndfCheb3<<endl;
    cout<<"chi2_value3 "<<chi2_value3<<endl;
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------

    // Calculate the F-statistics
    double F_stat_1vs2  = (chi2_value1 - chi2_value2) / (num_parameters2 - num_parameters1) / (chi2_value2 / ndfCheb2);
    double p_stat_1vs2 = 1 - ROOT::Math::fdistribution_cdf(F_stat_1vs2, num_parameters2 - num_parameters1, ndfCheb2);
    //double F_crit_1vs2 = ROOT::Math::fdistribution_quantile(0.95, num_parameters2 - num_parameters1, ndfCheb2);
    double F_crit_1vs2 = ROOT::Math::fdistribution_quantile(0.95, num_parameters2 - num_parameters1, ndfCheb2);
    double p_crit_1vs2 = 1 - ROOT::Math::fdistribution_cdf(F_crit_1vs2, num_parameters2 - num_parameters1, ndfCheb2) ;

    
    double F_stat_2vs3  = (chi2_value2 - chi2_value3) / (num_parameters3 - num_parameters2) / (chi2_value3 / ndfCheb3);
    double p_stat_2vs3 = 1 - ROOT::Math::fdistribution_cdf(F_stat_2vs3, num_parameters3 - num_parameters2, ndfCheb3);
    double F_crit_2vs3 = ROOT::Math::fdistribution_quantile(0.95, num_parameters3 - num_parameters2, ndfCheb3);
    double p_crit_2vs3 = 1 - ROOT::Math::fdistribution_cdf(F_crit_2vs3, num_parameters3 - num_parameters2, ndfCheb3) ;

    
    /*double F_stat_1vs3  = (chi2_value1 - chi2_value3) / (num_parameters3 - num_parameters1) / (chi2_value3 / ndfCheb3);
    double p_stat_1vs3 = 1 - ROOT::Math::fdistribution_cdf(F_stat_1vs3, num_parameters3 - num_parameters1, ndfCheb3);
    double F_crit_1vs3 = ROOT::Math::fdistribution_quantile(0.95, num_parameters3 - num_parameters1, ndfCheb3);
    double p_crit_1vs3 = 1 - ROOT::Math::fdistribution_cdf(F_crit_1vs3, num_parameters3 - num_parameters1, ndfCheb3) ;*/

    /*Double_t FDist(Double_t F, Double_t N, Double_t M)
      Computes the density function of F-distribution
 (probability function, integral of density, is computed in FDistI).

 Parameters N and M stand for degrees of freedom of chi-squares
 mentioned above parameter F is the actual variable x of the
 density function p(x) and the point at which the density function
 is calculated.

 About F distribution:
 F-distribution arises in testing whether two random samples
 have the same variance. It is the ratio of two chi-square
 distributions, with N and M degrees of freedom respectively,
 where each chi-square is first divided by it's number of degrees
 of freedom.".
    */

    std::cout << std::endl << "F-test results (1st vs 2nd order):" << std::endl;
    std::cout << "F-crit: " << F_crit_1vs2 << std::endl;
    std::cout << "P-value-crit: " << p_crit_1vs2 << std::endl;
    std::cout << "F-statistic: " << F_stat_1vs2 << std::endl;
    std::cout << "P-value : " << p_stat_1vs2 << std::endl;

    std::cout << std::endl << "F-test results (2nd vs 3rd order):" << std::endl;
    std::cout << "F-crit: " << F_crit_2vs3 << std::endl;
    std::cout << "P-value-crit: " << p_crit_2vs3 << std::endl;
    std::cout << "F-statistic: " << F_stat_2vs3 << std::endl;
    std::cout << "P-value : " << p_stat_2vs3 << std::endl;

    /*std::cout << std::endl << "F-test results (1st vs 3rd order):" << std::endl;
    std::cout << "F-crit: " << F_crit_1vs3 << std::endl;
    std::cout << "P-value-crit: " << p_crit_1vs3 << std::endl;
    std::cout << "F-statistic: " << F_stat_1vs3 << std::endl;
    std::cout << "P-value : " << p_stat_1vs3 << std::endl;*/

    // Clean up
    for (auto fitResult : fitResults) {
        delete fitResult;
    }
}
