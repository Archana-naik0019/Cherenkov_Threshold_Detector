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
        ymin-1 - margin*span,
        ymax+1 + margin*span
    );
}

// =======================================================
void StyleRatio(TGraphErrors* gr)
{
    gr->GetYaxis()->CenterTitle();
    gr->GetYaxis()->SetTitleSize(0.075);
    gr->GetYaxis()->SetLabelSize(0.060);
    gr->GetYaxis()->SetTitleOffset(0.70);

    gr->GetXaxis()->SetTitle("Pressure (atm)");
    gr->GetXaxis()->SetLabelSize(0.065);
    gr->GetXaxis()->SetTitleSize(0.080);
    gr->GetXaxis()->SetTitleOffset(1.10);
    gr->GetXaxis()->SetNdivisions(505);

    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.70);
}

// =======================================================
void DrawCanvas(const char* cname,
                TGraphErrors* g1050,
                TGraphErrors* gdec,
                TGraphErrors* gsim,
                TGraphErrors* gmy95,
                TGraphErrors* gmy97,
                TGraphErrors* ratio,
                const char* ratioYTitle)
{
    const double XMIN = 1.0;
    const double XMAX = 17.0;

    TCanvas *c = new TCanvas(cname, cname, 1200, 900);

    TPad *pMain = new TPad("pMain","",0,0.35,1,1.0);
    TPad *pRat  = new TPad("pRat","",0,0.00,1,0.35);

    for (auto p : {pMain,pRat}) {
        p->SetLeftMargin(0.14);
        p->SetRightMargin(0.05);
    }

    pMain->SetBottomMargin(0.02);
    pRat->SetBottomMargin(0.35);

    pMain->Draw();
    pRat->Draw();

    // ===================== MAIN =====================
    pMain->cd();

    g1050->SetTitle("Efficiency vs Pressure;Pressure (atm);Efficiency (%)");
    g1050->GetYaxis()->SetRangeUser(0,60);
    g1050->GetXaxis()->SetLimits(XMIN, XMAX);

    g1050->Draw("ALP");
    gdec ->Draw("LP SAME");
    gsim ->Draw("LP SAME");
    gmy95->Draw("LP SAME");
    gmy97->Draw("LP SAME");

    auto leg = new TLegend(0.12,0.62,0.45,0.88);
    leg->SetBorderSize(1);
    leg->SetFillStyle(1001);
    leg->SetTextSize(0.040);

    leg->AddEntry(g1050,"Old data (3.5 PE)","lp");
    leg->AddEntry(gdec ,"New data (3.5 PE)","lp");
    leg->AddEntry(gsim ,"Simulation (4 PE)","lp");
    leg->AddEntry(gmy95,"Mylar R = 0.95","lp");
    leg->AddEntry(gmy97,"Mylar R = 0.97","lp");
    leg->Draw();

    pMain->SetGrid();

    // ===================== RATIO =====================
    pRat->cd();

    ratio->SetTitle(";Pressure (atm);");
    ratio->GetYaxis()->SetTitle(ratioYTitle);
    ratio->GetXaxis()->SetLimits(XMIN, XMAX);

    ratio->Draw("ALP");
    AutoRangeRatio(ratio);
    StyleRatio(ratio);

    auto l = new TLine(XMIN,1.0,XMAX,1.0);
    l->SetLineStyle(2);
    l->Draw();

    pRat->SetGridy();
}

