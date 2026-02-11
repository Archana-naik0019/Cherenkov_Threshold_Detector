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

// =======================================================
// Main Analysis Macro
// =======================================================
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

    // 3. Simulation (4 PE)
    const int Nsim=15;
    double Psim[Nsim]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double Ysim[Nsim]={5.137,9.3093,11.5942,14.4366,20.5479,26.8707,27.5261,31.5789,36.4621,33.9223,41.0909,40.5594,41.0169,37.50,46.595};
    double NsimEv[Nsim]={292,333,276,284,292,294,287,285,277,283,275,281,295,312,279};
    double Esim[Nsim];
    for(int i=0;i<Nsim;i++) Esim[i]=100.*sqrt((Ysim[i]/100.)*(1-Ysim[i]/100.)/NsimEv[i]);
    auto gsim = new TGraphErrors(Nsim,Psim,Ysim,nullptr,Esim);

    // 4. Mylar 0.95
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

    // 5. Mylar 0.97
    double Ymy97raw[Nmy]={0.062295,0.111111,0.219178,0.24555,0.33550,0.36092,0.42857,0.43537};
    double Nmy97[Nmy]={305,288,292,281,307,302,280,294};
    double Ymy97[Nmy],Emy97[Nmy];
    for(int i=0;i<Nmy;i++) {
        Ymy97[i]=100.*Ymy97raw[i];
        Emy97[i]=100.*sqrt(Ymy97raw[i]*(1-Ymy97raw[i])/Nmy97[i]);
    }
    auto gmy97 = new TGraphErrors(Nmy,Pmy,Ymy97,nullptr,Emy97);

    // 6. Mylar 0.95 and Quartz Transmittance from jgs2
    const int Nnew = 8;
    double Pnew[Nnew] = {2, 4, 6, 8, 10, 12, 14, 16};
    double YnewRaw[Nnew] = {0.10616, 0.16608, 0.25, 0.34426, 0.35505, 0.41772, 0.42159, 0.46130};
    double NnewEv[Nnew] = {292, 283, 320, 305, 307, 316, 306, 323};
    double Ynew[Nnew], Enew[Nnew];
    for(int i=0; i<Nnew; i++) {
        Ynew[i] = 100. * YnewRaw[i];
        Enew[i] = 100. * std::sqrt(YnewRaw[i] * (1.0 - YnewRaw[i]) / NnewEv[i]);
    }
    auto gNew = new TGraphErrors(Nnew, Pnew, Ynew, nullptr, Enew);

    // Styling
    g1050->SetMarkerStyle(21); g1050->SetMarkerColor(kRed+1);      g1050->SetLineColor(kRed+1);
    gdec ->SetMarkerStyle(23); gdec ->SetMarkerColor(kOrange+7);   gdec ->SetLineColor(kOrange+7);
    gsim ->SetMarkerStyle(25); gsim ->SetMarkerColor(kBlue+1);     gsim ->SetLineColor(kBlue+1);
    gmy95->SetMarkerStyle(26); gmy95->SetMarkerColor(kGreen+2);    gmy95->SetLineColor(kGreen+2);
    gmy97->SetMarkerStyle(32); gmy97->SetMarkerColor(kMagenta+1);  gmy97->SetLineColor(kMagenta+1);
    gNew ->SetMarkerStyle(29); gNew ->SetMarkerColor(kCyan+2);     gNew ->SetLineColor(kCyan+2); gNew->SetMarkerSize(1.4);

    // ===================== OVERLAY PLOT =====================
    TCanvas *cOverlay = new TCanvas("cOverlay", "Overlay All Datasets", 1000, 700);
    cOverlay->SetGrid();
    g1050->GetXaxis()->SetLimits(1, 17);
    g1050->GetYaxis()->SetRangeUser(0, 60);
    g1050->GetXaxis()->SetTitle("Pressure (atm)");
    g1050->GetYaxis()->SetTitle("Efficiency (%)");
    g1050->Draw("ALP");
    gdec->Draw("LP SAME");
    gsim->Draw("LP SAME");
    gmy95->Draw("LP SAME");
    gmy97->Draw("LP SAME");
    gNew->Draw("LP SAME");

    auto leg = new TLegend(0.12, 0.65, 0.45, 0.88);
    leg->AddEntry(g1050, "Old data (3.5 PE)", "lp");
    leg->AddEntry(gdec,  "New data (3.5 PE)", "lp");
    leg->AddEntry(gsim,  "Simulation (4 PE)", "lp");
    leg->AddEntry(gmy95, "Mylar R=0.95", "lp");
    leg->AddEntry(gmy97, "Mylar R=0.97", "lp");
    leg->AddEntry(gNew,  "MylarR=0.95,Quartz JGS2", "lp");
    leg->Draw();

    // ===================== FINAL RATIO PLOT =====================
    TCanvas *cRatio = new TCanvas("cRatio", "Ratio Old/Spreadsheet", 800, 600);
    cRatio->SetGrid();
    TGraphErrors* ratio = MakeRatio_MatchingPoints(g1050, gNew);
    ratio->SetTitle("Ratio: Old Data / Spreadsheet Data;Pressure (atm);Ratio");
    ratio->SetMarkerStyle(20);
    ratio->SetMarkerColor(kBlack);
    ratio->GetXaxis()->SetLimits(1, 17);
    ratio->GetYaxis()->SetRangeUser(0.4, 1.4); // Adjust based on your data spread
    ratio->Draw("ALP");

    TLine *line = new TLine(1, 1, 17, 1);
    line->SetLineStyle(2);
    line->SetLineColor(kGray+2);
    line->Draw();

    cOverlay->SaveAs("Efficiency_Overlay.png");
    cRatio->SaveAs("Ratio_Old_vs_Spreadsheet.png");
}
