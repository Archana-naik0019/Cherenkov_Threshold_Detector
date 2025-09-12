#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLegend.h>
#include <TAxis.h>

void plot_two_datasets() {
    // --- Dataset 1 (first table you shared) ---
    const int N1 = 15;
    double pressure1[N1]  = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double threshold1[N1] = {3.115,2.536,2.176,1.935,1.786,1.625,1.515,1.425,
                             1.336,1.285,1.215,1.167,1.126,1.075,1.045};
    double ex1[N1] = {0};
    double ey1[N1]; for(int i=0;i<N1;i++) ey1[i]=0.05; // dummy errors

    // --- Dataset 2 (second table you shared) ---
    const int N2 = 15;
    double pressure2[N2]  = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double threshold2[N2] = {2.985,2.415,2.075,1.855,1.685,1.555,1.445,1.355,
                             1.285,1.225,1.165,1.115,1.075,1.035,0.995};
    double ex2[N2] = {0};
    double ey2[N2]; for(int i=0;i<N2;i++) ey2[i]=0.05; // dummy errors

    // Graphs
    TGraphErrors *gr1 = new TGraphErrors(N1, pressure1, threshold1, ex1, ey1);
    TGraphErrors *gr2 = new TGraphErrors(N2, pressure2, threshold2, ex2, ey2);

    gr1->SetMarkerStyle(20);
    gr1->SetMarkerColor(kBlue+2);
    gr1->SetLineColor(kBlue+2);

    gr2->SetMarkerStyle(21);
    gr2->SetMarkerColor(kRed+2);
    gr2->SetLineColor(kRed+2);

    gr1->SetTitle("Pressure vs Cherenkov Threshold;Pressure (atm);Threshold Energy (GeV)");

    // Fit function for both datasets: E = A / sqrt(P)
    TF1 *fit1 = new TF1("fit1","[0]/sqrt(x)",2,16);
    TF1 *fit2 = new TF1("fit2","[0]/sqrt(x)",2,16);

    fit1->SetParameter(0,5.0);
    fit2->SetParameter(0,5.0);

    gr1->Fit(fit1,"R");
    gr2->Fit(fit2,"R+");  // + keeps first fit

    // Canvas
    TCanvas *c1 = new TCanvas("c1","Pressure vs Threshold (Two Sets)",800,600);
    c1->SetGrid();

    gr1->Draw("AP");
    gr2->Draw("P SAME");

    fit1->SetLineColor(kBlue);
    fit1->SetLineStyle(2);
    fit2->SetLineColor(kRed);
    fit2->SetLineStyle(2);

    fit1->Draw("same");
    fit2->Draw("same");

    // Legend
    TLegend *leg = new TLegend(0.50,0.65,0.88,0.88);
    leg->AddEntry(gr1,"Corrected","lep");
    TString eq1 = Form("Fit1: E = %.3f / #sqrt{P}", fit1->GetParameter(0));
    leg->AddEntry(fit1, eq1, "l");

    leg->AddEntry(gr2,"From Ideal Gas Law","lep");
    TString eq2 = Form("Fit2: E = %.3f / #sqrt{P}", fit2->GetParameter(0));
    leg->AddEntry(fit2, eq2, "l");

    leg->Draw();

    c1->SaveAs("pressure_vs_threshold_two_datasets.png");
}

