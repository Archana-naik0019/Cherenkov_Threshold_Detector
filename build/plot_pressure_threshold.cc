#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLegend.h>
#include <TAxis.h>

void plot_pressure_threshold() {
    // Data from your table
    const int N = 15;
    double pressure[N]  = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double threshold[N] = {3.115,2.536,2.176,1.935,1.786,1.625,1.515,1.425,
                           1.336,1.285,1.215,1.167,1.126,1.075,1.045};

    // Errors (dummy example: 0.05 GeV on threshold, no error in pressure)
    double ex[N] = {0};  
    double ey[N];
    for(int i=0;i<N;i++) ey[i] = 0.05;

    // Graph with errors
    TGraphErrors *gr = new TGraphErrors(N, pressure, threshold, ex, ey);
    gr->SetTitle("Pressure vs Cherenkov Threshold;Pressure (atm);Threshold Energy (GeV)");
    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(kBlue+2);
    gr->SetLineColor(kBlue);

    // Fit function: E = A / sqrt(P)
    TF1 *fitFunc = new TF1("fitFunc","[0]/sqrt(x)",2,16);
    fitFunc->SetParameter(0,5.0); // initial guess

    gr->Fit(fitFunc,"R");

    // Canvas
    TCanvas *c1 = new TCanvas("c1","Pressure vs Threshold",800,600);
    c1->SetGrid(); // add grid

    gr->Draw("AP");

    // Draw fit
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineStyle(2);
    fitFunc->Draw("same");

    // Legend
    TLegend *leg = new TLegend(0.55,0.70,0.88,0.88);
    leg->AddEntry(gr,"Data points","lep");
    TString fitEq = Form("Fit: E = %.3f / #sqrt{P}", fitFunc->GetParameter(0));
    leg->AddEntry(fitFunc, fitEq, "l");
    leg->Draw();

    c1->SaveAs("pressure_vs_threshold.png");
}

