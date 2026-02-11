#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TLegend.h>
#include <TStyle.h>

void plotMuonKE() {
    // Open the ROOT file
    TFile *f = TFile::Open("EnergyDistOfMuonsShot.root");
    if (!f || f->IsZombie()) {
        printf("Error opening file!\n");
        return;
    }

    // Get the histogram
    TH1* h1 = (TH1*)f->Get("MuonKE");
    if (!h1) {
        printf("Histogram 'MuonKE' not found!\n");
        return;
    }

    // Create canvas
    TCanvas *c1 = new TCanvas("c1", "Muon Energy with Threshold Cuts", 800, 600);
    gStyle->SetOptStat(0);

    h1->SetLineColor(kBlue+2);
    h1->SetLineWidth(2);
    h1->GetXaxis()->SetTitle("Muon Energy (GeV)");
    h1->GetYaxis()->SetTitle("#Events");
    h1->Draw("HIST");

    // Threshold values from your table (Pressure 2–16 atm)
    double thresholds[15] = {
        3.115, 2.536, 2.176, 1.935, 1.786,
        1.625, 1.515, 1.425, 1.336, 1.285,
        1.215, 1.167, 1.126, 1.075, 1.045
    };
    int pressures[15] = {
        2, 3, 4, 5, 6,
        7, 8, 9, 10, 11,
        12, 13, 14, 15, 16
    };

    // Nice set of ROOT colors
    int colors[15] = {
        kRed, kGreen+2, kMagenta, kOrange+1, kCyan+2,
        kViolet, kPink+7, kAzure+1, kTeal+2, kSpring+5,
        kYellow+2, kGray+1, kCyan, kBlack, kOrange
    };

    // Add vertical lines
    double ymax = h1->GetMaximum() * 1.05;
    TLine *lines[15];
    for (int i=0; i<15; i++) {
        lines[i] = new TLine(thresholds[i], 0, thresholds[i], ymax);
        lines[i]->SetLineColor(colors[i]);
        lines[i]->SetLineStyle(1); // dashed
        lines[i]->SetLineWidth(2);
        lines[i]->Draw("same");
    }

    // Add legend
    TLegend *leg = new TLegend(0.7, 0.4, 0.9, 0.9);
    leg->SetHeader("Thresholds","C");
    for (int i=0; i<15; i++) {
        TString entry = Form("%d atm: %.3f GeV", pressures[i], thresholds[i]);
        leg->AddEntry(lines[i], entry, "l");
    }
    leg->Draw();

    // Save output
    c1->SaveAs("MuonKE_with_thresholds.png");
}

