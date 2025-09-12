#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TString.h"
#include "TStyle.h"
#include <iostream>

void PlotMuonPhotonHistos() {
    // Open ROOT file
    TFile *f = TFile::Open("below_threshold_12_100000eve.root"); // change filename if needed
    if (!f || f->IsZombie()) {
        std::cerr << "Error: could not open ROOT file!" << std::endl;
        return;
    }

    gStyle->SetOptStat(1110); // show stats box with entries, mean, stddev

    // ==========================
    // 1) MuonEnergyWithGeneratedCherenkov
    // ==========================
    TH1 *hMuonE = (TH1*)f->Get("MuonEnergyWithGeneratedCherenkov");
    if (hMuonE) {
        TCanvas *c1 = new TCanvas("c1", "Muon Energy with Generated Cherenkov", 800, 600);
        hMuonE->SetTitle("Muon Energy with Generated Cherenkov");
        hMuonE->GetXaxis()->SetTitle("Muon Energy [GeV]");
        hMuonE->GetYaxis()->SetTitle("Events");
        hMuonE->SetLineColor(kBlue);
        hMuonE->SetLineWidth(2);
        hMuonE->Draw("HIST");
        c1->SaveAs("MuonEnergyWithGeneratedCherenkov.png");
    } else {
        std::cerr << "Histogram MuonEnergyWithGeneratedCherenkov not found!" << std::endl;
    }

    // ==========================
    // 2) primPhotonVsEnergy
    // ==========================
    TH1 *hPrim = (TH1*)f->Get("primPhotonVsEnergy");
    if (hPrim) {
        TCanvas *c2 = new TCanvas("c2", "Primary Photon vs Energy", 800, 600);
        hPrim->SetTitle("Primary Photon Yield vs Muon Energy");
        hPrim->GetXaxis()->SetTitle("Muon Energy [GeV]");
        hPrim->GetYaxis()->SetTitle("Primary Photons");
        hPrim->SetLineColor(kRed);
        hPrim->SetLineWidth(2);
        hPrim->Draw("HIST");
        c2->SaveAs("primPhotonVsEnergy.png");
    } else {
        std::cerr << "Histogram primPhotonVsEnergy not found!" << std::endl;
    }

    // ==========================
    // 3) secPhotonVsEnergy
    // ==========================
    TH1 *hSec = (TH1*)f->Get("secPhotonVsEnergy");
    if (hSec) {
        TCanvas *c3 = new TCanvas("c3", "Secondary Photon vs Energy", 800, 600);
        hSec->SetTitle("Secondary Photon Yield vs Muon Energy");
        hSec->GetXaxis()->SetTitle("Muon Energy [GeV]");
        hSec->GetYaxis()->SetTitle("Secondary Photons");
        hSec->SetLineColor(kGreen+2);
        hSec->SetLineWidth(2);
        hSec->Draw("HIST");
        c3->SaveAs("secPhotonVsEnergy.png");
    } else {
        std::cerr << "Histogram secPhotonVsEnergy not found!" << std::endl;
    }

    f->Close();
}

