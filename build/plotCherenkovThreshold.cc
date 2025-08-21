void plotCherenkovThreshold() {
    // Data: Pressure (atm) vs Cherenkov Threshold Energy (GeV)
    const int n = 15;
    double pressure[n] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    double threshold[n] = {2.985, 2.415, 2.075, 1.855, 1.685, 1.555, 1.445, 1.355, 1.285, 1.225, 1.165, 1.115, 1.075, 1.035, 0.995};

    // Dummy error bars (use real ones if available)
    double xerr[n] = {0};
    double yerr[n];
    for (int i = 0; i < n; ++i)
        yerr[i] = 0.05;  // assumed uncertainty in GeV

    // Create graph with errors
    TGraphErrors *gr = new TGraphErrors(n, pressure, threshold, xerr, yerr);
    gr->SetTitle("Muon Cherenkov Threshold in Pressurized Nitrogen;Pressure (atm);Cherenkov Threshold Energy (GeV)");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.2);
    gr->SetLineWidth(2);
    gr->SetLineColor(kBlue + 1);
    gr->SetMarkerColor(kRed + 1);

    // Create canvas
    TCanvas *c1 = new TCanvas("c1", "Cherenkov Threshold vs Pressure", 800, 600);
    gr->Draw("AP");

    // Physically motivated fit function:
    // E_th(P) = m / sqrt(1 - 1 / (1 + kP)^2)
    TF1 *fitFunc = new TF1("fitFunc", "[0]/sqrt(1 - 1.0/pow(1 + [1]*x, 2))", 2, 16);
    fitFunc->SetParName(0, "MuonMass");
    fitFunc->SetParName(1, "k");

    fitFunc->SetParameters(0.1057, 0.0003);  // Initial guesses
    fitFunc->FixParameter(0, 0.1057);        // Fix muon mass (GeV)

    fitFunc->SetLineColor(kGreen + 2);
    fitFunc->SetLineStyle(2);
    fitFunc->SetLineWidth(2);

    gr->Fit(fitFunc, "R");

    // Add legend
    TLegend *leg = new TLegend(0.52, 0.75, 0.88, 0.88);
    leg->AddEntry(gr, "Simulation data", "lep");
    leg->AddEntry(fitFunc, "Fit: m / sqrt(1 - 1/(1 + kP)^{2})", "l");
    leg->Draw();

    // Grid and save
    c1->SetGrid();
    c1->SaveAs("CherenkovThreshold_vs_Pressure.pdf");
}

