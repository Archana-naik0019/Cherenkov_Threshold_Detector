#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <cmath>

void Overlaid_3p5PE_Avg() {

    // Slide dataset
    const int Nslide = 14;
    double pressure_slide[Nslide] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double slide[Nslide] = {
        9.70,11.27,15.08,23.04,24.79,28.00,28.43,
        32.58,35.11,41.68,40.76,46.05,44.48,45.95
    };
    double slide_err[Nslide] = {
        0.42,0.86,1.02,1.63,1.62,1.55,1.59,
        1.44,1.74,1.80,1.84,1.78,1.89,1.75
    };

    // Dataset A
    const int Nexcel = 13;
    double pressure_excel[Nexcel] = {2,3,4,5,6,7,8,9,10,11,13,14,15};
    double datasetA[Nexcel] = {
        11.95,11.19,13.07,15.84,18.45,16.65,
        20.50,22.65,27.14,28.27,30.73,28.18,30.20
    };
    double NA[Nexcel] = {
        1355,1224,2035,1863,840,799,1056,
        746,748,711,781,685,1129
    };
    double A_err[Nexcel];
    for(int i=0;i<Nexcel;i++){
        double p = datasetA[i]/100.0;
        A_err[i] = 100.0 * sqrt(p*(1-p)/NA[i]);
    }

    // Dataset B (same total N as dataset A)
    double datasetB[Nexcel] = {
        16.52,17.16,20.72,24.40,27.62,27.41,
        31.34,33.24,40.78,39.24,43.53,42.48,44.19
    };
    double B_err[Nexcel];
    for(int i=0;i<Nexcel;i++){
        double p = datasetB[i]/100.0;
        B_err[i] = 100.0 * sqrt(p*(1-p)/NA[i]);
    }

    // Simulation dataset
    const int Nsim = 15;
    double pressure_sim[Nsim] = {
        2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
    };
    double sim4PE[Nsim] = {
        5.137,9.3093,11.5942,14.4366,20.5479,
        26.8707,27.5261,31.5789,36.4621,33.9223,
        41.0909,40.5594,41.0169,37.50,46.595
    };

    // Simulation Nsuccess values
    double sim_total[Nsim] = {
        292,333,276,284,292,294,287,285,277,
        283,275,281,295,312,279
    };

    double sim_err[Nsim];
    for(int i=0;i<Nsim;i++){
        double p = sim4PE[i] / 100;
        sim_err[i] = 100.0 * sqrt(p*(1-p)/sim_total[i]);
    }
    
    // Shubhi_4PE
    
    const int Nshubhi4 = 14;
    double pressure_shubhi4[Nshubhi4] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double shubhi4[Nshubhi4] = {
        8.29,9.94,12.63,18.37,19.33,22.89,21.45,
        26.06,28.17,34.75,34.03,38.27,35.32,37.59
    };
    double shubhi4_err[Nshubhi4] = {
        0.39,0.81,0.95,1.5,1.48,1.45,1.45,
        1.35,1.64,1.74,1.77,1.74,1.82,1.7
    };

    auto *c = new TCanvas("c","Efficiency (%) Comparison",1000,650);

    auto *g_slide = new TGraphErrors(Nslide, pressure_slide, slide, nullptr, slide_err);
    g_slide->SetMarkerStyle(20);
    g_slide->SetLineWidth(3);

    auto *g_A = new TGraphErrors(Nexcel, pressure_excel, datasetA, nullptr, A_err);
    g_A->SetLineColor(kRed);
    g_A->SetMarkerColor(kRed);
    g_A->SetMarkerStyle(21);
    g_A->SetLineWidth(3);

    auto *g_B = new TGraphErrors(Nexcel, pressure_excel, datasetB, nullptr, B_err);
    g_B->SetLineColor(kBlue);
    g_B->SetMarkerColor(kBlue);
    g_B->SetMarkerStyle(22);
    g_B->SetLineWidth(3);

    auto *g_sim = new TGraphErrors(Nsim, pressure_sim, sim4PE, nullptr, sim_err);
    g_sim->SetLineColor(kGreen+2);
    g_sim->SetMarkerColor(kGreen+2);
    g_sim->SetMarkerStyle(33);
    g_sim->SetLineWidth(3);
    
    auto *g_shubhi4 = new TGraphErrors(Nshubhi4, pressure_shubhi4, shubhi4, nullptr, shubhi4_err);
    g_shubhi4->SetLineColor(kViolet+1);
    g_shubhi4->SetMarkerColor(kViolet+1);
    g_shubhi4->SetMarkerStyle(29);
    g_shubhi4->SetLineWidth(3);

    g_slide->SetTitle("Efficiency vs Pressure;Pressure (atm);Efficiency (%)");
    g_slide->GetYaxis()->SetRangeUser(0, 60);

    g_slide->Draw("ALP");
    g_A->Draw("LP SAME");
    g_B->Draw("LP SAME");
    g_sim->Draw("LP SAME");
    g_shubhi4->Draw("LP SAME");

    auto *leg = new TLegend(0.12,0.68,0.45,0.90);
    leg->AddEntry(g_slide, "Shubhi_data (3.5PE)", "lp");
    leg->AddEntry(g_A, "at_1050V (3.5PE)", "lp");
    leg->AddEntry(g_B, "at_1000V (3.5PE)", "lp");
    leg->AddEntry(g_sim, "Simulation (4PE)", "lp");
    leg->AddEntry(g_shubhi4, "Shubhi_data (4PE)", "lp");
    leg->Draw();

    c->SetGrid();
}

