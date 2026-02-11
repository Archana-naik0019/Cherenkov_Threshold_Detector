void OverlayWithRatio() {
    gStyle->SetOptStat(0);

    // ---------------------------------------------------------
    // 1. DATA: my Analysis (3.5 PE)
    // ---------------------------------------------------------
    const int Nshubhi4 = 9;
    double pressure_shubhi4[Nshubhi4] = {4.25,6,7.25,8,9,10,11,12,13};
    //double shubhi4[Nshubhi4] = {8.29,9.94,12.63,18.37,19.33,22.89,21.45,26.06,28.17,34.75,34.03,38.27,35.32,37.59};
    double shubhi4[Nshubhi4]= {13.5,15.9,17.8,16.3,19.8,21.0,23.4,24.6,25.0}; //shubhi 3.5
    //double shubhi4_err[Nshubhi4] = {0.42,0.86,1.02,1.63,1.62,1.55,1.59,1.44,1.74,1.80,1.84,1.78,1.89,1.75};
    double NA[Nshubhi4] = {467,2082,1971,2795,2750,2078,2061,2912,4419};
    double shubhi4_err[Nshubhi4];
    for(int i=0;i<Nshubhi4;i++){
        double p = shubhi4[i]/100.0;
        shubhi4_err[i] = 100.0 * sqrt(p*(1-p)/NA[i]);
    }

    // ---------------------------------------------------------
    // 2. SIMULATION: Mylar 0.95 and Quartz JGS2
    // ---------------------------------------------------------
    const int Nnew = 15;
    double Pnew[Nnew] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double YnewRaw[Nnew] = {0.1061643836,0.15,0.1660777385,0.2251655629,0.25,0.2974683544,0.3442622951,0.3375394322,0.3550488599,0.4033333333,0.417721519,0.4229508197,0.4215686275,0.4899328859,0.4613003096};
    double NnewEv[Nnew] = {292,300,283,302,320,316,305,317,307,300,316,305,306,298,323};
    double Ynew[Nnew], Enew[Nnew];
    for(int i=0; i<Nnew; i++) {
        Ynew[i] = 100. * YnewRaw[i];
        Enew[i] = 100. * std::sqrt(YnewRaw[i] * (1.0 - YnewRaw[i]) / NnewEv[i]);
    }

    // -----------------------------
    // Graphs Creation & Styling
    // -----------------------------
    auto gData = new TGraphErrors(Nshubhi4, pressure_shubhi4, shubhi4, nullptr, shubhi4_err);
    gData->SetMarkerStyle(20);
    gData->SetMarkerColor(kBlack);
    gData->SetLineColor(kBlack);
    gData->SetTitle("Old data 3.5PE vs JGS2 Simulation;Pressure (atm);Efficiency (%)");

    auto gSim = new TGraphErrors(Nnew, Pnew, Ynew, nullptr, Enew);
    gSim->SetMarkerStyle(24);
    gSim->SetMarkerColor(kRed);
    gSim->SetLineColor(kRed);

    // -----------------------------
    // Canvas with ratio pad
    // -----------------------------
    TCanvas *c1 = new TCanvas("c1","Overlay with Ratio",800,800);

    TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1.0);
    TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
    pad1->SetBottomMargin(0.02); // slight gap
    pad2->SetTopMargin(0.02); 
    pad2->SetBottomMargin(0.3);
    pad1->Draw(); pad2->Draw();

    // Top pad: overlay
    pad1->cd();
    pad1->SetGrid();
    gData->Draw("APL");
    
    gData->GetXaxis()->SetLimits(0, 18);   
    gData->GetYaxis()->SetRangeUser(0, 60); 
    
    gSim->Draw("PL SAME");
    gSim->Draw("E1 SAME");

    auto legend = new TLegend(0.15,0.7,0.55,0.88);
    legend->AddEntry(gData,"New Data_My analysis (3.5 PE)","lep");
    legend->AddEntry(gSim,"Sim (Mylar 0.95, JGS2)","lep");
    legend->Draw();

    // Bottom pad: ratio (Data / Sim)
    pad2->cd();
    pad2->SetGrid();

    std::vector<double> rP, rY, rE;
    // Logical match: Only calculate ratio where pressure points exist in both
    for (int i=0; i<Nshubhi4; i++) {
        for (int j=0; j<Nnew; j++) {
            if (std::abs(pressure_shubhi4[i] - Pnew[j]) < 1e-5) {
                double ratio = shubhi4[i] / Ynew[j];
                double err = ratio * std::sqrt(std::pow(shubhi4_err[i]/shubhi4[i],2) + std::pow(Enew[j]/Ynew[j],2));
                rP.push_back(pressure_shubhi4[i]);
                rY.push_back(ratio);
                rE.push_back(err);
                break;
            }
        }
    }

    TGraphErrors *gRatio = new TGraphErrors(rP.size(), &rP[0], &rY[0], nullptr, &rE[0]);
    gRatio->SetMarkerStyle(21);
    gRatio->SetMarkerColor(kBlue);
    gRatio->SetLineColor(kBlue);
    gRatio->SetTitle(";Pressure (atm);Data/Sim");
    gRatio->Draw("APL");
    
    // Axis Styling for Ratio
    gRatio->GetXaxis()->SetLimits(0, 18);
    gRatio->GetYaxis()->SetRangeUser(0.5, 1.5); // centered around 1.0
    gRatio->GetXaxis()->SetTitleSize(0.12);
    gRatio->GetYaxis()->SetTitleSize(0.12);
    gRatio->GetXaxis()->SetLabelSize(0.1);
    gRatio->GetYaxis()->SetLabelSize(0.06);
    gRatio->GetYaxis()->SetTitleOffset(0.4);
    gRatio->GetXaxis()->SetTitleOffset(1.1);

    // Reference Line at 1.0
    TLine *line = new TLine(0, 1, 18, 1);
    line->SetLineStyle(2);
    line->SetLineColor(kGray+2);
    line->Draw();

    c1->SaveAs("NewDataMyAnalysis3.5PE_vs_JGS2_Ratio.pdf");
}
