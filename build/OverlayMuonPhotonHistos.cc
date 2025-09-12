#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include <iostream>

void OverlayMuonPhotonHistos() {
    // Open ROOT file
    TFile *f = TFile::Open("below_threshold_12_100000eve.root"); // change filename if needed
    if (!f || f->IsZombie()) {
        std::cerr << "Error: could not open ROOT file!" << std::endl;
        return;
    }

    gStyle->SetOptStat(0); // turn off stats box for clarity

    // Retrieve histograms
    TH1 *hMuonE = (TH1*)f->Get("MuonEnergyWithGeneratedCherenkov");
    TH1 *hPrim  = (TH1*)f->Get("primPhotonVsEnergy");
    TH1 *hSec   = (TH1*)f->Get("secPhotonVsEnergy");

    if (!hMuonE || !hPrim || !hSec) {
        std::cerr << "Error: one or more histograms not found!" << std::endl;
        return;
    }

    // Create canvas
    TCanvas *c = new TCanvas("c", "Overlay of Muon and Photon Histograms", 900, 700);

    // Style histograms
    hMuonE->SetLineColor(kBlue);
    hMuonE->SetLineWidth(2);
    hMuonE->SetTitle("Muon and Photon Energy Distributions");
    hMuonE->GetXaxis()->SetTitle("Muon Energy [GeV]");
    hMuonE->GetYaxis()->SetTitle("Entries");

    hPrim->SetLineColor(kRed);
    hPrim->SetLineWidth(2);

    hSec->SetLineColor(kGreen+2);
    hSec->SetLineWidth(2);

    // Normalize histograms (optional, so they can be compared)
    //hMuonE->Scale(1.0 / hMuonE->Integral());
    //hPrim->Scale(1.0 / hPrim->Integral());
    //hSec->Scale(1.0 / hSec->Integral());

    // Draw first histogram and overlay others
    hMuonE->Draw("HIST");
    hPrim->Draw("HIST SAME");
    hSec->Draw("HIST SAME");

    // Add legend
    TLegend *leg = new TLegend(0.65, 0.7, 0.9, 0.9);
    leg->AddEntry(hMuonE, "Muon Energy w/ Cherenkov", "l");
    leg->AddEntry(hPrim,  "Primary Photons vs Energy", "l");
    leg->AddEntry(hSec,   "Secondary Photons vs Energy", "l");
    leg->Draw();

    // Save output
    //c->SaveAs("Overlay_MuonPhotonHistos.png");
    //c->SaveAs("Overlay_MuonPhotonHistos.pdf");

    //f->Close();
}

