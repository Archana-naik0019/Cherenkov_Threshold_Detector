#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TStyle.h>
#include <cmath>

void plotFoldRatios_allPE_binomial_v2() {
    // Number of data points
    const int N = 15;

    // Pressure (atm)
    double pressure[N] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    // N total for each pressure point
    double Ntotal[N] = {271, 272, 272, 272, 267, 301, 271, 270, 283, 281, 275, 297, 280, 280, 280};

    // Ratios from Excel (1PE, 3PE, 4PE, 5PE)
    double ratio1PE[N] = {
        0.236162362, 0.283088235, 0.386029412, 0.455882353, 0.483146067,
        0.511627907, 0.57195572, 0.592592593, 0.607773852, 0.583629893,
        0.629099099, 0.643097643, 0.632142857, 0.632142857, 0.685714286
    };

    double ratio3PE[N] = {
        0.136531365, 0.154411765, 0.246323529, 0.327205882, 0.36329588,
        0.431893688, 0.483394834, 0.488888889, 0.554770318, 0.512455516,
        0.563636364, 0.569023569, 0.578571429, 0.546428571, 0.628571429
    };

    double ratio4PE[N] = {
        0.114391144, 0.136029412, 0.224264706, 0.283088235, 0.337078652,
        0.395348837, 0.468364866, 0.462962963, 0.519434629, 0.498206411,
        0.527272727, 0.542087542, 0.564285714, 0.535714286, 0.603571429
    };

    double ratio5PE[N] = {
        0.092250923, 0.117647059, 0.202205882, 0.25, 0.31835056,
        0.375413282, 0.435424354, 0.425925926, 0.462897527, 0.466191721,
        0.50158182, 0.515151515, 0.535714286, 0.525, 0.582142857
    };

    // Error arrays
    double errX[N] = {0};
    double err1PE[N], err3PE[N], err4PE[N], err5PE[N];

    for (int i = 0; i < N; ++i) {
        err1PE[i] = sqrt(ratio1PE[i] * (1 - ratio1PE[i]) / Ntotal[i]);
        err3PE[i] = sqrt(ratio3PE[i] * (1 - ratio3PE[i]) / Ntotal[i]);
        err4PE[i] = sqrt(ratio4PE[i] * (1 - ratio4PE[i]) / Ntotal[i]);
        err5PE[i] = sqrt(ratio5PE[i] * (1 - ratio5PE[i]) / Ntotal[i]);
    }

    // Create graphs with errors
    TGraphErrors *g1 = new TGraphErrors(N, pressure, ratio1PE, errX, err1PE);
    TGraphErrors *g3 = new TGraphErrors(N, pressure, ratio3PE, errX, err3PE);
    TGraphErrors *g4 = new TGraphErrors(N, pressure, ratio4PE, errX, err4PE);
    TGraphErrors *g5 = new TGraphErrors(N, pressure, ratio5PE, errX, err5PE);

    // Style
    gStyle->SetOptStat(0);

    g1->SetLineColor(kBlue);
    g1->SetLineWidth(2);
    g1->SetMarkerStyle(20);
    g1->SetMarkerColor(kBlue);

    g3->SetLineColor(kGreen + 2);
    g3->SetLineWidth(2);
    g3->SetMarkerStyle(21);
    g3->SetMarkerColor(kGreen + 2);

    g4->SetLineColor(kRed);
    g4->SetLineWidth(2);
    g4->SetMarkerStyle(22);
    g4->SetMarkerColor(kRed);

    g5->SetLineColor(kMagenta);
    g5->SetLineWidth(2);
    g5->SetMarkerStyle(23);
    g5->SetMarkerColor(kMagenta);

    // Canvas
    TCanvas *c1 = new TCanvas("c1", "5-fold / 4-fold Ratios vs Pressure", 900, 700);
    g1->SetTitle("5-fold / 4-fold Ratios vs Pressure;Pressure (atm);Ratio");

    // Draw first
    g1->Draw("APL");
    g1->GetXaxis()->SetLimits(1.5, 16.5);
    g1->GetYaxis()->SetRangeUser(0, 0.75);

    // Overlay others
    g3->Draw("PL SAME");
    g4->Draw("PL SAME");
    g5->Draw("PL SAME");

    // Legend
    TLegend *leg = new TLegend(0.15, 0.68, 0.45, 0.88);
    leg->AddEntry(g1, "5-fold/4-fold (1PE)", "lp");
    leg->AddEntry(g3, "5-fold/4-fold (3PE)", "lp");
    leg->AddEntry(g4, "5-fold/4-fold (4PE)", "lp");
    leg->AddEntry(g5, "5-fold/4-fold (5PE)", "lp");
    leg->Draw();

    c1->Update();
}

