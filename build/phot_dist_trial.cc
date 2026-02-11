#include <vector>
#include <utility>
#include <iostream>

#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

void phot_dist_trial()
{
    gStyle->SetOptStat(0);

    // -----------------------------
    // USER INPUT
    // -----------------------------
    const char* filename = "2D_EandPhotDist_16atm.root";
    const char* h2name   = "Energy_vs_DetectedPhotons";

    std::vector<std::pair<double,double>> energy_bins = {
        {1.045, 1.285},
        {1.285, 1.336},
        {1.336, 1.425},
        {1.425, 1.515},
        {1.515, 1.625},
        {1.625, 1.786},
        {1.786, 1.935},
        {1.935, 2.176},
        {2.176, 2.536},
        {2.536, 3.115},
        {3.115, -1.0}   // open-ended
    };

    // -----------------------------
    // OPEN FILE
    // -----------------------------
    TFile* f = TFile::Open(filename);
    if (!f || f->IsZombie()) {
        std::cerr << "ERROR: Cannot open file " << filename << std::endl;
        return;
    }

    TH2D* h2 = (TH2D*)f->Get(h2name);
    if (!h2) {
        std::cerr << "ERROR: Histogram " << h2name << " not found" << std::endl;
        return;
    }

    // -----------------------------
    // CANVAS + LEGEND
    // -----------------------------
    TCanvas* c = new TCanvas("c", "Detected photons vs muon energy slices", 900, 700);
    c->SetLogy();   // comment out if you don't want log-scale

    TLegend* leg = new TLegend(0.55, 0.55, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    int colors[] = {
        kRed, kBlue, kGreen+2, kMagenta,
        kOrange+7, kCyan+2, kViolet,
        kSpring+5, kPink+7, kTeal+2, kBlack
    };

    bool first = true;

    // -----------------------------
    // LOOP OVER ENERGY BINS
    // -----------------------------
    for (size_t i = 0; i < energy_bins.size(); ++i) {

        double Emin = energy_bins[i].first;
        double Emax = energy_bins[i].second;

        int xbin_min = h2->GetXaxis()->FindBin(Emin);
        int xbin_max;

        TString label;
        if (Emax < 0) {
            xbin_max = h2->GetXaxis()->GetNbins();
            label = Form("E_{#mu} > %.3f GeV", Emin);
        } else {
            xbin_max = h2->GetXaxis()->FindBin(Emax - 1e-6);
            label = Form("%.3f–%.3f GeV", Emin, Emax);
        }

        TH1D* hproj = h2->ProjectionY(
            Form("h_det_%zu", i),
            xbin_min,
            xbin_max
        );

        if (hproj->Integral() <= 0) continue;

        // Normalize
        hproj->Scale(1.0 / hproj->Integral());

        hproj->SetLineColor(colors[i % 11]);
        hproj->SetLineWidth(2);

        hproj->GetXaxis()->SetTitle("Detected photons per event");
        hproj->GetYaxis()->SetTitle("Normalized counts");

        if (first) {
            hproj->Draw("HIST");
            first = false;
        } else {
            hproj->Draw("HIST SAME");
        }

        leg->AddEntry(hproj, label, "l");
    }

    // -----------------------------
    // FINALIZE
    // -----------------------------
    leg->Draw();
    c->Update();
    c->SaveAs("DetectedPhoton_Distributions_EnergySlices.png");
}

