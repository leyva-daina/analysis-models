import os
import sys
import csv

from ROOT import TCanvas, TPad, TLegend, TGraphAsymmErrors, TFile, TH1F, TLine
import ROOT
from array import array

def read_file(file_path):

    with open(file_path, 'r') as file:
        for line in file:
            if line.startswith('#') or line.startswith('X'):
                continue
            values = [float(val) for val in line.strip().split('&')]
            data["mass"].append(values[0])
            data["nominal"].append(values[3])
            data["up"].append(values[5])
            data["down"].append(values[1])

    return data

def calculate_systematics(ratio_up, ratio_down):
    systematics_up = [u / n for u, n in zip(ratio_up, ratio_down)]
    systematics_down = [d / n for d, n in zip(ratio_down, ratio_up)]
    return systematics_down, systematics_up

def create_csv_file(model_name, sys_name, year, x_array, ratio_down, ratio_up):
    csv_filename = "{}_{}_{}{}.csv".format(model_name, sys_name, year, channel)

    with open(csv_filename, mode='w') as csv_file:
        csv_writer = csv.writer(csv_file)
        #csv_writer.writerow(["Higgs_mass_GeV", "systematics_down", "systematics_up"])
        csv_writer.writerow(["Higgs_mass_GeV", "systematics_symmetric", "systematics_down", "systematics_up"])

        for mass, down, up in zip(x_array, ratio_down, ratio_up):
            systematics_symmetric = 1+(down - up)/2
            systematics_symmetric = round(systematics_symmetric, 3)
            #csv_writer.writerow([mass, down, up])
            down = round(down, 3)
            up = round(up, 3)
            csv_writer.writerow([mass, systematics_symmetric, down, up])

    print("Created csv file " + csv_filename)

