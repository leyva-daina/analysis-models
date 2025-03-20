import ROOT
import argparse
from ROOT import RooRealVar, RooDataHist, RooPlot, RooChebychev, RooBernstein, RooArgList, RooFit, RooAddPdf

def polynomial_fit(mc=True, xmin_=270, xmax_=2000, binning_=1, poldegree_=1, functname_="Chebychev", x0_=0.):
    ROOT.TH1.SetDefaultSumw2()
    ROOT.gROOT.SetBatch(ROOT.kTRUE)
    
    # variable
    x = RooRealVar("x", "x", xmin_, xmax_)
    x0 = RooRealVar("x0", "x0", x0_)

    # shape parameters Chebychev polynomials
    pol_cheb_d1_c1 = RooRealVar("pol_cheb_d1_c1", "1st deg term", 2.2619e-01, -1, 1)

    pol_cheb_d2_c1 = RooRealVar("pol_cheb_d2_c1", "1st deg term 2nd order pol", 2.2619e-02, -1, 1)
    pol_cheb_d2_c2 = RooRealVar("pol_cheb_d2_c2", "2nd deg term 2nd order pol", 1.0000e-03, -1, 1)
    
    pol_cheb_d3_c1 = RooRealVar("pol_cheb_d3_c1", "1st deg term 3rd order pol", 2.2619e-02, -1, 1)
    pol_cheb_d3_c2 = RooRealVar("pol_cheb_d3_c2", "2nd deg term 3rd order pol", 1.0000e-03, -1, 1)
    pol_cheb_d3_c3 = RooRealVar("pol_cheb_d3_c3", "3rd deg term 3rd order pol", 1.0000e-03, -1, 1)

    # shape parameters standard power polynomials
    pol_pow_d1_c1 = RooRealVar("pol_pow_d1_c1", "1st deg term", 5e-3, -1.00, 1.00)

    pol_pow_d2_c1 = RooRealVar("pol_pow_d2_c1", "1st deg term 2nd order pol", 5e-3, 0, 1.00)
    pol_pow_d2_c2 = RooRealVar("pol_pow_d2_c2", "2nd deg term 2nd order pol", 7.e-7, 0, 1.00)
    
    pol_pow_d3_c1 = RooRealVar("pol_pow_d3_c1", "1st deg term 3rd order pol", 5e-3, 0.00, 1.00)
    pol_pow_d3_c2 = RooRealVar("pol_pow_d3_c2", "2nd deg term 3rd order pol", 7.e-7, 0.00, 1.00)
    pol_pow_d3_c3 = RooRealVar("pol_pow_d3_c3", "3rd deg term 3rd order pol", 7.e-7, 0.00, 1.00)

    # shape parameters bernstein polynomials
    pol_bern_d1_c1 = RooRealVar("pol_bern_d1_c1", "1st deg term 1st order pol", 4.5270e-07, -0.000001, 0.000001)
    pol_bern_d1_c2 = RooRealVar("pol_bern_d1_c2", "2nd deg term 1st order pol", 1.0000e-03, -1, 1)
    
    pol_bern_d2_c1 = RooRealVar("pol_bern_d2_c1", "1st deg term 2nd order pol", 4.5270e-07, -0.000001, 0.000001)
    pol_bern_d2_c2 = RooRealVar("pol_bern_d2_c2", "2nd deg term 2nd order pol", 7.1742e-07, -0.000001, 0.000001)
    pol_bern_d2_c3 = RooRealVar("pol_bern_d2_c3", "3rd deg term 2nd order pol", 7.1742e-07, -0.000001, 0.000001)

    pol_bern_d3_c1 = RooRealVar("pol_bern_d3_c1", "1st deg term 3rd order pol", 4.5270e-07, -0.000001, 0.000001)
    pol_bern_d3_c2 = RooRealVar("pol_bern_d3_c2", "2nd deg term 3rd order pol", 7.1742e-07, -0.000001, 0.000001)
    pol_bern_d3_c3 = RooRealVar("pol_bern_d3_c3", "3rd deg term 3rd order pol", 7.1742e-07, -0.000001, 0.000001)
    pol_bern_d3_c4 = RooRealVar("pol_bern_d3_c4", "4th deg term 3rd order pol", 7.1742e-07, -0.000001, 0.000001)

    # Construct the PDF
    pol = None  # Declare a pointer to the RooAbsPdf

    if functname_ == "Chebychev":
        if poldegree_ == 1:
            pol = RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d1_c1))
        elif poldegree_ == 2:
            pol = RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d2_c1, pol_cheb_d2_c2))
        elif poldegree_ == 3:
            pol = RooChebychev("chebychev", "Chebychev PDF", x, RooArgList(pol_cheb_d3_c1, pol_cheb_d3_c2, pol_cheb_d3_c3))
        else:
            print("Chebychev polynomial degree {} not supported, review codes".format(poldegree_))
            return

    elif functname_ == "Bernstein":
        if poldegree_ == 1:
            pol = RooBernstein("bernstein", "Bernstein PDF", x, RooArgList(pol_bern_d1_c1, pol_bern_d1_c2))
        elif poldegree_ == 2:
            pol = RooBernstein("bernstein", "Bernstein PDF", x, RooArgList(pol_bern_d2_c1, pol_bern_d2_c2, pol_bern_d2_c3))
        elif poldegree_ == 3:
            pol = RooBernstein("bernstein", "Bernstein PDF", x, RooArgList(pol_bern_d3_c1, pol_bern_d3_c2, pol_bern_d3_c3, pol_bern_d3_c4))
        else:
            print("Bernstein polynomial degree {} not supported, review codes".format(poldegree_))
            return

    elif functname_ == "Power":
        if poldegree_ == 1:
            pol = analysis.models.RooPolynomialWithShift("pol", "polynomial", x, RooArgList(pol_pow_d1_c1), x0, 1)
        elif poldegree_ == 2:
            pol = analysis.models.RooPolynomialWithShift("pol", "polynomial", x, RooArgList(pol_pow_d2_c1, pol_pow_d2_c2), x0, 1)
        elif poldegree_ == 3:
            pol = analysis.models.RooPolynomialWithShift("pol", "polynomial", x, RooArgList(pol_pow_d3_c1, pol_pow_d3_c2, pol_pow_d3_c3), x0, 1)
        else:
            print("Standard polynomial degree {} not supported, review codes".format(poldegree_))
            return

    else:
        print("Polynomial family not supported, review codes")
        return

    # Open histograms VR and CR
    f_mc = ROOT.TFile("mssmhbb_fh_2018_bwgt_sr_x_cr_m_jet12_range.root", "old")

    if not mc:
        print("Data is blinded")
    else:
        h_m12_sr = f_mc.Get("m_jet12_h1_range").Clone("m_jet12_sr")
        h_m12_cr = f_mc.Get("m_jet12_h2_range").Clone("m_jet12_cr")

    h_m12_sr.Rebin(binning_)
    h_m12_cr.Rebin(binning_)


    dh_m12_sr = ROOT.RooDataHist("dh_m12_sr", "dh_m12_sr", ROOT.RooArgList(x), ROOT.RooFit.Binning(h_m12_sr.GetNbinsX()), ROOT.RooFit.Import(h_m12_sr))
    dh_m12_cr = ROOT.RooDataHist("dh_m12_cr", "dh_m12_cr", ROOT.RooArgList(x), ROOT.RooFit.Binning(h_m12_cr.GetNbinsX()), ROOT.RooFit.Import(h_m12_cr))

    # Plotting histograms
    dh_frame = x.frame(ROOT.RooFit.Title(" "))
    dh_m12_cr.plotOn(dh_frame)
    dh_m12_sr.plotOn(dh_frame)
    c_dh = ROOT.TCanvas("c_dh", "c_dh", 800, 400)
    dh_frame.Draw()

    # Ratio VR/CR
    h_m12_ratio = h_m12_sr.Clone("h_m12_ratio")
    h_m12_ratio.Divide(h_m12_cr)
    dh_m12_ratio = RooDataHist("dh_m12_ratio", "dh_m12_ratio", x, RooDataHist.Import(h_m12_ratio))

    dh_ratio_frame = x.frame(ROOT.RooFit.Title(" "))
    dh_ratio_frame.GetXaxis().SetTitle("m_{12} [GeV]")  # Set the x-axis title
    dh_ratio_frame.GetYaxis().SetTitle("bbb/bbnb")  # Set the y-axis title
    dh_m12_ratio.plotOn(dh_ratio_frame)

    # FIT
    pol_fit = pol.chi2FitTo(dh_m12_ratio, RooFit.Save())

    # Plot after fit
    pol.plotOn(dh_ratio_frame, RooFit.VisualizeError(pol_fit, 2), RooFit.FillColor(ROOT.kYellow))
    pol.plotOn(dh_ratio_frame, RooFit.VisualizeError(pol_fit, 1), RooFit.FillColor(ROOT.kGreen))
    dh_m12_ratio.plotOn(dh_ratio_frame)
    pol.plotOn(dh_ratio_frame)
    
    c_dh_ratio = ROOT.TCanvas("c_dh_ratio", "c_dh_ratio", 800, 400)
    dh_ratio_frame.Draw()

    num_parameters = pol_fit.floatParsFinal().getSize()
    print("chi^2/ndf =", dh_ratio_frame.chiSquare(num_parameters), ", for", num_parameters, "parameters")

    # Calculate p-value
    poldegree_s_of_freedom = num_parameters
    chi2_value = dh_ratio_frame.chiSquare(num_parameters) * poldegree_s_of_freedom
    p_value = ROOT.TMath.Prob(chi2_value, poldegree_s_of_freedom)

    print("p-value =", "{:.4f}".format(p_value))

    # Create a TPaveText to display p-value and chi-square
    text = ROOT.TPaveText(0.7, 0.2, 0.86, 0.4, "NDC")
    text.SetFillColor(0)
    text.SetBorderSize(0)
    text.AddText(" #chi^{2}/ndf = {:.2f}".format(dh_ratio_frame.chiSquare(num_parameters)))
    text.AddText(" p-value = {:.2f}".format(p_value))
    text.Draw()

    # Add the "CMS Work in Progress" sign
    cmsText = ROOT.TLatex()
    cmsText.SetNDC()
    cmsText.SetTextFont(61)
    cmsText.SetTextSize(0.055)
    cmsText.SetTextColor(ROOT.kBlack)
    cmsText.DrawLatex(0.1, 0.92, "CMS Simulation")
    cmsText.SetTextFont(52)
    cmsText.SetTextSize(0.05)
    cmsText.DrawLatex(0.35, 0.92, "Work in Progress")

    pol_fit.Print("V")
    c_dh_ratio.SaveAs("fitTF_{}_to_{}_bin{}_x0-0_{}-pol-g{}.png".format(xmin_, xmax_, binning_, functname_, poldegree_))
    c_dh_ratio.SaveAs("fitTF_{}_to_{}_bin{}_x0-0_{}-pol-g{}.pdf".format(xmin_, xmax_, binning_, functname_, poldegree_))

    # Print the correlation matrix
    print("Correlation matrix:")
    pol_fit.correlationMatrix().Print()

    # Print the covariance matrix
    print("Covariance matrix:")
    pol_fit.covarianceMatrix().Print()

    print("======================================================================================================================")
    print("======================================================================================================================")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Fit a polynomial to data")
    parser.add_argument("--mc", type=bool, default=True, help="Use MC data (True) or not (False)")
    parser.add_argument("--xmin", type=float, default=270, help="Minimum x value for the fit")
    parser.add_argument("--xmax", type=float, default=2000, help="Maximum x value for the fit")
    parser.add_argument("--binning", type=int, default=1, help="Binning for the histograms")
    parser.add_argument("--poldegree", type=int, default=1, help="Degree of the polynomial")
    parser.add_argument("--functname", type=str, default="Chebychev", help="Polynomial type (Chebychev, Bernstein, or Power)")
    parser.add_argument("--x0", type=float, default=0, help="x0 value")

    args = parser.parse_args()
    polynomial_fit(args.mc, args.xmin, args.xmax, args.binning, args.poldegree, args.functname, args.x0)