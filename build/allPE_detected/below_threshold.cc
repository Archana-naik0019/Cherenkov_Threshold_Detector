void ratio_all_files_different_cuts() {
    // Example: cuts for files allPE_2.root ... allPE_16.root
    // You can fill this array with your own cut values
    double cuts[15] = {3.115, 2.536, 2.176, 1.935, 1.786, 1.625, 1.515, 1.425, 1.336, 1.285, 1.215, 1.167, 1.126, 1.075, 1.045};

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
            std::cout << "Histogram not found in " << filename << std::endl;
            f->Close();
            continue;
        }

        double Xcut = cuts[idx];  // pick cut for this file
        int bin_cut = h1->GetXaxis()->FindBin(Xcut);

        double entries_below = h1->Integral(1, bin_cut-1);
        double total_entries  = h1->Integral(0, h1->GetNbinsX()+1);

        double ratio = (total_entries > 0) ? (entries_below / total_entries) : 0.0;

        std::cout << filename 
                  << " (cut=" << Xcut << ") : ratio = " 
                  << ratio << std::endl;

        f->Close();
    }
}

