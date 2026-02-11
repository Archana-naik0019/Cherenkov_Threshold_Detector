#include <iostream>
#include <sstream>
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TSystem.h"

void Overlay_All_Pressures(bool normalize = true) {
    gStyle->SetOptStat(1111);  // show stats box
    gStyle->SetLegendBorderSize(0);

    const int firstFile = 2;
    const int lastFile = 16;
    int rebinFactor = 10;  // adjust for coarser bins if needed

    // Colors for up to 15 files
    int colors[] = {kBlue, kRed, kGreen+2, kMagenta, kCyan+2, kOrange+1, kViolet,
                    kGray+2, kAzure+2, kTeal+3, kPink+2, kSpring+5, kYellow+2, kOrange+7, kBlack};
    
    //normalization                
    auto normalizeHist = [&](TH1F* h){
    if (normalize) {
        h->Sumw2(); // ensure proper stat handling
        double integral = h->Integral(0, h->GetNbinsX() + 1); // include under/overflow bins
        if (integral > 0) {
            h->Scale(1.0 / integral);
        } else {
            std::cerr << "⚠ Warning: Zero integral, normalization skipped\n";
        }
    }
};


    // --- Plot all Generated histograms ---
    TCanvas* cGen = new TCanvas("cGen", "All Generated Wavelengths", 800, 600);
    TLegend* legGen = new TLegend(0.7, 0.65, 0.9, 0.9);
    bool firstDrawn = false;

    for (int i = firstFile; i <= lastFile; ++i) {
        if ( i != 6 && i != 13 ) continue;
        std::ostringstream filename;
        filename << "cumulative_" << i << ".root";
        TFile* file = new TFile(filename.str().c_str());

        if (!file || file->IsZombie()) {
            std::cerr << "Skipping " << filename.str() << " (cannot open)\n";
            continue;
        }

        TH1F* h = (TH1F*)file->Get("hGeneratedWavelength_4-fold");
        if (!h) {
            std::cerr << "Skipping " << filename.str() << " (missing histogram)\n";
            file->Close();
            continue;
        }

        h->SetDirectory(0);
        file->Close();

        if (rebinFactor > 1) h->Rebin(rebinFactor);
        
        normalizeHist(h);

        
        h->SetLineColor(colors[(i - firstFile) % 15]);
        h->SetLineWidth(2);
        h->SetTitle("Generated Photon Wavelengths (All Pressures)");
        h->GetXaxis()->SetTitle("Wavelength (nm)");
        h->GetYaxis()->SetTitle("Number of Photons");
        h->GetYaxis()->SetRangeUser(0, 1);


        if (!firstDrawn) {
            h->Draw("HIST");
            firstDrawn = true;
        } else {
            h->Draw("HIST SAME");
        }

        std::ostringstream legLabel;
        legLabel << i << " atm";
        legGen->AddEntry(h, legLabel.str().c_str(), "l");
    }
    legGen->Draw();
    cGen->Update();


    // --- Plot all PMT histograms ---
    TCanvas* cPMT = new TCanvas("cPMT", "All PMT Wavelengths", 800, 600);
    TLegend* legPMT = new TLegend(0.7, 0.65, 0.9, 0.9);
    firstDrawn = false;

    for (int i = firstFile; i <= lastFile; ++i) {
        if ( i != 6 && i != 13 ) continue;
        std::ostringstream filename;
        filename << "cumulative_" << i << ".root";
        TFile* file = new TFile(filename.str().c_str());
        if (!file || file->IsZombie()) continue;

        TH1F* h = (TH1F*)file->Get("hAtPMTWavelength_4-fold");
        if (!h) {
            file->Close();
            continue;
        }

        h->SetDirectory(0);
        file->Close();

        if (rebinFactor > 1) h->Rebin(rebinFactor);
        
        normalizeHist(h);
        
        h->SetLineColor(colors[(i - firstFile) % 15]);
        h->SetLineWidth(2);
        h->SetTitle("Photon Wavelengths Reaching PMT (All Pressures)");
        h->GetXaxis()->SetTitle("Wavelength (nm)");
        h->GetYaxis()->SetTitle("Number of Photons");
        h->GetYaxis()->SetRangeUser(0, 1);

        if (!firstDrawn)
            h->Draw("HIST"), firstDrawn = true;
        else
            h->Draw("HIST SAME");

        std::ostringstream legLabel;
        legLabel << i << " atm";
        legPMT->AddEntry(h, legLabel.str().c_str(), "l");
    }
    legPMT->Draw();
    cPMT->Update();


    // --- Plot all Detected histograms ---
    TCanvas* cDet = new TCanvas("cDet", "All Detected Wavelengths", 800, 600);
    TLegend* legDet = new TLegend(0.7, 0.65, 0.9, 0.9);
    firstDrawn = false;

    for (int i = firstFile; i <= lastFile; ++i) {
        if ( i != 6 && i != 13 ) continue;
        std::ostringstream filename;
        filename << "cumulative_" << i << ".root";
        TFile* file = new TFile(filename.str().c_str());
        if (!file || file->IsZombie()) continue;

        TH1F* h = (TH1F*)file->Get("hDetectedWavelength_4-fold");
        if (!h) {
            file->Close();
            continue;
        }

        h->SetDirectory(0);
        file->Close();

        if (rebinFactor > 1) h->Rebin(rebinFactor);
        
        normalizeHist(h);
        
        h->SetLineColor(colors[(i - firstFile) % 15]);
        h->SetLineWidth(2);
        h->SetTitle("Detected Photon Wavelengths (All Pressures)");
        h->GetXaxis()->SetTitle("Wavelength (nm)");
        h->GetYaxis()->SetTitle("Number of Photons");
        h->GetYaxis()->SetRangeUser(0, 1);

        if (!firstDrawn)
            h->Draw("HIST"), firstDrawn = true;
        else
            h->Draw("HIST SAME");

        std::ostringstream legLabel;
        legLabel << i << " atm";
        legDet->AddEntry(h, legLabel.str().c_str(), "l");
    }
    legDet->Draw();
    cDet->Update();

    std::cout << "Overlays complete: Generated / PMT / Detected (across all pressures)" << std::endl;
}

