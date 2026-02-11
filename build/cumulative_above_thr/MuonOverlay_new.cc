void OverlayMuonPlots() {
    gStyle->SetOptStat(0); // Disable stat boxes
    const int rebinFactor = 50;
    const int nFiles = 15;

    // Only the two histograms we want
    std::vector<TString> histNames = {
        "MuonEnergyforAll_4-fold",
        "MuonEnergyWithDetectedCherenkov_4-fold_4PE"
    };

    // Fixed colors for consistency
    std::vector<int> colors = {kRed, kBlue};

    std::vector<TH1D*> storedHists;
    TLegend *legAll = new TLegend(0.65, 0.60, 0.88, 0.88);
    legAll->SetBorderSize(0);
    legAll->SetFillStyle(0);
    legAll->SetTextSize(0.025);

    // Loop over all cumulative_i.root files (i = 2..16)
    for (int i = 2; i <= 16; ++i) {
        TString fileName = Form("cumulative_%d.root", i);
        TFile *f = TFile::Open(fileName);
        if (!f || f->IsZombie()) {
            std::cout << "⚠️ Cannot open " << fileName << std::endl;
            continue;
        }

        TCanvas *c = new TCanvas(Form("cumulative_%d_canvas", i),
                                 Form("Overlay cumulative_%d", i), 1200, 900);

        TLegend *leg = new TLegend(0.65, 0.65, 0.88, 0.88);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);

        TH1D *firstHist = nullptr;

        // Loop over the two hists
        for (size_t j = 0; j < histNames.size(); ++j) {
            TString name = histNames[j];
            TH1D *h = (TH1D*)f->Get(name);
            if (!h) continue;

            TH1D *hClone = (TH1D*)h->Clone(Form("%s_clone_%d", name.Data(), i));
            hClone->SetDirectory(0);
            hClone->Rebin(rebinFactor);
            hClone->SetLineColor(colors[j]);
            hClone->SetLineWidth(3);

            // Draw histograms
            if (!firstHist) {
                hClone->GetXaxis()->SetTitle("Muon Energy [GeV]");
                hClone->GetYaxis()->SetTitle("Events");
                hClone->SetTitle(Form("Muon Energy Overlay at %d atm", i));
                hClone->Draw("HIST");
                firstHist = hClone;
            } else {
                hClone->Draw("HIST SAME");
            }

            leg->AddEntry(hClone, name, "l");
            storedHists.push_back(hClone);
        }

        leg->Draw();
        c->SaveAs(Form("Overlay_cumulative_%d.png", i));
        f->Close();
    }

    // GRAND OVERLAY
    TCanvas *grand = new TCanvas("GrandOverlay_AllFiles", "Grand Overlay", 1400, 1000);
    bool firstDrawn = false;
    int colorIndex = 0;

    for (auto &h : storedHists) {
        h->SetLineColor(colors[colorIndex % colors.size()]);
        colorIndex++;
        if (!firstDrawn) {
            h->GetXaxis()->SetTitle("Muon Energy [GeV]");
            h->GetYaxis()->SetTitle("Events");
            h->SetTitle("Grand Overlay: All 15 files + MuonKE");
            h->GetYaxis()->SetRangeUser(0, 1500);
            h->Draw("HIST");
            firstDrawn = true;
        } else {
            h->Draw("HIST SAME");
        }
    }

    // Add MuonKE from MuonKE_extended.root
    TFile *fMuonKE = TFile::Open("MuonKE_extended.root");
    if (fMuonKE && !fMuonKE->IsZombie()) {
        TH1D *hMuonKE = (TH1D*)fMuonKE->Get("MuonKE");
        if (hMuonKE) {
            TH1D *hClone = (TH1D*)hMuonKE->Clone("MuonKE_clone");
            hClone->SetDirectory(0);
            hClone->Rebin(rebinFactor);
            hClone->SetLineColor(kBlack);
            hClone->SetLineWidth(3);
            hClone->Draw("HIST SAME");
            legAll->AddEntry(hClone, "MuonKE (extended)", "l");
            storedHists.push_back(hClone);
        }
        fMuonKE->Close();
    }

    legAll->Draw();
    grand->SaveAs("GrandOverlay_AllFiles.png");
}

