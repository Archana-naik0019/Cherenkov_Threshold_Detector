void Corrected_bin0_photDist_AllFiles() {

    for (int i = 2; i <= 16; i++) {

        cout << "\n======================================" << endl;
        cout << " Processing file: allPE_" << i << ".root" << endl;
        cout << "======================================\n";

        TString infile  = Form("allPE_%d.root", i);
        TString outfile = Form("CorrectedOutput_%d.root", i);

        TFile *f = TFile::Open(infile, "READ");

        if (!f || f->IsZombie()) {
            cout << "ERROR: Could not open " << infile << endl;
            continue;
        }

        TH1D *hGen = (TH1D*) f->Get("hGeneratedPhotons_4-fold");
        TH1D *hPMT = (TH1D*) f->Get("hPMTPhotons_4-fold");
        TH1D *hDet = (TH1D*) f->Get("hDetectedPhotons_4-fold");

        if(!hGen || !hPMT || !hDet){
            cout << "ERROR: Missing histogram(s) in " << infile << endl;
            continue;
        }

        // Clone histograms
        TH1D *hPMT_corr = (TH1D*) hPMT->Clone(Form("hPMT_corrected_%d", i));
        TH1D *hDet_corr = (TH1D*) hDet->Clone(Form("hDetected_corrected_%d", i));

        // ---------------------------
        // 1. Correct PMT using Generated
        // ---------------------------
        int gen_bin0 = hGen->GetBinContent(1);
        int pmt_bin0 = hPMT->GetBinContent(1);
        int extra_pmt = pmt_bin0 - gen_bin0;

        if (extra_pmt > 0) {
            hPMT_corr->SetBinContent(1, gen_bin0);
            cout << "PMT bin0 corrected: removed " << extra_pmt << " extra\n";
        } else {
            cout << "PMT OK: no excess\n";
        }

        // ---------------------------
        // 2. Correct Detected using corrected PMT
        // ---------------------------
        int pmt_corr_bin0 = hPMT_corr->GetBinContent(1);
        int det_bin0      = hDet->GetBinContent(1);
        int extra_det     = det_bin0 - pmt_corr_bin0;

        if (extra_det > 0) {
            hDet_corr->SetBinContent(1, pmt_corr_bin0);
            cout << "Detected bin0 corrected: removed " << extra_det << " extra\n";
        } else {
            cout << "Detected OK: no excess\n";
        }

        // Fix ROOT entries counter
        hPMT_corr->SetEntries(hPMT_corr->Integral());
        hDet_corr->SetEntries(hDet_corr->Integral());

        // Save output
        TFile *fout = new TFile(outfile, "RECREATE");
        hPMT_corr->Write();
        hDet_corr->Write();
        fout->Close();

        cout << "Saved corrected file: " << outfile << "\n";
    }

    cout << "\n=== All files processed successfully ===\n\n";
}

