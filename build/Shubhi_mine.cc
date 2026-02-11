void EfficiencyOverlayWithRatio() {
    gStyle->SetOptStat(0);
    gStyle->SetEndErrorSize(4);

    // ========================================================================
    // 1. DATA DEFINITIONS
    // ========================================================================

    // Shubhi's analysis (3.5PE)
    const int Nslide = 14;
    double pressure_slide[Nslide] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double slide[Nslide] = {9.70,11.27,15.08,23.04,24.79,28.00,28.43,32.58,35.11,41.68,40.76,46.05,44.48,45.95};
    double slide_err[Nslide] = {0.42,0.86,1.02,1.63,1.62,1.55,1.59,1.44,1.74,1.80,1.84,1.78,1.89,1.75};

    // OldData_my analysis (3.5PE)
    const int Nexcel = 13;
    double pressure_excel[Nexcel] = {3,4,5,6,7,8,9,10,11,12,14,15,16};
    double datasetA[Nexcel] = {11.95,11.19,13.07,15.84,18.45,16.65,20.50,22.65,27.14,28.27,30.73,28.18,30.20};
    double NA[Nexcel] = {1355,1224,2035,1863,840,799,1056,746,748,711,781,685,1129};
    double A_err[Nexcel];
    for(int i=0;i<Nexcel;i++){
        double p = datasetA[i]/100.0;
        A_err[i] = 100.0 * sqrt(p*(1-p)/NA[i]);
    }

    // Shubhi's analysis (4PE)
    const int Nshubhi4 = 14;
    double pressure_shubhi4[Nshubhi4] = {2,3,4,5,6,7,8,9,10,11,12,14,15,16};
    double shubhi4[Nshubhi4] = {8.29,9.94,12.63,18.37,19.33,22.89,21.45,26.06,28.17,34.75,34.03,38.27,35.32,37.59};
    double shubhi4_err[Nshubhi4] = {0.39,0.81,0.95,1.50,1.48,1.45,1.45,1.35,1.64,1.74,1.77,1.74,1.82,1.70};

    // Newdata_my analysis (3.5PE)
    const int Ndec2025 = 9;
    double pressure_dec2025[Ndec2025] = {4.25,6,7.25,8,9,10,11,12,13};
    double dec2025_raw[Ndec2025] = {0.135,0.159,0.178,0.163,0.198,0.21,0.234,0.246,0.25};
    double dec2025_events[Ndec2025] = {467,2082,1971,2795,2750,2078,2061,2912,4419};
    double dec2025[Ndec2025], dec2025_err[Ndec2025];
    for (int i = 0; i < Ndec2025; i++) {
        dec2025[i] = 100.0 * dec2025_raw[i];
        dec2025_err[i] = 100.0 * sqrt(dec2025_raw[i]*(1-dec2025_raw[i])/dec2025_events[i]);
    }

    // OldData_my analysis (3PE)
    const int NOld3PE = 13;
    double pressure_Old3PE[NOld3PE] = {3,4,5,6,7,8,9,10,11,12,14,15,16};
    double data3Old[NOld3PE] = {11.73, 13.24, 16.53, 19.45, 21.79, 20.40, 24.81, 26.14, 32.88, 32.5, 36.0, 33.4, 35.7};
    double Neve[NOld3PE] = {1355,1224,2035,1863,840,799,1056,746,748,711,781,685,1129};
    double D_err[NOld3PE];
    for(int i=0;i<NOld3PE;i++){
        double p = data3Old[i]/100.0;
        D_err[i] = 100.0 * sqrt(p*(1-p)/Neve[i]);
    }

    // ========================================================================
    // 2. GRAPH CREATION & STYLING
    // ========================================================================

    std::vector<std::pair<std::string,TGraphErrors*>> graphs = {
        {"Shubhi's analysis (3.5PE)",   new TGraphErrors(Nslide, pressure_slide, slide, nullptr, slide_err)},
        {"OldData_my analysis (3.5PE)", new TGraphErrors(Nexcel, pressure_excel, datasetA, nullptr, A_err)},
        {"Shubhi's analysis (4PE)",     new TGraphErrors(Nshubhi4, pressure_shubhi4, shubhi4, nullptr, shubhi4_err)},
        {"Newdata_my analysis (3.5PE)", new TGraphErrors(Ndec2025, pressure_dec2025, dec2025, nullptr, dec2025_err)},
        {"OldData_my analysis (3PE)",   new TGraphErrors(NOld3PE, pressure_Old3PE, data3Old, nullptr, D_err)}
    };

    int colors[]  = {kBlack, kRed, kGreen+2, kViolet+1, kOrange+7};
    int markers[] = {20, 21, 29, 33, 34};

    for (size_t i=0; i<graphs.size(); ++i) {
        graphs[i].second->SetMarkerStyle(markers[i]);
        graphs[i].second->SetMarkerColor(colors[i]);
        graphs[i].second->SetLineColor(colors[i]);
        graphs[i].second->SetLineWidth(2);
    }

    // ========================================================================
    // 3. CANVAS & PAD CONFIGURATION
    // ========================================================================

    TCanvas *c1 = new TCanvas("c1","Efficiency Overlay with Ratio",1000,900);
    TPad *pad1 = new TPad("pad1","Main Plot",0,0.35,1,1.0);
    TPad *pad2 = new TPad("pad2","Ratio Plot",0,0.0,1,0.35);

    pad1->SetBottomMargin(0.02); // Minimize gap
    pad2->SetTopMargin(0.02);
    pad2->SetBottomMargin(0.25);
    pad1->SetGrid();
    pad2->SetGrid();
    pad1->Draw();
    pad2->Draw();

    // ------------------- Top Pad (Main Plot) -------------------
    pad1->cd();
    graphs[0].second->GetYaxis()->SetRangeUser(0,60);
    graphs[0].second->GetXaxis()->SetLimits(1,17);
    graphs[0].second->GetXaxis()->SetLabelSize(0); // Hide X labels on top
    graphs[0].second->SetTitle("Efficiency Comparison;Pressure (atm);Efficiency (%)");
    graphs[0].second->Draw("ALP");

    for (size_t i=1; i<graphs.size(); ++i) {
        graphs[i].second->Draw("LP SAME");
    }

    auto leg = new TLegend(0.12,0.65,0.45,0.88);
    for (auto &g : graphs) leg->AddEntry(g.second, g.first.c_str(), "lp");
    leg->Draw();

    // ------------------- Bottom Pad (Ratio Plot) -------------------
    pad2->cd();

    // Dataset Indices for Ratio: Shubhi 4PE (index 2) / Old Data 3PE (index 4)
    TGraphErrors* gNum = graphs[2].second; // Shubhi 4PE
    TGraphErrors* gDen = graphs[4].second; // Old 3PE
    
    std::vector<double> rP, rY, rE;
    for (int i=0; i < gNum->GetN(); i++) {
        double px, py; gNum->GetPoint(i, px, py);
        for (int j=0; j < gDen->GetN(); j++) {
            double qx, qy; gDen->GetPoint(j, qx, qy);
            if (std::abs(px - qx) < 1e-4 && qy > 0) {
                double ratio = py / qy;
                double err = ratio * sqrt(pow(gNum->GetErrorY(i)/py, 2) + pow(gDen->GetErrorY(j)/qy, 2));
                rP.push_back(px);
                rY.push_back(ratio);
                rE.push_back(err);
                break;
            }
        }
    }

    TGraphErrors *gRatio = new TGraphErrors(rP.size(), &rP[0], &rY[0], nullptr, &rE[0]);
    gRatio->SetMarkerStyle(20);
    gRatio->SetMarkerColor(kBlue+2);
    gRatio->SetLineColor(kBlue+2);
    gRatio->SetTitle(";Pressure (atm);Shubhi 4PE / Old 3PE");
    gRatio->GetXaxis()->SetLimits(1,17);
    gRatio->GetYaxis()->SetRangeUser(0.2, 1.8);
    
    // Axis styling to make it readable in the small pad
    gRatio->GetXaxis()->SetTitleSize(0.1);
    gRatio->GetXaxis()->SetLabelSize(0.08);
    gRatio->GetYaxis()->SetTitleSize(0.08);
    gRatio->GetYaxis()->SetLabelSize(0.07);
    gRatio->GetYaxis()->SetTitleOffset(0.5);
    gRatio->Draw("APL");

    TLine *line = new TLine(1, 1, 17, 1);
    line->SetLineStyle(2);
    line->SetLineColor(kGray+2);
    line->Draw();

    c1->SaveAs("Efficiency_Overlay_Ratio_Comparison.pdf");
}
