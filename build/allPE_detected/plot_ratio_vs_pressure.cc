void plot_ratio_vs_pressure() {
    // Pressures (atm) → same as "i" in allPE_i.root
    const int N = 15;
    double pressure[N] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

    // Ratios (fill with values you got from previous macro)
    double ratio[N] = {
        0.00515011,  0.00576669, 0.00549264, 0.00526331, 0.0062135,
        0.00517661,  0.00520344,  0.00521472, 0.00512505, 0.00542758,
        0.00533647, 0.00511049, 0.0051166, 0.00491388, 0.00486345
    };

    // Create TGraph
    TGraph *gr = new TGraph(N, pressure, ratio);

    gr->SetTitle("Ratio vs Pressure;Pressure (atm);Ratio");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.2);
    gr->SetLineColor(kBlue+2);
    gr->SetLineWidth(2);

    TCanvas *c1 = new TCanvas("c1","Ratio vs Pressure",800,600);

    // Force axis to start from 0,0
    gr->GetXaxis()->SetLimits(0, 17);  // X-axis: from 0 to just above max pressure
    gr->GetYaxis()->SetRangeUser(0, 0.007);  // Y-axis: from 0 to just above max ratio

    gr->Draw("APL");  // A=axes, P=points, L=line

    c1->SaveAs("ratio_vs_pressure_2.png");  // save the plot
}

