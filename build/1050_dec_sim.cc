#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLine.h>
#include <TAxis.h>
#include <TStyle.h>
#include <vector>
#include <cmath>

// =======================================================
// Ratio using ONLY exactly matching pressure points
// =======================================================
TGraphErrors* MakeRatio_MatchingPoints(TGraphErrors* gNum,
                                       TGraphErrors* gDen)
{
    std::vector<double> x, y, ex, ey;

    for (int i = 0; i < gNum->GetN(); ++i) {
        double xi, yi;
        gNum->GetPoint(i, xi, yi);
        double ei = gNum->GetErrorY(i);

        for (int j = 0; j < gDen->GetN(); ++j) {
            double xj, yj;
            gDen->GetPoint(j, xj, yj);

            if (fabs(xi - xj) < 1e-6 && yi > 0 && yj > 0) {
                double ej = gDen->GetErrorY(j);

                double r  = yi / yj;
                double er = r * std::sqrt(
                                std::pow(ei/yi, 2) +
                                std::pow(ej/yj, 2)
                            );

                x.push_back(xi);
                y.push_back(r);
                ex.push_back(0.0);
                ey.push_back(er);
                break;
            }
        }
    }

    return new TGraphErrors(x.size(),
                            x.data(), y.data(),
                            ex.data(), ey.data());
}

// =======================================================
// Auto Y-range for ratio plots (NO CLIPPING)
// =======================================================
void AutoRangeRatio(TGraphErrors* gr, double margin = 0.25)
{
    double ymin =  1e9;
    double ymax = -1e9;

    for (int i = 0; i < gr->GetN(); ++i) {
        double x, y;
        gr->GetPoint(i, x, y);
        double ey = gr->GetErrorY(i);

        ymin = std::min(ymin, y - ey);
        ymax = std::max(ymax, y + ey);
    }

    double span = ymax - ymin;
    gr->GetYaxis()->SetRangeUser(
        ymin - margin*span,
        ymax + margin*span
    );
}

// =======================================================
// Style ratio pads (slide-ready)
// =======================================================
void StyleRatio(TGraphErrors* gr, bool showX)
{
    gr->GetYaxis()->SetTitle("Ratio");
    gr->GetYaxis()->CenterTitle();
    gr->GetYaxis()->SetTitleSize(0.11);
    gr->GetYaxis()->SetLabelSize(0.09);
    gr->GetYaxis()->SetTitleOffset(0.55);

    gr->GetXaxis()->SetNdivisions(505);

    if (!showX) {
        gr->GetXaxis()->SetLabelSize(0);
        gr->GetXaxis()->SetTitleSize(0);
    } else {
        gr->GetXaxis()->SetTitle("Pressure (atm)");
        gr->GetXaxis()->SetLabelSize(0.11);
        gr->GetXaxis()->SetTitleSize(0.13);
        gr->GetXaxis()->SetTitleOffset(1.05);
    }

    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.75);
}