// =======================================================
// MAIN MACRO
// =======================================================
void FiveCurves_AllPairRatios()
{
    gStyle->SetEndErrorSize(4);

    // ===================== 1050 =====================
    const int N1050 = 13;
    double P1050[N1050]={3,4,5,6,7,8,9,10,11,12,14,15,16};
    double Y1050[N1050]={11.95,11.19,13.07,15.84,18.45,16.65,
                         20.50,22.65,27.14,28.27,30.73,28.18,30.20};
    double N1050ev[N1050]={1355,1224,2035,1863,840,799,
                           1056,746,748,711,781,685,1129};

    double E1050[N1050];
    for(int i=0;i<N1050;i++){
        double p=Y1050[i]/100.;
        E1050[i]=100.*sqrt(p*(1-p)/N1050ev[i]);
    }

    // ===================== dec =====================
    const int Ndec=9;
    double Pdec[Ndec]={4.25,6,7.25,8,9,10,11,12,13};
    double YdecRaw[Ndec]={0.135,0.159,0.178,0.163,0.198,
                          0.21,0.234,0.246,0.25};
    double NdecEv[Ndec]={467,2082,1971,2795,2750,
                         2078,2061,2912,4419};

    double Ydec[Ndec],Edec[Ndec];
    for(int i=0;i<Ndec;i++){
        Ydec[i]=100.*YdecRaw[i];
        Edec[i]=100.*sqrt(YdecRaw[i]*(1-YdecRaw[i])/NdecEv[i]);
    }

    // ===================== sim =====================
    const int Nsim=15;
    double Psim[Nsim]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double Ysim[Nsim]={5.137,9.3093,11.5942,14.4366,20.5479,
                       26.8707,27.5261,31.5789,36.4621,33.9223,
                       41.0909,40.5594,41.0169,37.50,46.595};
    double NsimEv[Nsim]={292,333,276,284,292,294,287,285,
                         277,283,275,281,295,312,279};

    double Esim[Nsim];
    for(int i=0;i<Nsim;i++){
        double p=Ysim[i]/100.;
        Esim[i]=100.*sqrt(p*(1-p)/NsimEv[i]);
    }

    // ===================== Mylar 0.95 =====================
    const int Nmy=8;
    double Pmy[Nmy]={2,4,6,8,10,12,14,16};
    double Ymy95raw[Nmy]={0.02317880795,0.1232394366,0.1847133758,
                          0.2564102564,0.3097643098,0.3766666667,
                          0.3993288591,0.4607142857};
    double Nmy95[Nmy]={302,284,314,312,297,300,298,280};

    double Ymy95[Nmy],Emy95[Nmy];
    for(int i=0;i<Nmy;i++){
        Ymy95[i]=100.*Ymy95raw[i];
        Emy95[i]=100.*sqrt(Ymy95raw[i]*(1-Ymy95raw[i])/Nmy95[i]);
    }

    // ===================== Mylar 0.97 =====================
    double Ymy97raw[Nmy]={0.06229508197,0.1111111111,0.2191780822,
                          0.2455516014,0.335504886,0.3609271523,
                          0.4285714286,0.4353741497};
    double Nmy97[Nmy]={305,288,292,281,307,302,280,294};

    double Ymy97[Nmy],Emy97[Nmy];
    for(int i=0;i<Nmy;i++){
        Ymy97[i]=100.*Ymy97raw[i];
        Emy97[i]=100.*sqrt(Ymy97raw[i]*(1-Ymy97raw[i])/Nmy97[i]);
    }

    // ===================== GRAPHS =====================
    auto g1050=new TGraphErrors(N1050,P1050,Y1050,nullptr,E1050);
    auto gdec =new TGraphErrors(Ndec ,Pdec ,Ydec ,nullptr,Edec );
    auto gsim =new TGraphErrors(Nsim ,Psim ,Ysim ,nullptr,Esim );
    auto gmy95=new TGraphErrors(Nmy  ,Pmy  ,Ymy95,nullptr,Emy95);
    auto gmy97=new TGraphErrors(Nmy  ,Pmy  ,Ymy97,nullptr,Emy97);

    g1050->SetMarkerStyle(21); g1050->SetMarkerColor(kRed+1);   g1050->SetLineColor(kRed+1);
    gdec ->SetMarkerStyle(23); gdec ->SetMarkerColor(kOrange+7);gdec ->SetLineColor(kOrange+7);
    gsim ->SetMarkerStyle(25); gsim ->SetMarkerColor(kBlue+1);  gsim ->SetLineColor(kBlue+1);
    gmy95->SetMarkerStyle(26); gmy95->SetMarkerColor(kGreen+2); gmy95->SetLineColor(kGreen+2);
    gmy97->SetMarkerStyle(32); gmy97->SetMarkerColor(kMagenta+1);gmy97->SetLineColor(kMagenta+1);

    // ===================== ALL PAIR RATIOS =====================
    DrawCanvas("r_1050_sim", g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(g1050,gsim),"1050 / Sim");
    DrawCanvas("r_dec_sim",  g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gdec ,gsim),"Dec / Sim");
    DrawCanvas("r_my95_sim", g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gmy95,gsim),"My95 / Sim");
    DrawCanvas("r_my97_sim", g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gmy97,gsim),"My97 / Sim");

    DrawCanvas("r_dec_1050", g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gdec ,g1050),"Dec / 1050");
    DrawCanvas("r_my95_1050",g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gmy95,g1050),"My95 / 1050");
    DrawCanvas("r_my97_1050",g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gmy97,g1050),"My97 / 1050");

    DrawCanvas("r_my95_dec", g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gmy95,gdec),"My95 / Dec");
    DrawCanvas("r_my97_dec", g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gmy97,gdec),"My97 / Dec");

    DrawCanvas("r_my97_my95",g1050,gdec,gsim,gmy95,gmy97, MakeRatio_MatchingPoints(gmy97,gmy95),"My97 / My95");
}

