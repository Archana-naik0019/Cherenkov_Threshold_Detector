#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLine.h>
#include <TAxis.h>
#include <TStyle.h>
#include <vector>
#include <cmath>
#include <algorithm>

// =======================================================
// Ratio using ONLY exactly matching pressure points
// =======================================================
TGraphErrors* MakeRatio_MatchingPoints(TGraphErrors* gNum, TGraphErrors* gDen) {
    std::vector<double> x, y, ex, ey;

    for (int i = 0; i < gNum->GetN(); ++i) {
        double xi, yi;
        gNum->GetPoint(i, xi, yi);
        double ei = gNum->GetErrorY(i);

        for (int j = 0; j < gDen->GetN(); ++j) {
            double xj, yj;
            gDen->GetPoint(j, xj, yj);

            if (std::abs(xi - xj) < 1e-6 && yi > 0 && yj > 0) {
                double ej = gDen->GetErrorY(j);
                double r  = yi / yj;
                double er = r * std::sqrt(std::pow(ei/yi, 2) + std::pow(ej/yj, 2));

                x.push_back(xi);
                y.push_back(r);
                ex.push_back(0.0);
                ey.push_back(er);
                break;
            }
        }
    }
    return new TGraphErrors(x.size(), x.data(), y.data(), ex.data(), ey.data());
}

