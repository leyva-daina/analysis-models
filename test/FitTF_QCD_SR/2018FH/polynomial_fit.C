#include "Analysis/Models/interface/RooPolynomialWithShift.h"
#include <iostream>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1F.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <RooChebychev.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>


using namespace RooFit;

int polynomial_fit(const bool & mc = true, const float & xmin_ = 270, const float & xmax_ = 2000, const float & binning_ = 1, const int & poldegree_ = 1, const std::string & functname_ = "Chebychev", const float & x0_ = 0.)
{
    
    cout<<"======================================================================================================================"<<endl;
    cout<<"======================================================================================================================"<<endl;
    cout<<"Fitting range "<<xmin_<<" to "<<xmax_<<" with "<<poldegree_<<"-degree "<< functname_.c_str() <<" polynomial, binning = "<<binning_<<endl<<endl;
    TH1::SetDefaultSumw2();
    // variable
    RooRealVar x("x", "x", xmin_, xmax_);
    RooRealVar x0("x0", "x0",x0_);

    //------------------------------------------------------------------------------------------------------
    // shape parameters Chebychev polynomials
    RooRealVar pol_cheb_d1_c1("pol_cheb_d1_c1", "1st deg term", 2.2619e-01 , -1, 1);

    RooRealVar pol_cheb_d2_c1("pol_cheb_d2_c1", "1st deg term 2nd order pol", 2.2619e-02 , -1, 1);
    RooRealVar pol_cheb_d2_c2("pol_cheb_d2_c2", "2nd deg term 2nd order pol",  1.0000e-03 , -1, 1);
    
    RooRealVar pol_cheb_d3_c1("pol_cheb_d3_c1", "1st deg term 3rd order pol", 2.2619e-02 , -1, 1);
    RooRealVar pol_cheb_d3_c2("pol_cheb_d3_c2", "2nd deg term 3rd order pol",  1.0000e-03 ,  -1, 1);
    RooRealVar pol_cheb_d3_c3("pol_cheb_d3_c3", "3rd deg term 3rd order pol",  1.0000e-03 ,  -1, 1);


    // shape parameters standard power polynomials
    RooRealVar pol_pow_d1_c1("pol_pow_d1_c1", "1st deg term", 5e-3, -1.00, 1.00);

    RooRealVar pol_pow_d2_c1("pol_pow_d2_c1", "1st deg term 2nd order pol", 5e-3, 0, 1.00);
    RooRealVar pol_pow_d2_c2("pol_pow_d2_c2", "2nd deg term 2nd order pol", 7.e-7, 0, 1.00);
    
    RooRealVar pol_pow_d3_c1("pol_pow_d3_c1", "1st deg term 3rd order pol", 5e-3, 0.00, 1.00);
    RooRealVar pol_pow_d3_c2("pol_pow_d3_c2", "2nd deg term 3rd order pol", 7.e-7, 0.00, 1.00);
    RooRealVar pol_pow_d3_c3("pol_pow_d3_c3", "3rd deg term 3rd order pol", 7.e-7, 0.00, 1.00);
    

    // shape parameters bernstein polynomials
    // from https://root.cern.ch/doc/master/classRooBernstein.html
    // By giving n+1 coefficients in the constructor, this class constructs the n+1 polynomials of degree n, and sums them to form an element of the space of polynomials of degree n.
    RooRealVar pol_bern_d1_c1("pol_bern_d1_c1", "1st deg term 1st order pol", 4.5270e-07 , -0.000001, 0.000001);
    RooRealVar pol_bern_d1_c2("pol_bern_d1_c2", "2nd deg term 1st order pol",  1.0000e-03 , -1, 1);
    
    RooRealVar pol_bern_d2_c1("pol_bern_d2_c1", "1st deg term 2nd order pol",  4.5270e-07 , -0.000001, 0.000001);
    RooRealVar pol_bern_d2_c2("pol_bern_d2_c2", "2nd deg term 2nd order pol",  7.1742e-07 , -0.000001, 0.000001);
    RooRealVar pol_bern_d2_c3("pol_bern_d2_c3", "3rd deg term 2nd order pol",  7.1742e-07 , -0.000001, 0.000001);

    RooRealVar pol_bern_d3_c1("pol_bern_d3_c1", "1st deg term 3rd order pol",  4.5270e-07 , -0.000001, 0.000001);
    RooRealVar pol_bern_d3_c2("pol_bern_d3_c2", "2nd deg term 3rd order pol",  7.1742e-07 , -0.000001, 0.000001);
    RooRealVar pol_bern_d3_c3("pol_bern_d3_c3", "3rd deg term 3rd order pol",  7.1742e-07 , -0.000001, 0.000001);
    RooRealVar pol_bern_d3_c4("pol_bern_d3_c4", "4th deg term 3rd order pol",  7.1742e-07 , -0.000001, 0.000001);
    
    //------------------------------------------------------------------------------------------------------
    
    
    // construct the PDF

    RooAbsPdf* pol = nullptr; // Declare a pointer to the RooAbsPdf

    if(functname_ == "Chebychev")
    {
        if (poldegree_ == 1)
        pol = new RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d1_c1));

        else if (poldegree_ == 2)
        pol = new RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d2_c1, pol_cheb_d2_c2));

        else if (poldegree_ == 3)
        pol = new RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d3_c1, pol_cheb_d3_c2, pol_cheb_d3_c3));
        
        else
        {cout<<endl<<"chebychev polynomial degree "<<poldegree_<< " not supported, review codes"<<endl; return 0;}
    }
    
    else if(functname_ == "Bernstein")
    {
        if (poldegree_ == 1)
        pol = new RooBernstein("bernstein", "Bernstein PDF", x, RooArgList(pol_bern_d1_c1, pol_bern_d1_c2));

        else if (poldegree_ == 2)
        pol = new RooBernstein("bernstein", "Bernstein PDF", x, RooArgList(pol_bern_d2_c1, pol_bern_d2_c2, pol_bern_d2_c3));

        else if (poldegree_ == 3)
        pol = new RooBernstein("bernstein", "Bernstein PDF", x, RooArgList(pol_bern_d3_c1, pol_bern_d3_c2, pol_bern_d3_c3, pol_bern_d3_c4));
        
        else
        {cout<<endl<<"Bernstein polynomial degree "<<poldegree_<< " not supported, review codes"<<endl; return 0;}
    }
    
    
    else if(functname_ == "Power")
    {
        if (poldegree_ == 1)
        pol = new analysis::models::RooPolynomialWithShift("pol", "polynomial", x, RooArgList(pol_pow_d1_c1), x0, 1);

        else if (poldegree_ == 2)
        pol = new analysis::models::RooPolynomialWithShift("pol", "polynomial", x, RooArgList(pol_pow_d2_c1, pol_pow_d2_c2), x0, 1);

        else if (poldegree_ == 3)
        pol = new analysis::models::RooPolynomialWithShift("pol", "polynomial", x, RooArgList(pol_pow_d3_c1, pol_pow_d3_c2, pol_pow_d3_c3), x0, 1);
        
        else
        {cout<<endl<<"Standard polynomial degree "<<poldegree_<< " not supported, review codes"<<endl; return 0;}
    }

    else
    {cout<<endl<<"polynomial family not supported, review codes"<<endl; return 0;}

    // open histograms VR and CR
    //TH1F *h_m12_vr;
    TH1F *h_m12_sr;
    TH1F *h_m12_cr;
    //TFile *f_vr = new TFile("mssmHbb_2018_FH_Run2018ABCD_vr.root", "old");
    //TFile *f_sr = new TFile("mssmHbb_2018_FH_Run2018ABCD_vr.root", "old");
    //TFile *f_cr = new TFile("mssmHbb_2018_FH_Run2018ABCD_cr.root", "old");
    //TFile *f_mc = new TFile("mssmhbb_fh_2018_bwgt_vr_x_cr_m_jet12.root", "old");
    TFile *f_mc = new TFile("mssmhbb_fh_2018_bwgt_sr_x_cr_m_jet12_range.root", "old");

    if (not mc)
    {
        //h_m12_vr = (TH1F *)f_vr->Get("m_jet12")->Clone("m_jet12_vr");
        //h_m12_sr = (TH1F *)f_sr->Get("m_jet12")->Clone("m_jet12_sr");
        //h_m12_cr = (TH1F *)f_cr->Get("m_jet12")->Clone("m_jet12_cr");
        cout<<"Data is blinded"<<endl;
    }
    else
    {
        //h_m12_vr = (TH1F *)f_mc->Get("m_jet12_h1")->Clone("m_jet12_vr");
        h_m12_sr = (TH1F *)f_mc->Get("m_jet12_h1_range")->Clone("m_jet12_sr");
        h_m12_cr = (TH1F *)f_mc->Get("m_jet12_h2_range")->Clone("m_jet12_cr");
    }

    //h_m12_vr->Rebin(10);
    h_m12_sr->Rebin(binning_);
    h_m12_cr->Rebin(binning_);

    //RooDataHist dh_m12_vr("dh_m12_vr", "dh_m12_vr", x, Import(*h_m12_vr));
    RooDataHist dh_m12_sr("dh_m12_sr", "dh_m12_sr", x, Import(*h_m12_sr));
    RooDataHist dh_m12_cr("dh_m12_cr", "dh_m12_cr", x, Import(*h_m12_cr));

    // Plotting histograms
    // As there are 2 histograms in the same frame, RooFit takes the 2nd (last) one
    // to set the normalisation of the PDFs
    // [#1] INFO:Plotting -- RooPlot::updateFitRangeNorm: New event count of 306256
    // will supercede previous event count of 1.63216e+06 for normalization of PDF
    // projections
    //RooPlot * dh_frame = x.frame(Title("bbb/bbnb"));
    RooPlot * dh_frame = x.frame(Title(" "));
    dh_m12_cr.plotOn(dh_frame);
    //dh_m12_vr.plotOn(dh_frame);
    dh_m12_sr.plotOn(dh_frame);
    TCanvas * c_dh = new TCanvas("c_dh", "c_dh", 800, 400);
    dh_frame->Draw();

    // ratio VR/CR
    //TH1F *h_m12_ratio = (TH1F *)h_m12_vr->Clone("h_m12_ratio");
    TH1F *h_m12_ratio = (TH1F *)h_m12_sr->Clone("h_m12_ratio");
    h_m12_ratio->Divide(h_m12_cr);
    RooDataHist dh_m12_ratio("dh_m12_ratio", "dh_m12_ratio", x, Import(*h_m12_ratio));

    
    RooPlot *dh_ratio_frame = x.frame(Title(" "));
    dh_ratio_frame->GetXaxis()->SetTitle("m_{12} [GeV]"); // Set the x-axis title
    dh_ratio_frame->GetYaxis()->SetTitle("bbb/bbnb"); // Set the y-axis title
    dh_m12_ratio.plotOn(dh_ratio_frame);

    // FIT
    RooFitResult *pol_fit = pol->chi2FitTo(dh_m12_ratio, Save());

    // Plot after fit
    pol->plotOn(dh_ratio_frame, VisualizeError(*pol_fit, 2), FillColor(kYellow));
    pol->plotOn(dh_ratio_frame, VisualizeError(*pol_fit, 1), FillColor(kGreen));
    dh_m12_ratio.plotOn(dh_ratio_frame);
    pol->plotOn(dh_ratio_frame);
    
    TCanvas *c_dh_ratio = new TCanvas("c_dh_ratio", "c_dh_ratio", 800, 400);
    dh_ratio_frame->Draw();

    int num_parameters = pol_fit->floatParsFinal().getSize();
    cout << "chi^2/ndf = " << dh_ratio_frame->chiSquare(num_parameters) << ", for " << num_parameters << " parameters" << endl;

    // Calculate p-value
    int poldegree_s_of_freedom = num_parameters;
    double chi2_value = dh_ratio_frame->chiSquare(num_parameters) * poldegree_s_of_freedom;
    double p_value = TMath::Prob(chi2_value, poldegree_s_of_freedom);

    cout << "p-value = " << setprecision(4)  << p_value << endl;


    // Create a TPaveText to display p-value and chi-square
    TPaveText* text = new TPaveText(0.7, 0.2, 0.86, 0.4, "NDC");
    text->SetFillColor(0);
    text->SetBorderSize(0);
    text->AddText(Form("#chi^{2}/ndf = %.2f", dh_ratio_frame->chiSquare(num_parameters)));
    text->AddText(Form("p-value = %.2f", p_value));
    text->Draw();

        // Add the "CMS Work in Progress" sign
    TLatex cmsText;
    cmsText.SetNDC();
    cmsText.SetTextFont(61);
    cmsText.SetTextSize(0.055);
    cmsText.SetTextColor(kBlack);
    cmsText.DrawLatex(0.1, 0.92, "CMS Simulation");
    cmsText.SetTextFont(52);
    cmsText.SetTextSize(0.05);
    cmsText.DrawLatex(0.35, 0.92, "Work in Progress");


    pol_fit->Print("V");
    c_dh_ratio->SaveAs(Form("fitTF_%.0fto%.0f_bin%.0f_x0-0_%s-pol-g%d.png", xmin_, xmax_, binning_, functname_.c_str(), poldegree_ ));
    c_dh_ratio->SaveAs(Form("fitTF_%.0fto%.0f_bin%.0f_x0-0_%s-pol-g%d.pdf", xmin_, xmax_, binning_, functname_.c_str(), poldegree_ ));

    
    // Print the correlation matrix
    cout << "Correlation matrix:" << endl;
    pol_fit->correlationMatrix().Print();

    // Print the covariance matrix
    cout << "Covariance matrix:" << endl;
    pol_fit->covarianceMatrix().Print();

    cout<<"======================================================================================================================"<<endl;
    cout<<"======================================================================================================================"<<endl<<endl;
    return 0;
}




    /*double f_statistic_2_1 = (fitResults[0]->minNll() - fitResults[1]->minNll()) / (fitResults[1]->minNll() / (fitResults[1]->floatParsFinal().getSize() - fitResults[0]->floatParsFinal().getSize()));
    double f_statistic_3_2 = (fitResults[1]->minNll() - fitResults[2]->minNll()) / (fitResults[2]->minNll() / (fitResults[2]->floatParsFinal().getSize() - fitResults[1]->floatParsFinal().getSize()));
    double f_statistic_3_1 = (fitResults[0]->minNll() - fitResults[2]->minNll()) / (fitResults[2]->minNll() / (fitResults[2]->floatParsFinal().getSize() - fitResults[0]->floatParsFinal().getSize()));

    // Print F-test results
    std::cout << "F-statistic (2 vs 1 degrees): " << f_statistic_2_1 << std::endl;
    std::cout << "F-statistic (3 vs 2 degrees): " << f_statistic_3_2 << std::endl;
    std::cout << "F-statistic (3 vs 1 degrees): " << f_statistic_3_1 << std::endl;*/