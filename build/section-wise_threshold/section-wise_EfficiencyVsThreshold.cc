#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TStyle.h>
#include <cmath>

void plotFoldRatios_allPE_binomial() {
    // Number of data points (after removing 2 atm)
    const int N = 14;

    // Pressure (atm)
    double pressure[N] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    // N total for each pressure (given)
    double Ntotal[N] = {331, 292, 337, 319, 379, 344, 334, 346, 349, 403, 404, 399, 306, 372};

    // Ratios from Excel
    double ratio1PE[N] = {
        0.105740181, 0.136986301, 0.106824926, 0.10031348,
        0.092342885, 0.084302326, 0.098802395, 0.072254335,
        0.048710602, 0.066997519, 0.047029703, 0.053097345,
        0.049016808, 0.048387097
    };

    double ratio3PE[N] = {
        0.045317221, 0.078767123, 0.050445104, 0.056426332,
        0.055408971, 0.043604651, 0.053892216, 0.046242775,
        0.031518625, 0.037220844, 0.032178218, 0.037593985,
        0.026143791, 0.040322581
    };

    double ratio4PE[N] = {
        0.036253776, 0.054794521, 0.041543027, 0.05015674,
        0.044858481, 0.026162791, 0.035928144, 0.037572254,
        0.022926236, 0.02776675, 0.027227723, 0.044247788,
        0.016339869, 0.032258065
    };

    double ratio5PE[N] = {
        0.018126888, 0.047945205, 0.029673591, 0.037617555,
        0.031662269, 0.020344887, 0.026946108, 0.034682081,
        0.020037307, 0.022335026, 0.024752475, 0.037593985,
        0.010139869, 0.029569892
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

    // Style setup
    gStyle->SetOptStat(0);

    g1->SetLineColor(kBlue);
    g1->SetLineWidth(1);
    g1->SetMarkerStyle(20);
    g1->SetMarkerColor(kBlue);

    g3->SetLineColor(kGreen + 2);
    g3->SetLineWidth(1);
    g3->SetMarkerStyle(21);
    g3->SetMarkerColor(kGreen + 2);

    g4->SetLineColor(kRed);
    g4->SetLineWidth(1);
    g4->SetMarkerStyle(22);
    g4->SetMarkerColor(kRed);

    g5->SetLineColor(kMagenta);
    g5->SetLineWidth(1);
    g5->SetMarkerStyle(23);
    g5->SetMarkerColor(kMagenta);

    // Canvas
    TCanvas *c1 = new TCanvas("c1", "5-fold / 4-fold Ratios vs Pressure", 900, 700);
    g1->SetTitle("5-fold / 4-fold Ratios vs Pressure;Pressure (atm);5-fold/4-fold Ratio");

    // Draw
    g1->Draw("APL");
    g1->GetXaxis()->SetLimits(0, 17);
    g1->GetYaxis()->SetRangeUser(0, 0.18);

    g3->Draw("PL SAME");
    g4->Draw("PL SAME");
    g5->Draw("PL SAME");

    // Legend
    TLegend *leg = new TLegend(0.15, 0.70, 0.45, 0.88);
    leg->AddEntry(g1, "1PE", "lp");
    leg->AddEntry(g3, "3PE", "lp");
    leg->AddEntry(g4, "4PE", "lp");
    leg->AddEntry(g5, "5PE", "lp");
    leg->Draw();
    
    c1->SetGridx();  // vertical grid
    c1->SetGridy();  // horizontal grid


    c1->Update();
}

