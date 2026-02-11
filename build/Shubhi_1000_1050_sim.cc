#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>

void Overlay_3p5PE_Avg() {

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

    // ✅ Simulation (4PE)
    const int Nsim = 15;
    double pressure_sim[Nsim] = {
        2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
    };
    double sim4PE[Nsim] = {
        5.137,9.3093,11.5942,14.4366,20.5479,
        26.8707,27.5261,31.5789,36.4621,33.9223,
        41.0909,40.5594,41.0169,37.50,46.595
    };

    auto *c = new TCanvas("c","Efficiency (%) Comparison",900,600);

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

    // ✅ new simulation graph
    TGraph *g_sim = new TGraph(Nsim, pressure_sim, sim4PE);
    g_sim->SetLineColor(kGreen+2);
    g_sim->SetMarkerColor(kGreen+2);
    g_sim->SetLineWidth(3);
    g_sim->SetMarkerStyle(33);

    g_slide->SetTitle("Efficiency vs Pressure");
    g_slide->GetXaxis()->SetTitle("Pressure (atm)");
    g_slide->GetYaxis()->SetTitle("Efficiency (%)");
    g_slide->GetYaxis()->SetRangeUser(0, 60);

    g_slide->Draw("ALP");
    g_A->Draw("LP SAME");
    g_B->Draw("LP SAME");
    g_sim->Draw("LP SAME");

    auto *leg = new TLegend(0.12,0.68,0.45,0.90);
    leg->AddEntry(g_slide, "Shubhi_data (3.5PE)", "lp");
    leg->AddEntry(g_A, "at_1050V (3.5PE)", "lp");
    leg->AddEntry(g_B, "at_1000V (3.5PE)", "lp");
    leg->AddEntry(g_sim, "Simulation (4PE)", "lp");
    leg->Draw();

    c->SetGrid();
}

