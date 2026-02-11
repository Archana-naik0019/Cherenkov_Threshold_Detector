#include <iostream>
#include <sstream>
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TStyle.h"
#include "TSystem.h"

void Overlay_Gen_PMT_Det_All() {
    gStyle->SetOptStat(1111); // Show entries, mean, RMS, etc.

    int canvasWidth = 800;
    int canvasHeight = 600;
    int offsetX = 50;   // horizontal spacing
    int offsetY = 50;   // vertical spacing

    int screenX = 0;
    int screenY = 0;
    int nCols = 3;      // Number of canvases per row before wrapping

    for (int i = 2; i <= 16; ++i) {
        std::ostringstream filename;
        filename << "cumulative_" << i << ".root";

        TFile* file = new TFile(filename.str().c_str());
        if (!file || file->IsZombie()) {
            std::cerr << "Error: Cannot open file " << filename.str() << std::endl;
            continue;
        }

        // Retrieve histograms
        TH1F* h1 = (TH1F*)file->Get("hGeneratedWavelength_4-fold");
        TH1F* h2 = (TH1F*)file->Get("hAtPMTWavelength_4-fold");
        TH1F* h3 = (TH1F*)file->Get("hDetectedWavelength_4-fold");

        if (!h1 || !h2 || !h3) {
            std::cerr << "Error: Missing histogram(s) in " << filename.str() << std::endl;
            file->Close();
            continue;
        }

        // Detach histograms from file so they persist after closing
        h1->SetDirectory(0);
        h2->SetDirectory(0);
        h3->SetDirectory(0);
        file->Close();

        // Unique canvas
        std::ostringstream cname, ctitle;
        cname << "c" << i;
        ctitle << "Photon Wavelength Overlay (Pressure " << i << " atm)";
        TCanvas* c1 = new TCanvas(cname.str().c_str(), ctitle.str().c_str(), screenX, screenY, canvasWidth, canvasHeight);

        // Update positions
        screenX += canvasWidth + offsetX;
        if ((i - 1) % nCols == 0 && i != 2) {
            screenX = 0;
            screenY += canvasHeight + offsetY;
        }

        // Style histograms
        h1->SetLineColor(kBlue);
        h2->SetLineColor(kRed);
        h3->SetLineColor(kGreen + 2);
        h1->SetLineWidth(2);
        h2->SetLineWidth(2);
        h3->SetLineWidth(2);

        h1->SetTitle(ctitle.str().c_str());
        h1->GetXaxis()->SetTitle("Wavelength (nm)");
        h1->GetYaxis()->SetTitle("Number of Photons");
        
        int rebinFactor = 5;
        h1->Rebin(rebinFactor);
        h2->Rebin(rebinFactor);
        h3->Rebin(rebinFactor);

        // Draw histograms
        h1->Draw("HIST");
        h2->Draw("HIST SAME");
        h3->Draw("HIST SAME");
        c1->Update();

        // Adjust stat boxes
        TPaveStats* st1 = (TPaveStats*)h1->FindObject("stats");
        TPaveStats* st2 = (TPaveStats*)h2->FindObject("stats");
        TPaveStats* st3 = (TPaveStats*)h3->FindObject("stats");

        if (st1) {
            st1->SetTextColor(kBlue);
            st1->SetX1NDC(0.78); st1->SetX2NDC(0.98);
            st1->SetY1NDC(0.78); st1->SetY2NDC(0.98);
        }
        if (st2) {
            st2->SetTextColor(kRed);
            st2->SetX1NDC(0.78); st2->SetX2NDC(0.98);
            st2->SetY1NDC(0.58); st2->SetY2NDC(0.78);
        }
        if (st3) {
            st3->SetTextColor(kGreen + 2);
            st3->SetX1NDC(0.78); st3->SetX2NDC(0.98);
            st3->SetY1NDC(0.38); st3->SetY2NDC(0.58);
        }

        // Add legend
        TLegend* legend = new TLegend(0.15, 0.75, 0.35, 0.9);
        legend->AddEntry(h1, "Generated", "l");
        legend->AddEntry(h2, "Reached PMT", "l");
        legend->AddEntry(h3, "Detected", "l");
        legend->Draw();

        c1->Update();

        // Optional: Save each plot
        // std::ostringstream outname;
        // outname << "Overlay_Run" << i << ".png";
        // c1->SaveAs(outname.str().c_str());
    }

    std::cout << "✅ All plots created successfully." << std::endl;
}

