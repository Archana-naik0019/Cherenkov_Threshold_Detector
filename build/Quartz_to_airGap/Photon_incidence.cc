#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TLegend.h>
#include <TStyle.h>
#include <vector>
#include <string>
#include <iostream>

void overlay_incidence_angle_180()
{
    gStyle->SetOptStat(0);

    // ---------------------------------
    // Canvas
    // ---------------------------------
    TCanvas* c = new TCanvas(
        "c",
        "Photon incidence distribution at quartz-air",
        900, 700
    );
    c->SetGrid();

    // ---------------------------------
    // Legend
    // ---------------------------------
    TLegend* leg = new TLegend(0.60, 0.60, 0.88, 0.88);
    leg->SetBorderSize(1);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);
    leg->SetHeader("Pressure", "C");

    // ---------------------------------
    // Colors
    // ---------------------------------
    std::vector<int> colors = {
        kBlack, kRed+1, kBlue+1, kGreen+2, kMagenta+1,
        kOrange+7, kCyan+1, kViolet+1, kTeal+1,
        kPink+1, kAzure+2, kSpring+5,
        kYellow+2, kGray+2, kRed+3
    };

    bool first = true;
    int colorIndex = 0;

    // ---------------------------------
    // Loop over ROOT files
    // ---------------------------------
    //for (int i = 2; i <= 16; ++i)
    for (int i = 2; i <= 16; i+= 4)
    {
        TString filename = Form("QuartzToAirGap_%d.root", i);
        TFile* f = TFile::Open(filename, "READ");

        if (!f || f->IsZombie()) {
            std::cerr << "Could not open " << filename << std::endl;
            continue;
        }

        TH1* hOrig =
            dynamic_cast<TH1*>(f->Get("hPhotonIncidenceAngle"));

        if (!hOrig) {
            std::cerr << "Histogram not found in " << filename << std::endl;
            f->Close();
            continue;
        }

        // ---------------------------------
        // Create transformed histogram
        // ---------------------------------
        TH1* hNew = (TH1*)hOrig->Clone(
            Form("hIncidence180_%d", i)
        );
        hNew->Reset();
        hNew->SetDirectory(0);

        // Fill with (180 - theta)
        for (int b = 1; b <= hOrig->GetNbinsX(); ++b) {
            double theta = hOrig->GetBinCenter(b);
            double content = hOrig->GetBinContent(b);

            double theta_new = 180.0 - theta;

            if (theta_new >= 0.0 && theta_new <= 90.0) {
                hNew->Fill(theta_new, content);
            }
        }

        // Normalize
        if (hNew->Integral() > 0)
            hNew->Scale(1.0 / hNew->Integral());

        // Styling
        int col = colors[colorIndex % colors.size()];
        hNew->SetLineColor(col);
        hNew->SetLineWidth(2);

        hNew->GetXaxis()->SetTitle("Photon incidence angle (deg)");
        hNew->GetYaxis()->SetTitle("Number of events");
        
        hNew->GetXaxis()->SetTitleSize(0.040);
        hNew->GetYaxis()->SetTitleSize(0.040);

        hNew->GetXaxis()->SetLabelSize(0.032);
        hNew->GetYaxis()->SetLabelSize(0.032);
        
        hNew->GetXaxis()->SetRangeUser(0.0, 90.0);
        //hNew->GetYaxis()->SetRangeUser(0.0, 20000.0);
        hNew->GetYaxis()->SetRangeUser(0.0, 0.1);

        // Draw
        if (first) {
            hNew->SetTitle("Photon incidence distribution at quartz-air");
            hNew->Draw("HIST");
            first = false;
        } else {
            hNew->Draw("HIST SAME");
        }

        leg->AddEntry(hNew, Form("%d atm", i), "l");

        f->Close();
        colorIndex++;
    }

    leg->Draw();

    c->SaveAs("selectiveNormalized_PhotonIncidenceAngle_180_overlay.pdf");
}