void EfficiencyAnalysis() {
    gStyle->SetEndErrorSize(4);
    gStyle->SetOptTitle(0);

    // 1. Old data (3.5 PE)
    const int N1050 = 13;
    double P1050[N1050]={3,4,5,6,7,8,9,10,11,12,14,15,16};
    double Y1050[N1050]={11.95,11.19,13.07,15.84,18.45,16.65,20.50,22.65,27.14,28.27,30.73,28.18,30.20};
    double N1050ev[N1050]={1355,1224,2035,1863,840,799,1056,746,748,711,781,685,1129};
    double E1050[N1050];
    for(int i=0;i<N1050;i++) E1050[i]=100.*sqrt((Y1050[i]/100.)*(1-Y1050[i]/100.)/N1050ev[i]);
    auto g1050 = new TGraphErrors(N1050,P1050,Y1050,nullptr,E1050);

    // 2. New data (3.5 PE)
    const int Ndec=9;
    double Pdec[Ndec]={4.25,6,7.25,8,9,10,11,12,13};
    double YdecRaw[Ndec]={0.135,0.159,0.178,0.163,0.198,0.21,0.234,0.246,0.25};
    double NdecEv[Ndec]={467,2082,1971,2795,2750,2078,2061,2912,4419};
    double Ydec[Ndec],Edec[Ndec];
    for(int i=0;i<Ndec;i++) {
        Ydec[i]=100.*YdecRaw[i];
        Edec[i]=100.*sqrt(YdecRaw[i]*(1-YdecRaw[i])/NdecEv[i]);
    }
    auto gdec = new TGraphErrors(Ndec,Pdec,Ydec,nullptr,Edec);

    // 3. Simulation (4 PE), MR99, QT73
    const int Nsim=15;
    double Psim[Nsim]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double Ysim[Nsim]={5.137,9.3093,11.5942,14.4366,20.5479,26.8707,27.5261,31.5789,36.4621,33.9223,41.0909,40.5594,41.0169,37.50,46.595};
    double NsimEv[Nsim]={292,333,276,284,292,294,287,285,277,283,275,281,295,312,279};
    double Esim[Nsim];
    for(int i=0;i<Nsim;i++) Esim[i]=100.*sqrt((Ysim[i]/100.)*(1-Ysim[i]/100.)/NsimEv[i]);
    auto gsim = new TGraphErrors(Nsim,Psim,Ysim,nullptr,Esim);

    // 4. Mylar 0.95, QT73
    const int Nmy=8;
    double Pmy[Nmy]={2,4,6,8,10,12,14,16};
    double Ymy95raw[Nmy]={0.023178,0.123239,0.184713,0.25641,0.30976,0.37666,0.39932,0.46071};
    double Nmy95[Nmy]={302,284,314,312,297,300,298,280};
    double Ymy95[Nmy],Emy95[Nmy];
    for(int i=0;i<Nmy;i++) {
        Ymy95[i]=100.*Ymy95raw[i];
        Emy95[i]=100.*sqrt(Ymy95raw[i]*(1-Ymy95raw[i])/Nmy95[i]);
    }
    auto gmy95 = new TGraphErrors(Nmy,Pmy,Ymy95,nullptr,Emy95);

    // 5. Mylar 0.95, QT80
    const int NT=15;
    double PmT[NT]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double Ymy97raw[NT]={0.1084337349,0.1666666667,0.2079207921,0.2218309859,0.295532646,0.3247588424,0.3231292517,0.3647798742,0.3706070288,0.3973509934,0.4208754209,0.4383116883,0.4782608696,0.5384615385,0.5544217687};
    double Nmy97[NT]={332,318,303,284,291,311,294,318,313,302,297,308,299,299,294};
    double Ymy97[NT],Emy97[NT];
    for(int i=0;i<NT;i++) {
        Ymy97[i]=100.*Ymy97raw[i];
        Emy97[i]=100.*sqrt(Ymy97raw[i]*(1-Ymy97raw[i])/Nmy97[i]);
    }
    auto gmy97 = new TGraphErrors(NT,PmT,Ymy97,nullptr,Emy97);

    // 6. Mylar 0.95 and Quartz Transmittance from jgs2
    const int Nnew = 15;
    double Pnew[Nnew] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double YnewRaw[Nnew] = {0.1061643836,0.15,0.1660777385,0.2251655629,0.25,0.2974683544,0.3442622951,0.3375394322,0.3550488599,0.4033333333,0.417721519,0.4229508197,0.4215686275,0.4899328859,0.4613003096};
    double NnewEv[Nnew] = {292,300,283,302,320,316,305,317,307,300,316,305,306,298,323};
    double Ynew[Nnew], Enew[Nnew];
    for(int i=0; i<Nnew; i++) {
        Ynew[i] = 100. * YnewRaw[i];
        Enew[i] = 100. * std::sqrt(YnewRaw[i] * (1.0 - YnewRaw[i]) / NnewEv[i]);
    }
    auto gNew = new TGraphErrors(Nnew, Pnew, Ynew, nullptr, Enew);

    // --- NEW: Shubhi's Analysis (3.5 PE) ---
    const int Nslide = 14;
    double pressure_slide[Nslide] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double slide[Nslide] = {9.70,11.27,15.08,23.04,24.79,28.00,28.43,32.58,35.11,41.68,40.76,46.05,44.48,45.95};
    double slide_err[Nslide] = {0.42,0.86,1.02,1.63,1.62,1.55,1.59,1.44,1.74,1.80,1.84,1.78,1.89,1.75};
    auto gShubhi35 = new TGraphErrors(Nslide, pressure_slide, slide, nullptr, slide_err);

    // --- NEW: Shubhi's Analysis (4 PE) ---
    const int Nshubhi4 = 14;
    double pressure_shubhi4[Nshubhi4] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double shubhi4[Nshubhi4] = {8.29,9.94,12.63,18.37,19.33,22.89,21.45,26.06,28.17,34.75,34.03,38.27,35.32,37.59};
    double shubhi4_err[Nshubhi4] = {0.39,0.81,0.95,1.50,1.48,1.45,1.45,1.35,1.64,1.74,1.77,1.74,1.82,1.70};
    auto gShubhi40 = new TGraphErrors(Nshubhi4, pressure_shubhi4, shubhi4, nullptr, shubhi4_err);

    // Styling
    g1050->SetMarkerStyle(21); g1050->SetMarkerColor(kRed+1);      g1050->SetLineColor(kRed+1);
    gdec ->SetMarkerStyle(23); gdec ->SetMarkerColor(kOrange+7);   gdec ->SetLineColor(kOrange+7);
    gsim ->SetMarkerStyle(25); gsim ->SetMarkerColor(kBlue+1);     gsim ->SetLineColor(kBlue+1);
    gmy95->SetMarkerStyle(26); gmy95->SetMarkerColor(kGreen+2);    gmy95->SetLineColor(kGreen+2);
    gmy97->SetMarkerStyle(32); gmy97->SetMarkerColor(kMagenta+1);  gmy97->SetLineColor(kMagenta+1);
    gNew ->SetMarkerStyle(29); gNew ->SetMarkerColor(kCyan+2);     gNew ->SetLineColor(kCyan+2); gNew->SetMarkerSize(1.4);
    
    // Styling for new Shubhi datasets
    gShubhi35->SetMarkerStyle(20); gShubhi35->SetMarkerColor(kBlack);   gShubhi35->SetLineColor(kBlack);
    gShubhi40->SetMarkerStyle(24); gShubhi40->SetMarkerColor(kGray+2);  gShubhi40->SetLineColor(kGray+2);

    // ===================== OVERLAY PLOT =====================
    TCanvas *cOverlay = new TCanvas("cOverlay", "Overlay All Datasets", 1000, 700);
    cOverlay->SetGrid();
    
    // Draw the first graph to set the axes
    g1050->GetXaxis()->SetLimits(1, 17);
    g1050->GetYaxis()->SetRangeUser(0, 60);
    g1050->GetXaxis()->SetTitle("Pressure (atm)");
    g1050->GetYaxis()->SetTitle("Efficiency (%)");
    g1050->Draw("ALP");

    // Draw existing
    gdec->Draw("LP SAME");
    gsim->Draw("LP SAME");
    gmy95->Draw("LP SAME");
    gmy97->Draw("LP SAME");
    gNew->Draw("LP SAME");

    // Draw NEW Shubhi datasets
    gShubhi35->Draw("LP SAME");
    gShubhi40->Draw("LP SAME");

    // Update Legend (extended size to fit new entries)
    auto leg = new TLegend(0.12, 0.55, 0.48, 0.88); 
    leg->AddEntry(g1050, "Old data (3.5 PE)", "lp");
    leg->AddEntry(gdec,  "New data (3.5 PE)", "lp");
    leg->AddEntry(gsim,  "Mylar R=0.99,Quartz T=0.73", "lp");
    leg->AddEntry(gmy95, "Mylar R=0.95,Quartz T=0.73", "lp");
    leg->AddEntry(gmy97, "Mylar R=0.95,Quartz T=0.8", "lp");
    leg->AddEntry(gNew,  "Mylar R=0.95,Quartz JGS2", "lp");
    leg->AddEntry(gShubhi35, "Shubhi Analysis (3.5 PE)", "lp");
    leg->AddEntry(gShubhi40, "Shubhi Analysis (4 PE)", "lp");
    leg->Draw();

    // ===================== FINAL RATIO PLOT =====================
    // Keeping your original ratio as is (Old vs Spreadsheet)
    TCanvas *cRatio = new TCanvas("cRatio", "Ratio Old/Spreadsheet", 800, 600);
    cRatio->SetGrid();
    TGraphErrors* ratio = MakeRatio_MatchingPoints(g1050, gNew);
    ratio->SetTitle("Ratio: Old Data / Spreadsheet Data;Pressure (atm);Ratio");
    ratio->SetMarkerStyle(20);
    ratio->SetMarkerColor(kBlack);
    ratio->GetXaxis()->SetLimits(1, 17);
    ratio->GetYaxis()->SetRangeUser(0.4, 1.4);
    ratio->Draw("ALP");

    TLine *line = new TLine(1, 1, 17, 1);
    line->SetLineStyle(2);
    line->SetLineColor(kGray+2);
    line->Draw();

    cOverlay->SaveAs("Efficiency_Overlay.png");
    cRatio->SaveAs("Ratio_Old_vs_Spreadsheet.png");
}
