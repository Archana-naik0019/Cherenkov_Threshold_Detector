void OverlayWithRatio() {
    gStyle->SetOptStat(0);

    // -----------------------------
    // DATA from slide 13 (col %(4))
    // -----------------------------
    //const int N = 15;
    const int N = 13;
    double pressure[N]   = {2,3,4,5,6,7,8,9,10,11,13,14,15};
    double data[N]       = {12.96023564, 17.52241239, 18.56240126, 33.7535014, 36.41755635, 36.90773067, 42.68177526, 45.52736983, 50.59920107, 50.98039216, 57.65306122, 54.65116279, 56.38820639}; // %
    double data_err[N]   = {0.911413416, 1.085282105, 1.092729594, 1.769667903, 1.65733469, 1.703960651, 1.519916126, 1.819636101, 1.824394892, 1.870843228, 1.764672692, 1.897966585, 1.738136629}; // %

    // -----------------------------
    // SIMULATION
    // -----------------------------
    double sim[N] = {5.821917808, 10.51051051, 12.68115942, 15.14084507, 21.23287671, 27.89115646, 28.91986063, 34.38596491, 38.98916968, 36.04240283, 43.06049822, 42.37288136, 40.70512821};
    
    double sim_err[N] = {1.370302754, 1.680644289, 2.002990642, 2.126988976, 2.393235772, 2.615494801, 2.676278374, 2.813627183, 2.930458507, 2.854037795, 2.953882369, 2.877043599, 2.78135122};

/*    double sim_err[N];
    int Nev = 100000;
    for (int i=0; i<N; i++) {
        double p = sim[i]/100.0;
        sim_err[i] = sqrt(p*(1.0-p)/Nev) * 100.0;
    }
    
    for (int i=0; i<N; i++) {
    std::cout << " err=" << sim_err[i] << std::endl;
}
*/
// --- exaggerate errors for visibility in overlay ---
//for (int i=0; i<N; i++) sim_err[i] *= 10;


    // -----------------------------
    // Graphs
    // -----------------------------
    TGraphErrors *gData = new TGraphErrors(N, pressure, data, 0, data_err);
    gData->SetMarkerStyle(20);
    gData->SetMarkerColor(kBlack);
    gData->SetLineColor(kBlack);
    gData->SetTitle("Data vs Simulation;Pressure (atm);Efficiency (%)");

    TGraphErrors *gSim = new TGraphErrors(N, pressure, sim, 0, sim_err);
    gSim->SetMarkerStyle(24);
    gSim->SetMarkerColor(kRed);
    gSim->SetLineColor(kRed);

    // -----------------------------
    // Canvas with ratio pad
    // -----------------------------
    TCanvas *c1 = new TCanvas("c1","Overlay with Ratio",800,800);

    TPad *pad1 = new TPad("pad1","pad1",0,0.3,1,1.0);
    TPad *pad2 = new TPad("pad2","pad2",0,0.0,1,0.3);
    pad1->SetBottomMargin(0); // join pads
    pad2->SetTopMargin(0); pad2->SetBottomMargin(0.25);
    pad1->Draw(); pad2->Draw();

    // Top pad: overlay
    pad1->cd();
    gData->Draw("AP");
    
    gData->GetXaxis()->SetLimits(0, 17);   // X-axis from 0 to 17 atm
    gData->GetYaxis()->SetRangeUser(0, 50); // Y-axis from 0 to max ratio
    
    gSim->Draw("P SAME");
    gSim->Draw("E1 SAME");

    
    auto legend = new TLegend(0.15,0.7,0.45,0.85);
    legend->AddEntry(gData,"Shubhi_3.5PE","lep");
    legend->AddEntry(gSim,"Simulation","lep");
    legend->Draw();

    // Bottom pad: ratio
    pad2->cd();
    pad2->SetGridy();
    double ratio[N], ratio_err[N];
    for (int i=0; i<N; i++) {
        ratio[i] = (sim[i] != 0) ? data[i]/sim[i] : 0;
        // propagate only data error + sim error
        ratio_err[i] = ratio[i] * sqrt( pow(data_err[i]/data[i],2) + pow(sim_err[i]/sim[i],2) );
    }
    TGraphErrors *gRatio = new TGraphErrors(N, pressure, ratio, 0, ratio_err);
    gRatio->SetMarkerStyle(21);
    gRatio->SetMarkerColor(kBlue);
    gRatio->SetLineColor(kBlue);
    gRatio->SetTitle(";Pressure (atm);Data/Sim");
    gRatio->Draw("APL");
    
    gRatio->GetXaxis()->SetTitleSize(0.09);   // bigger X-axis title
    gRatio->GetYaxis()->SetTitleSize(0.09);   // bigger Y-axis title
    gRatio->GetXaxis()->SetLabelSize(0.08);   // bigger X-axis labels
    gRatio->GetYaxis()->SetLabelSize(0.08);   // bigger Y-axis labels

    //gRatio->GetXaxis()->SetTitleOffset(1.0);  // adjust title offset
    gRatio->GetYaxis()->SetTitleOffset(0.5);  // keep Y-axis title closer

    c1->SaveAs("Overlay_Ratio.pdf");
}

