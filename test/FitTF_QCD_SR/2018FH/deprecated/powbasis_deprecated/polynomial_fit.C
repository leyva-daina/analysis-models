#include "Analysis/Models/interface/RooPolynomialWithShift.h"

using namespace RooFit;

int polynomial_fit(const bool & mc = false,const float & x0_ = 0., const float & xmin_ = 270, const float & xmax_ = 2000, const float & binning_ = 1, const int & poldegree_ = 1)
{
    
    cout<<"======================================================================================================================"<<endl;
    cout<<"======================================================================================================================"<<endl;
    cout<<"Fitting range "<<xmin_<<" to "<<xmax_<<" with "<<poldegree_<<"-degree polynomial, shifted by "<<x0_<<" on x, binning = "<<binning_<<endl<<endl;
    TH1::SetDefaultSumw2();
    // variable
    RooRealVar x("x", "x", xmin_, xmax_);
    // shape parameters
    RooRealVar pol_c1("pol_c1", "linear term"   , 5e-3, -1.00, 1.00);
    RooRealVar pol_c2("pol_c2", "quadratic term", 7.e-7, -1.00, 1.00);
    // x -> x-x0
    RooRealVar x0("x0", "x0",x0_);
    // construct the PDF
    
    std::unique_ptr<analysis::models::RooPolynomialWithShift> pol; // Use a smart pointer

    if (poldegree_ == 1)
    pol = std::make_unique<analysis::models::RooPolynomialWithShift>("pol", "polynomial", x, RooArgList(pol_c1), x0, 1);
    else if (poldegree_ == 2)
    pol = std::make_unique<analysis::models::RooPolynomialWithShift>("pol", "polynomial", x, RooArgList(pol_c1, pol_c2), x0, 1);
    else
    {cout<<endl<<"polynomial poldegree_ "<<poldegree_<< " not supported, review codes"<<endl; return 0;}

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
    RooPlot * dh_frame = x.frame(Title("dh_frame"));
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

    RooPlot *dh_ratio_frame = x.frame(Title("dh_ratio_frame"));
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

    pol_fit->Print("V");
    c_dh_ratio->SaveAs(Form("fitTF_%.0fto%.0f_bin%.0f_x0-%.0f_pol-g%d.png", xmin_, xmax_, binning_, x0_, poldegree_ ));
    c_dh_ratio->SaveAs(Form("fitTF_%.0fto%.0f_bin%.0f_x0-%.0f_pol-g%d.pdf", xmin_, xmax_, binning_, x0_, poldegree_ ));

    
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