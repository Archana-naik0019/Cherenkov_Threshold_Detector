#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TH1.h>
#include <cmath>
#include <map>
#include <vector>
#include <string>

void Overlaid_3p5PE_Avg_withRatios() {

    // --- Original data definitions ---
    const int Nslide = 14;
    double pressure_slide[Nslide] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double slide[Nslide] = {9.70,11.27,15.08,23.04,24.79,28.00,28.43,32.58,35.11,41.68,40.76,46.05,44.48,45.95};
    double slide_err[Nslide] = {0.42,0.86,1.02,1.63,1.62,1.55,1.59,1.44,1.74,1.80,1.84,1.78,1.89,1.75};

    const int Nexcel = 13;
    double pressure_excel[Nexcel] = {2,3,4,5,6,7,8,9,10,11,13,14,15};
    double datasetA[Nexcel] = {11.95,11.19,13.07,15.84,18.45,16.65,20.50,22.65,27.14,28.27,30.73,28.18,30.20};
    double NA[Nexcel] = {1355,1224,2035,1863,840,799,1056,746,748,711,781,685,1129};
    double A_err[Nexcel];
    for(int i=0;i<Nexcel;i++){
        double p = datasetA[i]/100.0;
        A_err[i] = 100.0 * sqrt(p*(1-p)/NA[i]);
    }

    double datasetB[Nexcel] = {16.52,17.16,20.72,24.40,27.62,27.41,31.34,33.24,40.78,39.24,43.53,42.48,44.19};
    double B_err[Nexcel];
    for(int i=0;i<Nexcel;i++){
        double p = datasetB[i]/100.0;
        B_err[i] = 100.0 * sqrt(p*(1-p)/NA[i]);
    }

    const int Nsim = 15;
    double pressure_sim[Nsim] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double sim4PE[Nsim] = {5.137,9.3093,11.5942,14.4366,20.5479,26.8707,27.5261,31.5789,36.4621,33.9223,41.0909,40.5594,41.0169,37.50,46.595};
    double sim_total[Nsim] = {292,333,276,284,292,294,287,285,277,283,275,281,295,312,279};
    double sim_err[Nsim];
    for(int i=0;i<Nsim;i++){
        double p = sim4PE[i] / 100;
        sim_err[i] = 100.0 * sqrt(p*(1-p)/sim_total[i]);
    }

    const int Nshubhi4 = 14;
    double pressure_shubhi4[Nshubhi4] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double shubhi4[Nshubhi4] = {8.29,9.94,12.63,18.37,19.33,22.89,21.45,26.06,28.17,34.75,34.03,38.27,35.32,37.59};
    double shubhi4_err[Nshubhi4] = {0.39,0.81,0.95,1.5,1.48,1.45,1.45,1.35,1.64,1.74,1.77,1.74,1.82,1.7};

    // --- Create all TGraphErrors ---
    std::vector<std::pair<std::string,TGraphErrors*>> graphs = {
        {"Shubhi_data (3.5PE)", new TGraphErrors(Nslide, pressure_slide, slide, nullptr, slide_err)},
        {"at_1050V (3.5PE)",     new TGraphErrors(Nexcel, pressure_excel, datasetA, nullptr, A_err)},
        {"at_1000V (3.5PE)",     new TGraphErrors(Nexcel, pressure_excel, datasetB, nullptr, B_err)},
        {"Simulation (4PE)",     new TGraphErrors(Nsim, pressure_sim, sim4PE, nullptr, sim_err)},
        {"Shubhi_data (4PE)",    new TGraphErrors(Nshubhi4, pressure_shubhi4, shubhi4, nullptr, shubhi4_err)}
    };

    int colors[] = {kBlack, kRed, kBlue, kGreen+2, kViolet+1};
    int markers[] = {20,21,22,33,29};

    for (size_t i=0;i<graphs.size();++i) {
        graphs[i].second->SetMarkerStyle(markers[i]);
        graphs[i].second->SetMarkerColor(colors[i]);
        graphs[i].second->SetLineColor(colors[i]);
        graphs[i].second->SetLineWidth(3);
    }

    // --- Main plot ---
    TCanvas *c1 = new TCanvas("c1","Main Plot",1000,650);
    graphs[0].second->SetTitle("Efficiency vs Pressure;Pressure (atm);Efficiency (%)");
    graphs[0].second->GetYaxis()->SetRangeUser(0,60);
    graphs[0].second->Draw("ALP");
    for (size_t i=1;i<graphs.size();++i)
        graphs[i].second->Draw("LP SAME");

    auto leg = new TLegend(0.12,0.68,0.45,0.90);
    for (auto &g : graphs)
        leg->AddEntry(g.second, g.first.c_str(), "lp");
    leg->Draw();
    c1->SetGrid();

    // --- Ratio plots (mutual pair ratios) ---
    auto get_common_ratio_graph = [&](TGraphErrors* g1, TGraphErrors* g2){
        std::vector<double> x, y, ex, ey;
        for (int i=0;i<g1->GetN();++i){
            double x1, y1; g1->GetPoint(i, x1, y1);
            double e1 = g1->GetErrorY(i);
            // find same x in g2
            for (int j=0;j<g2->GetN();++j){
                double x2, y2; g2->GetPoint(j, x2, y2);
                if (fabs(x1-x2)<1e-6){ // match pressure
                    double e2 = g2->GetErrorY(j);
                    if (y2 != 0){
                        double r = y1/y2;
                        double er = r * sqrt( pow(e1/y1,2) + pow(e2/y2,2) );
                        x.push_back(x1);
                        y.push_back(r);
                        ex.push_back(0);
                        ey.push_back(er);
                    }
                }
            }
        }
        return new TGraphErrors(x.size(), x.data(), y.data(), ex.data(), ey.data());
    };

    TCanvas *c2 = new TCanvas("c2","All Ratio Plots",1200,800);
    c2->Divide(4,3); // up to 12 pads (10 used)

    int pad = 1;
    for (size_t i=0;i<graphs.size();++i){
        for (size_t j=i+1;j<graphs.size();++j){
            c2->cd(pad++);
            auto *g_ratio = get_common_ratio_graph(graphs[i].second, graphs[j].second);
            g_ratio->SetTitle(Form("Ratio: %s / %s;Pressure (atm);Ratio", graphs[i].first.c_str(), graphs[j].first.c_str()));
            g_ratio->SetMarkerStyle(20);
            g_ratio->Draw("ALP");
            
            // a horizontal line at ratio = 1
            double xmin = g_ratio->GetXaxis()->GetXmin();
            double xmax = g_ratio->GetXaxis()->GetXmax();
            TLine *line1 = new TLine(xmin, 1.0, xmax, 1.0);
            line1->SetLineStyle(2);  // dashed
            line1->SetLineColor(kGray+2);
            line1->Draw("SAME");
            
            gPad->SetGrid();
        }
    }
}

