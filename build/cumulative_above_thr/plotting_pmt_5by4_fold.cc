void plot_corrected_ratios() {
    
    // Arrays to store data
    const int nFiles = 15;  // Files 2-16
    double fileNumbers[nFiles];
    double ratios[nFiles];
    double poissonErrors[nFiles];
    double binomialErrors[nFiles];
    
    // File names corresponding to your output
    TString fileNames[] = {
        "CorrectedOutput_2.root",
        "CorrectedOutput_3.root",
        "CorrectedOutput_4.root",
        "CorrectedOutput_5.root",
        "CorrectedOutput_6.root",
        "CorrectedOutput_7.root",
        "CorrectedOutput_8.root",
        "CorrectedOutput_9.root",
        "CorrectedOutput_10.root",
        "CorrectedOutput_11.root",
        "CorrectedOutput_12.root",
        "CorrectedOutput_13.root",
        "CorrectedOutput_14.root",
        "CorrectedOutput_15.root",
        "CorrectedOutput_16.root"
    };
    
    // Read data from corrected files
    for (int i = 0; i < nFiles; i++) {
        int fileNum = i + 2;  // Files start from 2
        fileNumbers[i] = fileNum;
        
        TFile *f = TFile::Open(fileNames[i], "READ");
        if (!f || f->IsZombie()) {
            cout << "ERROR: Could not open " << fileNames[i] << endl;
            ratios[i] = 0;
            poissonErrors[i] = 0;
            binomialErrors[i] = 0;
            continue;
        }
        
        TH1D *hGen = (TH1D*) f->Get(Form("hGenerated_corrected_%d", fileNum));
        TH1D *hPMT = (TH1D*) f->Get(Form("hPMT_corrected_%d", fileNum));
        
        if (!hGen || !hPMT) {
            cout << "ERROR: Missing histograms in " << fileNames[i] << endl;
            ratios[i] = 0;
            poissonErrors[i] = 0;
            binomialErrors[i] = 0;
            continue;
        }
        
        double total_pmt = hPMT->Integral();
        double total_gen = hGen->Integral();
        
        // Calculate ratio
        if (total_gen > 0) {
            ratios[i] = total_pmt / total_gen;
            
            // Poisson error: sqrt(N)/N for each histogram
            double pmt_error = (total_pmt > 0) ? sqrt(total_pmt) / total_pmt : 0;
            double gen_error = (total_gen > 0) ? sqrt(total_gen) / total_gen : 0;
            
            // Error propagation for ratio R = PMT/Gen
            // dR/R = sqrt( (dPMT/PMT)^2 + (dGen/Gen)^2 )
            poissonErrors[i] = ratios[i] * sqrt(pow(pmt_error, 2) + pow(gen_error, 2));
            
            // Binomial error for efficiency: sqrt(ε(1-ε)/N)
            // Where ε = PMT/Gen, N = total_gen
            if (ratios[i] >= 0 && ratios[i] <= 1) {
                binomialErrors[i] = sqrt(ratios[i] * (1 - ratios[i]) / total_gen);
            } else {
                // If ratio > 1, binomial formula doesn't apply directly
                binomialErrors[i] = 0;
            }
            
            cout << "File " << fileNum << ": Ratio = " << ratios[i] 
                 << " ± " << poissonErrors[i] << " (Poisson)"
                 << " ± " << binomialErrors[i] << " (Binomial)" << endl;
                 
        } else {
            ratios[i] = 0;
            poissonErrors[i] = 0;
            binomialErrors[i] = 0;
        }
        
        f->Close();
    }
    
    // Create canvas
    TCanvas *c1 = new TCanvas("c1", "Corrected PMT/Generated Ratios", 800, 600);
    c1->SetGrid();
    
    // Create graph with Poisson errors
    TGraphErrors *gPoisson = new TGraphErrors(nFiles, fileNumbers, ratios, 0, poissonErrors);
    gPoisson->SetTitle("PMT/Generated Ratios;Pressure(atm);Ratio PMT/Generated");
    gPoisson->SetMarkerStyle(20);
    gPoisson->SetMarkerColor(kBlue);
    gPoisson->SetLineColor(kBlue);
    gPoisson->SetFillColor(kBlue-10);
    gPoisson->SetFillStyle(1001);
    
    // Create graph with Binomial errors
    TGraphErrors *gBinomial = new TGraphErrors(nFiles, fileNumbers, ratios, 0, binomialErrors);
    gBinomial->SetMarkerStyle(21);
    gBinomial->SetMarkerColor(kRed);
    gBinomial->SetLineColor(kRed);
    
    // Draw Poisson graph (with shaded error band)
    gPoisson->Draw("AP4");
    gPoisson->Draw("P same");  // Redraw points on top
    
    // Draw Binomial graph
    gBinomial->Draw("P same");
    
    // Add legend
    TLegend *leg = new TLegend(0.15, 0.7, 0.4, 0.85);
    leg->AddEntry(gPoisson, "Poisson Error", "lp");
    leg->AddEntry(gBinomial, "Binomial Error", "lp");
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->Draw();
    
    // Adjust y-axis range
    double ymin = 0;
    double ymax = 1.2 * TMath::MaxElement(nFiles, ratios);
    gPoisson->GetYaxis()->SetRangeUser(ymin, ymax);
    
    // Add horizontal line at ratio = 1 (if relevant)
    if (ymax > 1.0) {
        TLine *line1 = new TLine(fileNumbers[0], 1.0, fileNumbers[nFiles-1], 1.0);
        line1->SetLineColor(kGreen+2);
        line1->SetLineStyle(2);
        line1->Draw();
        
        // Add text for ratio = 1 line
        TLatex *tex = new TLatex(fileNumbers[0] + 0.2, 1.05, "Ratio = 1");
        tex->SetTextColor(kGreen+2);
        tex->SetTextSize(0.03);
        tex->Draw();
    }
    
    // Save plot
    c1->SaveAs("corrected_ratios.pdf");
    c1->SaveAs("corrected_ratios.png");
    
    // Create separate plots for comparison
    TCanvas *c2 = new TCanvas("c2", "Error Comparison", 1200, 400);
    c2->Divide(2,1);
    
    // Plot 1: Poisson errors
    c2->cd(1);
    gPad->SetGrid();
    gPoisson->Draw("APL");
    gPoisson->SetTitle("With Poisson Errors;File Number;Ratio PMT/Generated");
    
    // Plot 2: Binomial errors
    c2->cd(2);
    gPad->SetGrid();
    gBinomial->Draw("APL");
    gBinomial->SetTitle("With Binomial Errors;File Number;Ratio PMT/Generated");
    
    c2->SaveAs("error_comparison.pdf");
    
    // Print summary table
    cout << "\n=============================================" << endl;
    cout << "          SUMMARY OF CORRECTED RATIOS" << endl;
    cout << "=============================================" << endl;
    cout << setw(8) << "File" << setw(12) << "Ratio" 
         << setw(15) << "Poisson Err" << setw(15) << "Binomial Err" << endl;
    cout << "---------------------------------------------" << endl;
    
    for (int i = 0; i < nFiles; i++) {
        cout << setw(8) << fileNumbers[i] 
             << setw(12) << fixed << setprecision(4) << ratios[i]
             << setw(15) << poissonErrors[i]
             << setw(15) << binomialErrors[i] << endl;
    }
    cout << "=============================================" << endl;
    
    // Cleanup
    delete c1;
    delete c2;
}
