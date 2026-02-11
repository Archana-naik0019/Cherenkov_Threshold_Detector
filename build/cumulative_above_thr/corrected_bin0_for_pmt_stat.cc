void Corrected_bin0_photDist_AllFiles() {

    for (int i = 2; i <= 16; i++) {

        cout << "\n======================================" << endl;
        cout << " Processing file: allPE_" << i << ".root" << endl;
        cout << "======================================\n";

        TString infile  = Form("cumulative_%d.root", i);
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

        // Store original values
        double original_gen_total = hGen->Integral();
        double original_pmt_total = hPMT->Integral();
        double original_gen_bin0 = hGen->GetBinContent(1);
        double original_pmt_bin0 = hPMT->GetBinContent(1);
        
        cout << "Original values:" << endl;
        cout << "  Generated: total = " << original_gen_total 
             << ", bin0 = " << original_gen_bin0 << endl;
        cout << "  PMT:       total = " << original_pmt_total 
             << ", bin0 = " << original_pmt_bin0 << endl;

        // Create corrected histograms
        TH1D *hGen_corr = (TH1D*) hGen->Clone(Form("hGenerated_corrected_%d", i));
        TH1D *hPMT_corr = (TH1D*) hPMT->Clone(Form("hPMT_corrected_%d", i));
        TH1D *hDet_corr = (TH1D*) hDet->Clone(Form("hDetected_corrected_%d", i));
        
        // ------------------------------------------------------------
        // NEW CORRECTION LOGIC:
        // 1. corr_hgen = (no. of entries in original hpmt - no. of entries in zeroth bin of hgen original)
        // 2. corr_hpmt = (its original entries no. - its zeroth bin entries)
        // ------------------------------------------------------------
        
        // Calculate the new total for hGen_corr
        double new_gen_total = original_pmt_total - original_gen_bin0;
        
        // Scale the entire hGen_corr histogram to have this new total
        // First, remove bin 0
        hGen_corr->SetBinContent(1, 0);
        
        // Get the current total (without bin 0)
        double gen_without_bin0 = hGen_corr->Integral();
        
        if (gen_without_bin0 > 0) {
            // Calculate scaling factor
            double scale_factor = new_gen_total / gen_without_bin0;
            
            // Scale all bins (except bin 0 which is already 0)
            for (int bin = 2; bin <= hGen_corr->GetNbinsX(); bin++) {
                double current_value = hGen_corr->GetBinContent(bin);
                hGen_corr->SetBinContent(bin, current_value * scale_factor);
            }
            
            cout << "\nGenerated histogram correction:" << endl;
            cout << "  Original total: " << original_gen_total << endl;
            cout << "  New target total: " << new_gen_total << " (PMT total - Gen bin0)" << endl;
            cout << "  Scaling factor applied: " << scale_factor << endl;
        } else {
            cout << "\nWARNING: Generated histogram has no entries outside bin 0!" << endl;
            cout << "  Cannot scale Generated histogram." << endl;
        }
        
        // For hPMT_corr: simply remove bin 0 entries
        double original_pmt_without_bin0 = original_pmt_total - original_pmt_bin0;
        hPMT_corr->SetBinContent(1, 0);
        
        cout << "\nPMT histogram correction:" << endl;
        cout << "  Original total: " << original_pmt_total << endl;
        cout << "  After removing bin0: " << original_pmt_without_bin0 << endl;
        
        // For hDet_corr: also just remove bin 0
        hDet_corr->SetBinContent(1, 0);
        
        // ------------------------------------------------------------
        // Calculate final totals and ratios
        // ------------------------------------------------------------
        double final_gen_total = hGen_corr->Integral();
        double final_pmt_total = hPMT_corr->Integral();
        
        cout << "\nFinal corrected totals:" << endl;
        cout << "  Generated: " << final_gen_total << endl;
        cout << "  PMT:       " << final_pmt_total << endl;
        
        if (final_gen_total > 0) {
            double final_ratio = final_pmt_total / final_gen_total;
            cout << "  Ratio PMT/Generated: " << final_ratio << endl;
            
            // Check if ratio is reasonable
            if (final_ratio > 1.0) {
                cout << "  WARNING: Ratio > 1! PMT has more entries than Generated." << endl;
            }
        } else {
            cout << "  WARNING: Generated total is 0, cannot calculate ratio" << endl;
        }
        
        // Fix entries counter
        hGen_corr->SetEntries(hGen_corr->Integral());
        hPMT_corr->SetEntries(hPMT_corr->Integral());
        hDet_corr->SetEntries(hDet_corr->Integral());

        // Save output
        TFile *fout = new TFile(outfile, "RECREATE");
        hGen_corr->Write();
        hPMT_corr->Write();
        hDet_corr->Write();
        fout->Close();

        cout << "Saved corrected file: " << outfile << "\n";
    }

    cout << "\n=== All files processed successfully ===\n\n";
}
