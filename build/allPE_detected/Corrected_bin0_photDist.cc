void Corrected_bin0_photDist() {

    TFile *f = TFile::Open("allPE_12.root", "READ");

    // Read histograms
    TH1D *hGen = (TH1D*) f->Get("hGeneratedPhotons_4-fold");
    TH1D *hPMT = (TH1D*) f->Get("hPMTPhotons_4-fold");
    TH1D *hDet = (TH1D*) f->Get("hDetectedPhotons_4-fold");

    if(!hGen || !hPMT || !hDet){
        cout << "ERROR: One or more histograms not found!" << endl;
        return;
    }

    // Clone histograms (ROOT-safe)
    TH1D *hPMT_corr = (TH1D*) hPMT->Clone("hPMT_corrected");
    TH1D *hDet_corr = (TH1D*) hDet->Clone("hDetected_corrected");

    cout << "\n=== ORIGINAL ENTRIES ===" << endl;
    cout << "Generated:  " << hGen->GetEntries() << endl;
    cout << "PMT:        " << hPMT->GetEntries() << endl;
    cout << "Detected:   " << hDet->GetEntries() << endl;

    // ---------------------------
    // 1. Correct PMT using Generated
    // ---------------------------
    int gen_bin0 = hGen->GetBinContent(1);
    int pmt_bin0 = hPMT->GetBinContent(1);

    int extra_pmt = pmt_bin0 - gen_bin0;

    if (extra_pmt > 0) {
        cout << "\nPMT bin 0: removing " << extra_pmt << " extra events\n";
        hPMT_corr->SetBinContent(1, gen_bin0);
    } else {
        cout << "\nPMT has no excess in bin 0.\n";
    }

    // ---------------------------
    // 2. Correct Detected using corrected PMT
    // ---------------------------
    int pmt_corr_bin0 = hPMT_corr->GetBinContent(1);
    int det_bin0      = hDet->GetBinContent(1);

    int extra_det = det_bin0 - pmt_corr_bin0;

    if (extra_det > 0) {
        cout << "Detected bin 0: removing " << extra_det << " extra events\n";
        hDet_corr->SetBinContent(1, pmt_corr_bin0);
    } else {
        cout << "Detected has no excess in bin 0.\n";
    }

    // ------------------------------------------
    // IMPORTANT: ROOT FIX — Force correct entries
    // ------------------------------------------
    hPMT_corr->SetEntries(hPMT_corr->Integral());
    hDet_corr->SetEntries(hDet_corr->Integral());

    cout << "\n=== CORRECTED ENTRIES ===" << endl;
    cout << "PMT corrected:      " << hPMT_corr->GetEntries() << endl;
    cout << "Detected corrected: " << hDet_corr->GetEntries() << endl;

    // Save output
    TFile *fout = new TFile("CorrectedOutput.root", "RECREATE");
    hPMT_corr->Write();
    hDet_corr->Write();
    fout->Close();

    cout << "\nCorrected histograms saved in CorrectedOutput.root\n" << endl;
}