def create_plot(input_path, sys_name="pdfas", model_name="mhmodp_200", year="2018"):
    canvas = TCanvas("canvas", "canvas", 800, 600)
    
    pad1 = TPad("pad1", "pad1", 0, 0.3, 1, 1)
    pad1.SetBottomMargin(0.03)
    pad1.Draw()
    pad1.cd()

    legend = TLegend(0.6, 0.12, 0.88, 0.38)
    legend.SetHeader(model_name + ", " +  sys_name + " unc." )
    
    if year == 2018:
        file_nominal = os.path.join(input_path, "both_{}_13TeV_{}_nominal.txt".format(model_name, year, sys_name))
        file_up      = os.path.join(input_path, "both_{}_13TeV_{}_{}up.txt".format(model_name, year, sys_name))
        file_down    = os.path.join(input_path, "both_{}_13TeV_{}_{}down.txt".format(model_name, year, sys_name))
    else:
        file_nominal = os.path.join(input_path, "three_{}_13TeV_{}{}_nominal.txt".format(model_name, year, channel, sys_name))
        file_up      = os.path.join(input_path, "three_{}_13TeV_{}{}_{}up.txt".format(model_name, year, channel, sys_name))
        file_down    = os.path.join(input_path, "three_{}_13TeV_{}{}_{}down.txt".format(model_name, year, channel, sys_name))

    data_nominal = {"X": [], "-2s": [], "-1s": [], "median": [], "+1s": [], "+2s": []}
    data_up      = {"X": [], "-2s": [], "-1s": [], "median": [], "+1s": [], "+2s": []}
    data_down    = {"X": [], "-2s": [], "-1s": [], "median": [], "+1s": [], "+2s": []}

    n_masspoints = 0

    with open(file_nominal, 'r') as file:
        for line in file:
            if line.startswith('#') or line.startswith('X'):
                continue
            values = [float(val) for val in line.strip().split('&')]
            data_nominal["X"].append(values[0])
            data_nominal["median"].append(values[3])
            n_masspoints += 1
    
    with open(file_up, 'r') as file:
        for line in file:
            if line.startswith('#') or line.startswith('X'):
                continue
            values = [float(val) for val in line.strip().split('&')]
            data_up["X"].append(values[0])
            data_up["median"].append(values[3])
            
    
    
    with open(file_down, 'r') as file:
        for line in file:
            if line.startswith('#') or line.startswith('X'):
                continue
            values = [float(val) for val in line.strip().split('&')]
            data_down["X"].append(values[0])
            data_down["median"].append(values[3])
    
    x_array = array('f', data_nominal["X"])
    nominal = array('f', data_nominal["median"])
    down = array('f', data_down["median"])
    up   = array('f', data_up["median"])

    print(x_array)

    h_nominal = ROOT.TGraph(len(x_array)-1, x_array, nominal)
    h_down = ROOT.TGraph(len(x_array)-1, x_array, up)
    h_up = ROOT.TGraph(len(x_array)-1, x_array, down)

    #for i in range(len(x_array)):
    #    print (x_array[i])
    #    print (nominal[i])
    #    h_nominal.SetBinContent(i+1, nominal[i])
    #    h_down.SetBinContent(i+1, down[i])
    #    h_up.SetBinContent(i+1, up[i])

    h_up.GetXaxis().SetTitle("#font[12]{M_{#Phi}}")
    h_down.GetXaxis().SetTitle("#font[12]{M_{#Phi}}")
    h_nominal.GetXaxis().SetTitle("#font[12]{M_{#Phi}}")
    h_nominal.GetYaxis().SetTitle("tan #beta")
    h_nominal.GetYaxis().SetRangeUser(0, 60)
    h_nominal.GetYaxis().SetLabelSize(0.05)  
    h_nominal.GetXaxis().SetLabelSize(0.)  
    h_nominal.GetYaxis().SetTitleSize(0.05)  
    h_nominal.GetYaxis().SetTitleOffset(0.7)  
    h_nominal.GetXaxis().SetTitleSize(0.12)  
    
    h_nominal.SetLineColor(1)
    h_up.SetLineColor(4)  # Red color
    h_down.SetLineColor(2)  # Blue color

    h_nominal.SetLineWidth(2)
    h_up.SetLineWidth(2)
    h_down.SetLineWidth(2)
    
    h_nominal.GetXaxis().SetRangeUser(x_array[0], x_array[len(x_array)-1])
    h_down.GetXaxis().SetRangeUser(x_array[0], x_array[len(x_array)-1])
    h_up.GetXaxis().SetRangeUser(x_array[0], x_array[len(x_array)-1])

    h_nominal.SetStats(0)
    h_up.SetStats(0)
    h_down.SetStats(0)

    print(x_array[0], x_array[len(x_array)-1])
    
    mg = ROOT.TMultiGraph()
    mg.Add(h_nominal, "AL")
    mg.Add(h_up, "AL")
    mg.Add(h_down, "AL")

    mg.Draw("A")
    mg.GetXaxis().SetTitle("#font[12]{M_{#Phi}}")
    mg.GetYaxis().SetTitle("tan #beta")
    mg.GetYaxis().SetRangeUser(0, 60)
    mg.GetYaxis().SetLabelSize(0.05)  
    mg.GetXaxis().SetLabelSize(0.)  
    mg.GetYaxis().SetTitleSize(0.05)  
    mg.GetYaxis().SetTitleOffset(0.7)  
    mg.GetXaxis().SetTitleSize(0.12)  
    mg.GetXaxis().SetRangeUser(x_array[0], x_array[len(x_array)-1])


    legend.AddEntry(h_nominal, "Nominal", "l")
    legend.AddEntry(h_up, "Up Variation", "l")
    legend.AddEntry(h_down, "Down Variation", "l")
    
    legend.Draw()
    
    canvas.cd()

    pad2 = TPad("pad2", "pad2", 0, 0, 1, 0.3)
    pad2.SetTopMargin(0.01)
    pad2.SetBottomMargin(0.35)
    #pad2.SetLeftMargin(0.25)
    pad2.Draw()
    pad2.cd()

    ratio_up = [u / n for u, n in zip(data_up["median"], data_nominal["median"])]
    ratio_down = [d / n for d, n in zip(data_down["median"], data_nominal["median"])]


    h_ratio_up = ROOT.TGraph(len(x_array), x_array, array('f', ratio_up))
    h_ratio_down = ROOT.TGraph(len(x_array), x_array, array('f', ratio_down))

    h_ratio_up.GetYaxis().SetTitle("up(down)/central")
    h_ratio_up.GetYaxis().SetRangeUser(0.78, 1.22)  # Adjust the range as needed
    h_ratio_up.GetYaxis().SetNdivisions(505)  # Set the number of divisions explicitly
    h_ratio_up.GetYaxis().SetLabelSize(0.10)  
    h_ratio_up.GetXaxis().SetLabelSize(0.10)  
    h_ratio_up.GetYaxis().SetTitleSize(0.10)  
    h_ratio_up.GetYaxis().SetTitleOffset(0.35)  
    h_ratio_up.GetXaxis().SetTitleSize(0.12)  
    h_ratio_up.SetMarkerColor(4)  
    h_ratio_up.SetLineColor(4)  
    h_ratio_down.SetMarkerColor(2)  
    h_ratio_down.SetLineColor(2)  

    mg_ratio = ROOT.TMultiGraph()
    mg_ratio.Add(h_ratio_up, "AL")
    mg_ratio.Add(h_ratio_down, "AL")

    mg_ratio.Draw("A")
    mg_ratio.GetXaxis().SetTitle("#font[12]{M_{#Phi}}")
    mg_ratio.GetYaxis().SetTitle("up(down)/central")
    mg_ratio.GetYaxis().SetRangeUser(0.78, 1.22)
    mg_ratio.GetYaxis().SetNdivisions(505)
    mg_ratio.GetYaxis().SetLabelSize(0.10)  
    mg_ratio.GetXaxis().SetLabelSize(0.10)  
    mg_ratio.GetYaxis().SetTitleSize(0.10)  
    mg_ratio.GetYaxis().SetTitleOffset(0.35)  
    mg_ratio.GetXaxis().SetTitleSize(0.12)  
    mg_ratio.GetXaxis().SetRangeUser(x_array[0], x_array[len(x_array)-1])  

    line = TLine(x_array[0], 1, x_array[len(x_array)-1], 1)
    line.SetLineStyle(2)
    line.Draw()

    # Calculate systematics
    systematics_down, systematics_up = calculate_systematics(ratio_up, ratio_down)

    create_csv_file(model_name, sys_name, year, x_array, ratio_down, ratio_up)
    # Save to CSV
    #csv_filename = "{}_{}_{}.csv".format(model_name, sys_name, year)
    #with open(csv_filename, mode='w') as csv_file:
    #    csv_writer = csv.writer(csv_file)
    #    csv_writer.writerow(['Higgs_mass_GeV', 'systematics_down', 'systematics_up'])
    #    for mass, sys_down, sys_up in zip(data_nominal["X"], systematics_down, systematics_up):
    #        csv_writer.writerow([mass, sys_down, sys_up])

   # print("Results saved to " + csv_filename)
    
    canvas.SaveAs("output_{}_{}_{}{}.pdf".format(sys_name, model_name, year, channel))
    canvas.SaveAs("output_{}_{}_{}{}.png".format(sys_name, model_name, year, channel))


if __name__ == "__main__":
    input_path = sys.argv[1]
    sys_name = sys.argv[2] if len(sys.argv) > 2 else "pdfas"
    model_name = sys.argv[3] if len(sys.argv) > 3 else "mhmodp_200"
    year = sys.argv[4] if len(sys.argv) > 4 else "2018"
    channel = sys.argv[5] if len(sys.argv) > 5 else "FH"

    if not input_path:
        print("No matching files found.")
    else:
        create_plot(input_path, sys_name, model_name, year)
