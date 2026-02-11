#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TStyle.h>
#include <cmath>

void plot_photon_loss()
{
    gStyle->SetOptStat(0);

    // -----------------------------
    // Data
    // -----------------------------
    const int n = 15;

    double atm[n] = {
        2, 3, 4, 5, 6,
        7, 8, 9, 10, 11,
        12, 13, 14, 15, 16
    };

    double loss[n] = {
        0.946499187,
        0.932601849,
        0.926891809,
        0.909478007,
        0.897518748,
        0.879992926,
        0.889353368,
        0.848905407,
        0.8508045,
        0.846365468,
        0.824456238,
        0.801212155,
        0.788003607,
        0.810149077,
        0.776052397
    };

    double N[n] = {
        199268, 195584, 219565, 217704, 216020,
        218263, 235389, 220630, 231448, 248629,
        247397, 249638, 251759, 311181, 287344
    };

    // -----------------------------
    // Errors
    // -----------------------------
    double xerr[n] = {0};
    double yerr[n];

    for (int i = 0; i < n; ++i) {
        yerr[i] = std::sqrt(loss[i] * (1.0 - loss[i]) / N[i]);
    }

    // -----------------------------
    // Graph
    // -----------------------------
    TGraphErrors* gr = new TGraphErrors(n, atm, loss, xerr, yerr);

    gr->SetTitle("");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.2);
    gr->SetLineWidth(2);

    gr->GetXaxis()->SetTitle("Pressure (atm)");
    gr->GetYaxis()->SetTitle("Photon loss fraction");

    gr->GetXaxis()->SetTitleSize(0.045);
    gr->GetYaxis()->SetTitleSize(0.045);
    gr->GetXaxis()->SetLabelSize(0.04);
    gr->GetYaxis()->SetLabelSize(0.04);

    gr->GetYaxis()->SetRangeUser(0.75, 1.0);

    // -----------------------------
    // Canvas
    // -----------------------------
    TCanvas* c = new TCanvas("c", "Photon loss vs pressure", 800, 600);
    c->SetGrid();

    gr->Draw("ALP");
    
    // -----------------------------
// Legend
// -----------------------------
TLegend* legend = new TLegend(0.6, 0.2, 0.88, 0.35);  // position
//legend->SetBorderSize(0);
legend->SetBorderSize(1);
legend->SetFillColor(0);
legend->SetTextSize(0.04);

// "lep" = line + error bars + points
legend->AddEntry(gr, "Measured photon loss", "lep");
legend->Draw();


    c->SaveAs("photon_loss_vs_pressure.pdf");
}
