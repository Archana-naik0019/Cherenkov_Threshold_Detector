#include <TFile.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>

void print_mean_median() {
    const int nFiles = 15;
    double pressure[nFiles];
    double mean[nFiles], median[nFiles], rms[nFiles];
    double meanErr[nFiles], medianErr[nFiles];

    for (int idx = 0; idx < nFiles; idx++) {
        int i = idx + 2;  // pressure index (2 to 16 atm)
        TString filename = Form("allPE_%d.root", i);

        TFile *f = TFile::Open(filename);
        if (!f || f->IsZombie()) {
            std::cout << "Could not open " << filename << std::endl;
            continue;
        }

        TH1 *h = (TH1*)f->Get("MuonEnergyWithDetectedCherenkov_4-fold_4PE");
        if (!h) {
            std::cout << "Histogram not found in " << filename << std::endl;
            f->Close();
            continue;
        }
	
	double N = h->GetEntries();
	
        // Mean and RMS
        mean[idx] = h->GetMean();
        meanErr[idx] = h->GetMeanError();
        rms[idx] = h->GetRMS();

        // Median
        double q[1] = {0.5};
        h->GetQuantiles(1, &median[idx], q);
        
        int binMed = h->FindBin(median[idx]);
        double binWidth = h->GetXaxis()->GetBinWidth(binMed);
        double fMed = h->GetBinContent(binMed) / (N * binWidth);
        medianErr[idx] = (fMed > 0) ? 1.0 / (2.0 * fMed * sqrt(N)) : 0.0;

        // Approximate error on median using binomial assumption
        // N = total entries, p ~ 0.5 for median
        //double N = h->GetEntries();
        medianErr[idx] = 0.5 / sqrt(N);

        pressure[idx] = i; // pressure value = index

        std::cout << filename << std::endl;
        std::cout << "  Mean   = " << mean[idx] 
                  << " ± " << meanErr[idx] << std::endl;
        std::cout << "  Median = " << median[idx] 
                  << " ± " << medianErr[idx] << std::endl;
        std::cout << "  RMS    = " << rms[idx] << std::endl;

        f->Close();
    }

    // Plotting
    TCanvas *c = new TCanvas("c", "Mean and Median vs Pressure", 800, 600);
    c->SetGrid();

    TGraphErrors *gMean = new TGraphErrors(nFiles, pressure, mean, 0, meanErr);
    gMean->SetMarkerStyle(20);
    gMean->SetMarkerColor(kBlue);
    gMean->SetLineColor(kBlue);
    gMean->SetTitle("Mean & Median vs Pressure;Pressure (atm);Value");

    TGraphErrors *gMedian = new TGraphErrors(nFiles, pressure, median, 0, medianErr);
    gMedian->SetMarkerStyle(21);
    gMedian->SetMarkerColor(kRed);
    gMedian->SetLineColor(kRed);

    gMean->Draw("APL");
    gMean->GetXaxis()->SetLimits(0, 17);    // x from 0 to 17 atm
    gMean->SetMinimum(0);                   // y starts at 0

    gMedian->Draw("PL SAME");

    TLegend *leg = new TLegend(0.6,0.7,0.85,0.85);
    leg->AddEntry(gMean, "Mean", "lp");
    leg->AddEntry(gMedian, "Median", "lp");
    leg->Draw();

    c->SaveAs("mean_median_vs_pressure.png");
}

