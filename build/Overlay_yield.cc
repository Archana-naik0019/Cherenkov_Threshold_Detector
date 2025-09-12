#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TColor.h>
#include <vector>
#include <string>

TFile *f1 = new TFile("output_2.root");
TFile *f2 = new TFile("output_3.root");
TFile *f3 = new TFile("output_4.root");
TFile *f4 = new TFile("output_5.root");
TFile *f5 = new TFile("output_6.root");
TFile *f6 = new TFile("output_7.root");
TFile *f7 = new TFile("output_8.root");
TFile *f8 = new TFile("output_9.root");
TFile *f9 = new TFile("output_10.root");
TFile *f10 = new TFile("output_11.root");
TFile *f11 = new TFile("output_12.root");
TFile *f12 = new TFile("output_13.root");
TFile *f13 = new TFile("output_14.root");
TFile *f14 = new TFile("output_15.root");
TFile *f15 = new TFile("output_16.root");

TH1D *h1 = (TH1D*)f1->Get("hYield");
TH1D *h2 = (TH1D*)f2->Get("hYield");
TH1D *h3 = (TH1D*)f3->Get("hYield");
TH1D *h4 = (TH1D*)f4->Get("hYield");
TH1D *h5 = (TH1D*)f5->Get("hYield");
TH1D *h6 = (TH1D*)f6->Get("hYield");
TH1D *h7 = (TH1D*)f7->Get("hYield");
TH1D *h8 = (TH1D*)f8->Get("hYield");
TH1D *h9 = (TH1D*)f9->Get("hYield");
TH1D *h10 = (TH1D*)f10->Get("hYield");
TH1D *h11 = (TH1D*)f11->Get("hYield");
TH1D *h12 = (TH1D*)f12->Get("hYield");
TH1D *h13 = (TH1D*)f13->Get("hYield");
TH1D *h14 = (TH1D*)f14->Get("hYield");
TH1D *h15 = (TH1D*)f15->Get("hYield");

std::vector<TH1D*> hists = {h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12, h13, h14, h15};
std::vector<std::string> pressures = {
    "2 atm", "3 atm", "4 atm", "5 atm", "6 atm", "7 atm", "8 atm", "9 atm",
    "10 atm", "11 atm", "12 atm", "13 atm", "14 atm", "15 atm", "16 atm"
};
std::vector<Color_t> colors = {
    kRed, kBlue, kGreen, kBlack, kBrown, kCyan, kPink, kGray,
    kYellow, kViolet, kTeal, kMagenta, kSpring, kOrange, kAzure
};

// Set canvas
TCanvas *c1 = new TCanvas("c1", "Overlay", 800, 800);

// Loop through and draw histograms
for (size_t i = 0; i < hists.size(); ++i) {
    hists[i]->Rebin(2); // Coarser binning
    hists[i]->SetLineColor(colors[i]);
    hists[i]->SetLineWidth(2);
    if (i == 0) {
        hists[i]->Draw("HIST");
        hists[i]->SetMaximum(50000); // Set Y axis max
    } else {
        hists[i]->Draw("HIST SAME");
    }
}

// Axis labels
h1->SetTitle("Yield(per mm muon path) at Different Pressures");
h1->GetXaxis()->SetTitle("Yield(generated photons per mm)");
h1->GetYaxis()->SetTitle("Entries");

// Create and fill legend (only pressure values)
TLegend *legend = new TLegend(0.65, 0.45, 0.88, 0.88);
legend->SetHeader("Pressure", "C");
legend->SetBorderSize(0);
legend->SetFillStyle(0);
legend->SetTextSize(0.025);

for (size_t j = 0; j < hists.size(); ++j) {
    legend->AddEntry(hists[j], pressures[j].c_str(), "l");
}

legend->Draw();
c1->SaveAs("Overlay_MuonEnergy.png");

