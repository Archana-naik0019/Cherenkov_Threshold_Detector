#include <TFile.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <iostream>
#include <vector>
#include <cmath>

void plot_FoldRatio_vs_Pressure() {

    // Pressure points (atm)
    std::vector<double> pressure = {3,4,5,6,7,8,9,10,11,12,13,14,15,16};

    // Ratios (from Excel)
    std::vector<double> r1PE = {0.105740188,0.153614458,0.242811502,0.290322581,0.357142857,0.423766012,0.513678712,0.475342466,0.525157233,0.541582839,0.535384616,0.546153846,0.600609756,0.607250755};
    std::vector<double> r3PE = {0.045317221,0.114457831,0.169329073,0.203225806,0.279220779,0.355140187,0.419452888,0.392228295,0.433962264,0.482739093,0.464615385,0.461538462,0.527439024,0.549848943};
    std::vector<double> r4PE = {0.036253776,0.096385542,0.195744409,0.183870968,0.253246753,0.320872274,0.364741641,0.372990353,0.405660377,0.458471761,0.446153846,0.461538462,0.5,0.525697758};
    std::vector<double> r5PE = {0.018126888,0.078313253,0.14057508,0.161290323,0.237012987,0.305295995,0.331306991,0.363344051,0.380503145,0.408637874,0.430769231,0.481707317,0.481707317,0.501510574};

    // Arrays for errors
    std::vector<double> err1PE, err3PE, err4PE, err5PE;
    std::vector<double> errX(pressure.size(), 0.0);

    // Retrieve N from ROOT files
    for (size_t i = 0; i < pressure.size(); ++i) {
        int P = static_cast<int>(pressure[i]);
        TString filename = Form("sectioned_%d.root", P);
        TFile *f = TFile::Open(filename);
        if (!f || f->IsZombie()) {
            std::cerr << "Warning: Cannot open " << filename << ". Setting N=1." << std::endl;
            err1PE.push_back(0);
            err3PE.push_back(0);
            err4PE.push_back(0);
            err5PE.push_back(0);
            continue;
        }

        TH1 *h = (TH1*)f->Get("hGeneratedPhotons_4-fold");
        if (!h) {
            std::cerr << "Histogram not found in " << filename << std::endl;
            err1PE.push_back(0);
            err3PE.push_back(0);
            err4PE.push_back(0);
            err5PE.push_back(0);
            f->Close();
            continue;
        }

        double N = h->GetEntries();
        f->Close();

        // Compute binomial errors
        err1PE.push_back( sqrt(r1PE[i] * (1 - r1PE[i]) / N) );
        err3PE.push_back( sqrt(r3PE[i] * (1 - r3PE[i]) / N) );
        err4PE.push_back( sqrt(r4PE[i] * (1 - r4PE[i]) / N) );
        err5PE.push_back( sqrt(r5PE[i] * (1 - r5PE[i]) / N) );
    }

    // Create graphs
    TGraphErrors *g1PE = new TGraphErrors(pressure.size(), &pressure[0], &r1PE[0], &errX[0], &err1PE[0]);
    TGraphErrors *g3PE = new TGraphErrors(pressure.size(), &pressure[0], &r3PE[0], &errX[0], &err3PE[0]);
    TGraphErrors *g4PE = new TGraphErrors(pressure.size(), &pressure[0], &r4PE[0], &errX[0], &err4PE[0]);
    TGraphErrors *g5PE = new TGraphErrors(pressure.size(), &pressure[0], &r5PE[0], &errX[0], &err5PE[0]);

    // Style
    g1PE->SetMarkerStyle(20); g1PE->SetMarkerColor(kBlue); g1PE->SetLineColor(kBlue);
    g3PE->SetMarkerStyle(21); g3PE->SetMarkerColor(kGreen+2); g3PE->SetLineColor(kGreen+2);
    g4PE->SetMarkerStyle(22); g4PE->SetMarkerColor(kRed); g4PE->SetLineColor(kRed);
    g5PE->SetMarkerStyle(23); g5PE->SetMarkerColor(kMagenta); g5PE->SetLineColor(kMagenta);

    //line width
    g1PE->SetMarkerStyle(20); g1PE->SetMarkerColor(kBlue); g1PE->SetLineColor(kBlue); g1PE->SetLineWidth(2);
    g3PE->SetMarkerStyle(21); g3PE->SetMarkerColor(kGreen+2); g3PE->SetLineColor(kGreen+2); g3PE->SetLineWidth(2);
    g4PE->SetMarkerStyle(22); g4PE->SetMarkerColor(kRed); g4PE->SetLineColor(kRed); g4PE->SetLineWidth(2);
    g5PE->SetMarkerStyle(23); g5PE->SetMarkerColor(kMagenta); g5PE->SetLineColor(kMagenta); g5PE->SetLineWidth(2);



    // Canvas and plot
    TCanvas *c1 = new TCanvas("c1","5-fold/4-fold Ratios vs Pressure",800,600);
    g1PE->SetTitle("5-fold / 4-fold Ratios vs N2 Pressure;Pressure (atm);5-fold / 4-fold Ratio");
    g1PE->GetXaxis()->SetLimits(0, 17);
    g1PE->GetYaxis()->SetRangeUser(0,0.7);
    g1PE->Draw("APL");
    g3PE->Draw("PL SAME");
    g4PE->Draw("PL SAME");
    g5PE->Draw("PL SAME");

    // Legend
    auto leg = new TLegend(0.65,0.15,0.88,0.35);
    leg->AddEntry(g1PE,"1 PE","lp");
    leg->AddEntry(g3PE,"3 PE","lp");
    leg->AddEntry(g4PE,"4 PE","lp");
    leg->AddEntry(g5PE,"5 PE","lp");
    leg->Draw();

    c1->SaveAs("FoldRatio_vs_Pressure.png");
}

