#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>

void Overlay_3p5PE_Averaged() {

    const int Nslide = 14;
    double pressure_slide[Nslide] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double slide[Nslide] = {
        9.70,11.27,15.08,23.04,24.79,28.00,28.43,
        32.58,35.11,41.68,40.76,46.05,44.48,45.95
    };

    const int Nexcel = 13;
    double pressure_excel[Nexcel] = {2,3,4,5,6,7,8,9,10,11,13,14,15};

    double datasetA[Nexcel] = {
        11.95,11.19,13.07,15.84,18.45,16.65,
        20.50,22.65,27.14,28.27,30.73,28.18,30.20
    };

    double datasetB[Nexcel] = {
        16.52,17.16,20.72,24.40,27.62,27.41,
        31.34,33.24,40.78,39.24,43.53,42.48,44.19
    };

    auto *c = new TCanvas("c","3.5PE (%) Comparison - Averaged Data",900,600);

    TGraph *g_slide = new TGraph(Nslide, pressure_slide, slide);
    g_slide->SetLineWidth(3);
    g_slide->SetMarkerStyle(20);

    TGraph *g_A = new TGraph(Nexcel, pressure_excel, datasetA);
    g_A->SetLineColor(kRed);
    g_A->SetMarkerColor(kRed);
    g_A->SetLineWidth(3);
    g_A->SetMarkerStyle(21);

    TGraph *g_B = new TGraph(Nexcel, pressure_excel, datasetB);
    g_B->SetLineColor(kBlue);
    g_B->SetMarkerColor(kBlue);
    g_B->SetLineWidth(3);
    g_B->SetMarkerStyle(22);

    g_slide->SetTitle("3.5PE (%) vs Pressure");
    g_slide->GetXaxis()->SetTitle("Pressure (atm)");
    g_slide->GetYaxis()->SetTitle("3.5PE (%)");
    g_slide->GetYaxis()->SetRangeUser(0, 60);

    g_slide->Draw("ALP");
    g_A->Draw("LP SAME");
    g_B->Draw("LP SAME");

    auto *leg = new TLegend(0.12,0.7,0.45,0.88);
    leg->AddEntry(g_slide, "Shubhi_data", "lp");
    leg->AddEntry(g_A, "Dataset A (at_1050V)", "lp");
    leg->AddEntry(g_B, "Dataset B (at_1000V)", "lp");
    leg->Draw();

    c->SetGrid();
}

