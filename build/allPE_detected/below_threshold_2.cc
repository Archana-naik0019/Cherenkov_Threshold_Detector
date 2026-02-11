void ratio_all_files_different_cuts_ref() {
    // Example: cuts for files allPE_2.root ... allPE_16.root
    double cuts[15] = {3.115, 2.536, 2.176, 1.935, 1.786, 1.625, 1.515, 1.425, 1.336, 1.285, 1.215, 1.167, 1.126, 1.075, 1.045};

    // Reference file + histogram (used for denominator)
    TFile *fref = TFile::Open("EnergyDistOfMuonsShot.root");
    if (!fref || fref->IsZombie()) {
        std::cout << "Could not open reference file" << std::endl;
        return;
    }
    TH1 *href = (TH1*)fref->Get("MuonKE");
    if (!href) {
        std::cout << "Reference histogram not found!" << std::endl;
        fref->Close();
        return;
    }

    for (int idx = 0; idx < 15; idx++) {
        int i = idx + 2;  // file index goes from 2 to 16
        TString filename = Form("allPE_%d.root", i);

        TFile *f = TFile::Open(filename);
        if (!f || f->IsZombie()) {
            std::cout << "Could not open " << filename << std::endl;
            continue;
        }

        TH1 *h1 = (TH1*)f->Get("primPhotonVsEnergy");
        if (!h1) {
            std::cout << "Histogram H1 not found in " << filename << std::endl;
            f->Close();
            continue;
        }

        double Xcut = cuts[idx];  // pick cut for this file

        int bin_cut_num = h1->GetXaxis()->FindBin(Xcut);
        int bin_cut_den = href->GetXaxis()->FindBin(Xcut);

        double entries_num = h1->Integral(1, bin_cut_num-1);   // numerator
        double entries_den = href->Integral(1, bin_cut_den-1); // denominator

        double ratio = (entries_den > 0) ? (entries_num / entries_den) : 0.0;

        std::cout << filename 
                  << " (cut=" << Xcut << ") : ratio = " 
                  << ratio << std::endl;

        f->Close();
    }

    fref->Close();
}

