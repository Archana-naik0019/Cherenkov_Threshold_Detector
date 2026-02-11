#include <TFile.h>
#include <TH1.h>
#include <iostream>

void Print_PMT_ZeroPhotonBin() {

    const int iMin = 2;
    const int iMax = 16;
    const int step = 1;   // change to 1 or 2 if needed

    std::cout << "Entries at x = 0 bin of hPMTPhotons_4-fold\n";
    std::cout << "-----------------------------------------\n";

    for (int i = iMin; i <= iMax; i += step) {

        TString fname = Form("MR95_%datm_T_jgs2.root", i);
        TFile *f = TFile::Open(fname);

        if (!f || f->IsZombie()) {
            std::cerr << "⚠️  Could not open " << fname << std::endl;
            continue;
        }

        TH1 *hPMT = (TH1*)f->Get("hPMTPhotons_4-fold");
        if (!hPMT) {
            std::cerr << "⚠️  hPMTPhotons_4-fold not found in "
                      << fname << std::endl;
            f->Close();
            continue;
        }

        // Bin corresponding to x = 0 photons
        int binZero = hPMT->GetXaxis()->FindBin(0.0);
        double entriesAtZero = hPMT->GetBinContent(binZero);

        std::cout << "Pressure " << i << " atm : "
                  << "Entries at 0 photons = "
                  << entriesAtZero
                  << std::endl;

        f->Close();
    }
}

