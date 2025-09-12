#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TStyle.h"

void plot_curves_with_fit() {
    // Improve visual style
    gStyle->SetOptFit(1);

    // Example data points for two curves
    const int n = 15;
    double x[n] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,13 ,14, 15, 16};
    double y1[n] = {0.0101327, 0.0128349, 0.0356102, 0.0366671, 0.0332725, 0.0294975, 0.026026, 0.0230223, 0.0206773, 0.0206831, 0.0171228, 0.0158301, 0.0147183, 0.0138276, 0.01311};
    double y2[n] = {0.00534988,0.00913221,0.0334054,0.035173,0.0318128,0.0278152,0.0243309,0.0217034,0.0195587,0.0178487,0.0164492,0.0152412,0.0141598,0.0132212,0.0124291};

    // Create graphs
    TGraph *g1 = new TGraph(n, x, y1);
    g1->SetLineColor(kRed);
    g1->SetMarkerColor(kRed);
    g1->SetMarkerStyle(20);
    g1->SetTitle("Curves with Approx Fit;X-axis;Y-axis");

    TGraph *g2 = new TGraph(n, x, y2);
    g2->SetLineColor(kBlue);
    g2->SetMarkerColor(kBlue);
    g2->SetMarkerStyle(21);

    // Create canvas
    TCanvas *c1 = new TCanvas("c1", "Curves with Fit", 800, 600);
    c1->cd();

    // Draw the first graph
    g1->Draw("APL"); // Axis, Points, Lines
    g2->Draw("PL SAME"); // Draw second graph on same canvas

    // Fit first graph with a linear function
    TF1 *fit1 = new TF1("fit1", "pol1", 0, 5); // pol1 = linear
    g1->Fit(fit1, "R");

    // Fit second graph with a linear function
    TF1 *fit2 = new TF1("fit2", "pol1", 0, 5);
    g2->Fit(fit2, "R");

    // Add legend
    TLegend *leg = new TLegend(0.6,0.7,0.88,0.88);
    leg->AddEntry(g1, "Curve 1", "lp");
    leg->AddEntry(g2, "Curve 2", "lp");
    leg->Draw();

    c1->Update();
}