// =======================================================
// MAIN MACRO
// =======================================================
void ThreeCurves_WithAlignedRatios()
{
    gStyle->SetEndErrorSize(4);

    // ===================== DATA =====================
    const int N1050 = 13;
    double P1050[N1050]  = {2,3,4,5,6,7,8,9,10,11,13,14,15};
    double Y1050[N1050]  = {11.95,11.19,13.07,15.84,18.45,16.65,
                            20.50,22.65,27.14,28.27,30.73,28.18,30.20};
    double N1050ev[N1050]= {1355,1224,2035,1863,840,799,
                            1056,746,748,711,781,685,1129};

    double E1050[N1050];
    for(int i=0;i<N1050;i++){
        double p = Y1050[i]/100.;
        E1050[i] = 100.*std::sqrt(p*(1-p)/N1050ev[i]);
    }

    const int Ndec = 9;
    double Pdec[Ndec]   = {3.25,5,6.25,7,8,9,10,11,12};
    double YdecRaw[Ndec]= {0.135,0.159,0.178,0.163,0.198,
                           0.21,0.234,0.246,0.25};
    double NdecEv[Ndec] = {467,2082,1971,2795,2750,
                           2078,2061,2912,4419};

    double Ydec[Ndec], Edec[Ndec];
    for(int i=0;i<Ndec;i++){
        Ydec[i] = 100.*YdecRaw[i];
        Edec[i] = 100.*std::sqrt(YdecRaw[i]*(1-YdecRaw[i])/NdecEv[i]);
    }

    const int Nsim = 15;
    double Psim[Nsim]  = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double Ysim[Nsim]  = {5.137,9.3093,11.5942,14.4366,20.5479,
                          26.8707,27.5261,31.5789,36.4621,33.9223,
                          41.0909,40.5594,41.0169,37.50,46.595};
    double NsimEv[Nsim]= {292,333,276,284,292,294,287,285,
                          277,283,275,281,295,312,279};

    double Esim[Nsim];
    for(int i=0;i<Nsim;i++){
        double p = Ysim[i]/100.;
        Esim[i] = 100.*std::sqrt(p*(1-p)/NsimEv[i]);
    }

    auto g1050 = new TGraphErrors(N1050,P1050,Y1050,nullptr,E1050);
    auto gdec  = new TGraphErrors(Ndec ,Pdec ,Ydec ,nullptr,Edec );
    auto gsim  = new TGraphErrors(Nsim ,Psim ,Ysim ,nullptr,Esim );

// ===================== MAIN GRAPH STYLES =====================

// 1050V (old dataset)
    g1050->SetMarkerStyle(21);
    g1050->SetMarkerColor(kRed+1);
    g1050->SetLineColor(kRed+1);
    g1050->SetLineWidth(2);

// dec2025 data (new dataset)
    gdec->SetMarkerStyle(34);
    gdec->SetMarkerColor(kOrange+7);
    gdec->SetLineColor(kOrange+7);
    gdec->SetLineWidth(2);

// Simulation
    gsim->SetMarkerStyle(29);
    gsim->SetMarkerColor(kBlue+1);
    gsim->SetLineColor(kBlue+1);
    gsim->SetLineWidth(2);

// ===================== LEGEND =====================
    auto leg = new TLegend(0.12, 0.70, 0.42, 0.88);

    leg->SetBorderSize(0);     // no box
    leg->SetFillStyle(0);      // transparent
    leg->SetTextSize(0.045);   // slide-readable
    leg->SetTextFont(42);

// Explicit entries to enforce color + style consistency
    leg->AddEntry(g1050, "1050V (3.5 PE)", "lp");
    leg->AddEntry(gdec , "dec2025 data",  "lp");
    leg->AddEntry(gsim , "Simulation (4 PE)", "lp");

    leg->Draw();


    // ===================== CANVAS =====================
    TCanvas *c = new TCanvas("c","Efficiency + ratios",1200,900);

    // Taller ratio pads (presentation-safe)
    TPad *pMain = new TPad("pMain","",0,0.48,1,1.0);
    TPad *pR1   = new TPad("pR1","",0,0.32,1,0.48);
    TPad *pR2   = new TPad("pR2","",0,0.16,1,0.32);
    TPad *pR3   = new TPad("pR3","",0,0.00,1,0.16);

    for (auto p : {pMain,pR1,pR2,pR3}) {
        p->SetLeftMargin(0.14);
        p->SetRightMargin(0.05);
    }

    pMain->SetBottomMargin(0.02);
    pR3->SetBottomMargin(0.35);

    pMain->Draw(); pR1->Draw(); pR2->Draw(); pR3->Draw();

    // ===================== MAIN =====================
    pMain->cd();
    g1050->SetTitle("Efficiency vs Pressure;Pressure (atm);Efficiency (%)");
    g1050->GetYaxis()->SetRangeUser(0,60);
    g1050->Draw("ALP");
    gdec->Draw("LP SAME");
    gsim->Draw("LP SAME");
    pMain->SetGrid();

    // ===================== RATIOS =====================
    TGraphErrors* r1 = MakeRatio_MatchingPoints(g1050, gsim);
    TGraphErrors* r2 = MakeRatio_MatchingPoints(gdec , gsim);
    TGraphErrors* r3 = MakeRatio_MatchingPoints(gdec , g1050);

    pR1->cd(); AutoRangeRatio(r1); r1->Draw("ALP"); StyleRatio(r1,false);
    pR2->cd(); AutoRangeRatio(r2); r2->Draw("ALP"); StyleRatio(r2,false);
    pR3->cd(); AutoRangeRatio(r3); r3->Draw("ALP"); StyleRatio(r3,true);

    for (auto p : {pR1,pR2,pR3}) {
        p->cd();
        auto l = new TLine(2,1,16,1);
        l->SetLineStyle(2);
        l->Draw();
        p->SetGridy();
    }
}
