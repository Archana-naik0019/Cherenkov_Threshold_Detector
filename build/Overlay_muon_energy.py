import ROOT

# Define ROOT files and corresponding pressure labels
files = [f"output_{i}.root" for i in range(2, 17)]
pressures = [f"{i} atm" for i in range(2, 17)]

colors = [
    ROOT.kRed, ROOT.kBlue, ROOT.kGreen+2, ROOT.kMagenta+2, ROOT.kCyan+2,
    ROOT.kOrange+7, ROOT.kViolet+2, ROOT.kTeal+3, ROOT.kPink+10, ROOT.kAzure+7,
    ROOT.kSpring+10, ROOT.kYellow+2, ROOT.kGray+2, ROOT.kBlue+3, ROOT.kRed+3
]

histos = []
canvas = ROOT.TCanvas("c1", "Overlay Muon Energy", 1000, 700)
legend = ROOT.TLegend(0.65, 0.45, 0.88, 0.88)
legend.SetHeader("Muon Energy vs Pressure", "C")
legend.SetBorderSize(0)
legend.SetFillStyle(0)
legend.SetTextSize(0.025)

for i, file in enumerate(files):
    f = ROOT.TFile.Open(file)
    h = f.Get("MuonEnergyWithGeneratedCherenkov").Clone()
    h.SetDirectory(0)
    h.SetLineColor(colors[i])
    h.SetLineWidth(2)
    h.SetTitle("Muon Energy with Cherenkov Photons")
    h.GetXaxis().SetTitle("Muon Energy [MeV]")
    h.GetYaxis().SetTitle("Entries")
    
    # Get stats
    entries = int(h.GetEntries())
    mean = round(h.GetMean(), 2)
    overshoot = int(h.GetBinContent(h.GetNbinsX() + 1))
    undershoot = int(h.GetBinContent(0))
    
    label = f"{pressures[i]}: N={entries}, ⟨E⟩={mean}, O={overshoot}, U={undershoot}"
    legend.AddEntry(h, label, "l")
    
    histos.append(h)

# Draw all histograms overlaid
histos[0].Draw("HIST")
for h in histos[1:]:
    h.Draw("HIST SAME")

legend.Draw()
canvas.Update()
canvas.SaveAs("Overlay_MuonEnergy.png")

