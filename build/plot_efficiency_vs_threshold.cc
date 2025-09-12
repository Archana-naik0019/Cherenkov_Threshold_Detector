#include <TCanvas.h>
#include <TGraph.h>
#include <TF1.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TStyle.h>

void plot_efficiency_vs_threshold() {
    const int N = 15;

    double threshold[N] = {3.115,2.536,2.176,1.935,1.75,1.625,1.535,1.465,1.385,1.285,1.215,1.167,1.126,1.075,1.045};
    double eff_gt1[N]   = {0.099315,0.129129,0.199275,0.221831,0.291096,0.357143,0.379791,0.396491,0.440433,0.431095,0.490909,0.476868,0.454237,0.464744,0.53405};
    double eff_gt2[N]   = {0.075342,0.111111,0.15942,0.179577,0.243151,0.323129,0.320557,0.361404,0.407942,0.385159,0.469091,0.448399,0.406769,0.432692,0.519713};
    double eff_gt3[N]   = {0.058219,0.105105,0.093085,0.151408,0.213229,0.278912,0.289199,0.34386,0.389892,0.360424,0.454545,0.423077,0.423729,0.407051,0.494624};
    double eff_gt3_5[N] = {0.05137,0.093093,0.115942,0.144366,0.205479,0.268707,0.275261,0.315789,0.346621,0.339223,0.410909,0.405594,0.410169,0.375,0.465995};
    double eff_gt4[N]   = {0.05137,0.093093,0.115942,0.144366,0.205479,0.268707,0.275261,0.315789,0.346621,0.339223,0.410909,0.405594,0.410169,0.375,0.465995};
    double eff_gt4_5[N] = {0.037671,0.087087,0.101449,0.130282,0.202055,0.244898,0.236934,0.282411,0.33935,0.314488,0.392727,0.374126,0.389381,0.371795,0.44086};

    // Array of labels & styles
    const char* labels[6] = {">=1 PE",">=2 PE",">=3 PE",">=3.5 PE",">=4 PE",">=4.5 PE"};
    double* effs[6] = {eff_gt1, eff_gt2, eff_gt3, eff_gt3_5, eff_gt4, eff_gt4_5};
    int colors[6] = {kRed, kBlue, kGreen+2, kMagenta, kOrange+7, kCyan+2};
    int markers[6] = {20, 21, 22, 23, 24, 25};

    // --- Separate plots ---
    TCanvas *c_sep = new TCanvas("c_sep","Separate Fits",1200,800);
    c_sep->Divide(3,2);

    TF1* fits[6]; // store fits for overlay

    for (int i=0;i<6;i++) {
        c_sep->cd(i+1);

        TGraph *g = new TGraph(N,threshold,effs[i]);
        g->SetMarkerStyle(markers[i]);
        g->SetMarkerColor(colors[i]);
        g->SetTitle(Form("%s;Threshold (GeV);Efficiency",labels[i]));
        g->Draw("AP");

        // Fit (2nd order poly, change to pol3/pol4 if needed)
        fits[i] = new TF1(Form("fit%d",i),"pol2",1.0,3.2);
        fits[i]->SetLineColor(colors[i]);
        fits[i]->SetLineStyle(2);
        g->Fit(fits[i],"RQ"); // R=range, Q=quiet
    }

    // --- Overlay only trendlines ---
    TCanvas *c_overlay = new TCanvas("c_overlay","Overlay Fits Only",800,600);

    bool first = true;
    for (int i=0;i<6;i++) {
        if (first) {
            fits[i]->SetTitle("Efficiency vs Threshold;Threshold (GeV);Efficiency");
            fits[i]->Draw(); // first one
            first = false;
        } else {
            fits[i]->Draw("SAME");
        }
    }

    auto legend = new TLegend(0.6,0.2,0.88,0.45);
    for (int i=0;i<6;i++) legend->AddEntry(fits[i],labels[i],"l");
    legend->Draw();
}

