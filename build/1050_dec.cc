#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TPad.h>
#include <TLegend.h>
#include <TLine.h>
#include <TAxis.h>
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

            if (xi == xj && yi > 0 && yj > 0) {
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
// CMS/ATLAS-style ratio axis formatting helper
// =======================================================
void StyleRatioGraph(TGraphErrors* gr, bool showX,
                     double ymin = 0.5, double ymax = 1.5)
{
    gr->GetYaxis()->SetRangeUser(ymin, ymax);
    gr->GetYaxis()->SetTitleSize(0.12);
    gr->GetYaxis()->SetLabelSize(0.10);
    gr->GetYaxis()->SetTitleOffset(0.45);
    gr->GetYaxis()->CenterTitle();

    gr->GetXaxis()->SetNdivisions(505);

    if (!showX) {
        gr->GetXaxis()->SetLabelSize(0);
        gr->GetXaxis()->SetTitleSize(0);
    } else {
        gr->GetXaxis()->SetLabelSize(0.12);
        gr->GetXaxis()->SetTitleSize(0.14);
        gr->GetXaxis()->SetTitleOffset(1.05);
    }

    gr->SetMarkerSize(0.9);
}

// =======================================================
// MAIN MACRO
// =======================================================
void ThreeCurves_WithAlignedRatios()
{
    // ===================== DATA =====================

    // ---- 1050V (3.5PE) ----
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

    // ---- dec2025 data ----
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

    // ---- Simulation (4PE) ----
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

    // ===================== GRAPHS =====================

    auto g1050 = new TGraphErrors(N1050,P1050,Y1050,nullptr,E1050);
    auto gdec  = new TGraphErrors(Ndec ,Pdec ,Ydec ,nullptr,Edec );
    auto gsim  = new TGraphErrors(Nsim ,Psim ,Ysim ,nullptr,Esim );

    g1050->SetMarkerStyle(21); g1050->SetMarkerColor(kRed);
    gdec ->SetMarkerStyle(34); gdec ->SetMarkerColor(kOrange+7);
    gsim ->SetMarkerStyle(29); gsim ->SetMarkerColor(kBlue);

    g1050->SetLineColor(kRed);
    gdec ->SetLineColor(kOrange+7);
    gsim ->SetLineColor(kBlue);

    // ===================== CANVAS & PADS =====================

    TCanvas *c = new TCanvas("c","Three curves with ratios",1200,900);

    TPad *pMain = new TPad("pMain","",0,0.40,1,1.0);
    TPad *pR1   = new TPad("pR1","",0,0.27,1,0.40);
    TPad *pR2   = new TPad("pR2","",0,0.14,1,0.27);
    TPad *pR3   = new TPad("pR3","",0,0.00,1,0.14);

    pMain->SetBottomMargin(0.015);

    pR1->SetTopMargin(0.02); pR1->SetBottomMargin(0.02);
    pR2->SetTopMargin(0.02); pR2->SetBottomMargin(0.02);
    pR3->SetTopMargin(0.02); pR3->SetBottomMargin(0.40);

    pMain->Draw();
    pR1->Draw();
    pR2->Draw();
    pR3->Draw();

    // ===================== MAIN PAD =====================

    pMain->cd();
    g1050->SetTitle("Efficiency vs Pressure;Pressure (atm);Efficiency (%)");
    g1050->GetYaxis()->SetRangeUser(0,60);
    g1050->Draw("ALP");
    gdec->Draw("LP SAME");
    gsim->Draw("LP SAME");

    auto leg = new TLegend(0.12,0.72,0.42,0.88);
    leg->AddEntry(g1050,"1050V (3.5PE)","lp");
    leg->AddEntry(gdec ,"dec2025 data","lp");
    leg->AddEntry(gsim ,"Simulation (4PE)","lp");
    leg->Draw();

    pMain->SetGrid();

    // ===================== RATIOS =====================

    struct RatioDef {
        TGraphErrors* num;
        TGraphErrors* den;
        const char*   title;
    } R[3] = {
        {g1050, gsim , "1050V / Simulation"},
        {gdec , gsim , "dec2025 / Simulation"},
        {gdec , g1050, "dec2025 / 1050V"}
    };

    TPad* pads[3] = {pR1,pR2,pR3};

    for(int i=0;i<3;i++){
        pads[i]->cd();

        auto gr = MakeRatio_MatchingPoints(R[i].num, R[i].den);
        gr->SetTitle(Form("%s;Pressure (atm);Ratio", R[i].title));
        gr->SetMarkerStyle(20);
        gr->Draw("ALP");

        StyleRatioGraph(gr, i==2);

        double xmin = gr->GetXaxis()->GetXmin();
        double xmax = gr->GetXaxis()->GetXmax();
        auto l = new TLine(xmin,1.0,xmax,1.0);
        l->SetLineStyle(2);
        l->SetLineWidth(2);
        l->Draw();

        pads[i]->SetGridy();
    }
}
