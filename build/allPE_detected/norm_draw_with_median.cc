void OverlayOriginalVsNormalized() {
    gStyle->SetOptStat(0);

    // -----------------------------
    // Open ROOT file
    // -----------------------------
    TFile *f = TFile::Open("allPE_14.root");
    if (!f || f->IsZombie()) {
        cout << "Error: cannot open file allPE_14.root!" << endl;
        return;
    }

    // -----------------------------
    // Get histogram
    // -----------------------------
    TH1F *h = (TH1F*)f->Get("MuonEnergyWithDetectedCherenkov_4-fold_4PE");
    if (!h) {
        cout << "Error: histogram MuonEnergyWithDetectedCherenkov_4-fold_4PE not found!" << endl;
        return;
    }

    // -----------------------------
    // Clone and normalize
    // -----------------------------
    TH1F *h_norm = (TH1F*)h->Clone("h_norm");
    if (h_norm->Integral() != 0) {
        h_norm->Scale(1.0 / h_norm->Integral());
    }

    // -----------------------------
    // Style setup
    // -----------------------------
    h->SetLineColor(kBlue);
    h->SetLineWidth(2);

    h_norm->SetLineColor(kRed);
    h_norm->SetLineStyle(2);
    h_norm->SetLineWidth(2);

    // -----------------------------
    // Draw
    // -----------------------------
    TCanvas *c = new TCanvas("c", "Original vs Normalized", 800, 600);

    // Make sure axes are scaled to show both
    double maxVal = std::max(h->GetMaximum(), h_norm->GetMaximum());
    h->SetMaximum(1.2 * maxVal);

    h->Draw("HIST");
    h_norm->Draw("HIST SAME");

    // -----------------------------
    // Legend
    // -----------------------------
    TLegend *leg = new TLegend(0.55, 0.75, 0.88, 0.88);
    leg->AddEntry(h, "Original", "l");
    leg->AddEntry(h_norm, "Normalized", "l");
    leg->Draw();
}

