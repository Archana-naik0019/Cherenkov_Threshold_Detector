#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TString.h"
#include "TColor.h"
#include <vector>
#include <iostream>

void OverlayMuonEnergy() {
    gROOT->SetBatch(kTRUE); // no pop-ups
    gStyle->SetOptStat(0);

    const int rebinFactor = 50;
    const int nFiles = 15;

    std::vector<TString> histNames = {
        "MuonEnergyforAll_4-fold",
        "MuonEnergyWithDetectedCherenkov_4-fold_1PE",
        "MuonEnergyWithDetectedCherenkov_4-fold_4PE"
    };

    std::vector<int> colors = {kRed, kBlue, kGreen + 2, kMagenta, kOrange + 1, kCyan + 1, kViolet, kAzure - 3, kPink + 7};

    // === STEP 1: Per-file overlays ===
    for (int i = 2; i <= 16; ++i) {
        TString filename = Form("cumulative_%d.root", i);
        TFile *f = TFile::Open(filename);
        if (!f || f->IsZombie()) {
            std::cerr << "Cannot open " << filename << std::endl;
            continue;
        }

        TCanvas *c = new TCanvas(Form("cumulative_%d_canvas", i), "", 800, 600);
        TLegend *leg = new TLegend(0.6, 0.65, 0.88, 0.88);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);

        bool first = true;
        int colorIndex = 0;

        for (auto &hname : histNames) {
            TH1D *h = (TH1D*)f->Get(hname);
            if (!h) { std::cerr << "Missing " << hname << " in " << filename << std::endl; continue; }

            h->Rebin(rebinFactor);
            h->SetLineColor(colors[colorIndex % colors.size()]);
            h->SetLineWidth(2);
            h->SetTitle(Form("Overlay of %s histograms (file %d)", filename.Data(), i));
            if (first) {
                h->Draw("HIST");
                first = false;
            } else {
                h->Draw("HIST SAME");
            }
            leg->AddEntry(h, hname, "l");
            colorIndex++;
        }

        leg->Draw();
        c->SaveAs(Form("Overlay_cumulative_%d.png", i));
        f->Close();
    }

// === STEP 2: Grand overlay (all 3×15 + MuonKE) ===
TCanvas *grand = new TCanvas("GrandOverlay", "Grand Overlay", 1000, 800);
TLegend *legAll = new TLegend(0.6, 0.55, 0.88, 0.88);
legAll->SetBorderSize(0);
legAll->SetFillStyle(0);

bool firstDraw = true;
int colorCount = 0;
std::vector<TH1D*> storedHists; // keep clones alive

for (int i = 2; i <= 16; ++i) {
    TString filename = Form("cumulative_%d.root", i);
    TFile *f = TFile::Open(filename);
    if (!f || f->IsZombie()) continue;

    for (auto &hname : histNames) {
        TH1D *h = (TH1D*)f->Get(hname);
        if (!h) continue;
        TH1D *hclone = (TH1D*)h->Clone(Form("%s_%d_clone", hname.Data(), i));
        hclone->SetDirectory(0); // detach from file
        hclone->Rebin(rebinFactor);
        hclone->SetLineColor(colors[colorCount % colors.size()]);
        hclone->SetLineWidth(2);
        hclone->SetTitle("Grand Overlay of All Histograms");

        if (firstDraw) {
            hclone->Draw("HIST");
            firstDraw = false;
        } else {
            hclone->Draw("HIST SAME");
        }
        legAll->AddEntry(hclone, Form("%s (file %d)", hname.Data(), i), "l");
        storedHists.push_back(hclone);
        colorCount++;
    }

    f->Close();
}

// Add MuonKE from MuonKE_extended.root
TFile *fMuonKE = TFile::Open("MuonKE_extended.root");
if (fMuonKE && !fMuonKE->IsZombie()) {
    TH1D *hMuonKE = (TH1D*)fMuonKE->Get("MuonKE");
    if (hMuonKE) {
        TH1D *hClone = (TH1D*)hMuonKE->Clone("MuonKE_clone");
        hClone->SetDirectory(0);
        hClone->Rebin(rebinFactor);
        hClone->SetLineColor(kBlack);
        hClone->SetLineWidth(3);
        hClone->Draw("HIST SAME");
        legAll->AddEntry(hClone, "MuonKE (extended)", "l");
        storedHists.push_back(hClone);
    }
    fMuonKE->Close();
}

legAll->Draw();
grand->SaveAs("GrandOverlay_AllFiles.png");
}
