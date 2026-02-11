#include <iostream>
#include <string>
#include "TFile.h"
#include "TH1.h"

void PhotonSummary() {
    // Loop through ROOT files cumulative_2.root → cumulative_16.root
    for (int i = 2; i <= 16; ++i) {
        std::string filename = "cumulative_" + std::to_string(i) + ".root";
        TFile *file = TFile::Open(filename.c_str(), "READ");

        if (!file || file->IsZombie()) {
            std::cout << "❌ Could not open file: " << filename << std::endl;
            continue;
        }

        std::cout << "\n===============================" << std::endl;
        std::cout << " File: " << filename << std::endl;
        std::cout << "===============================" << std::endl;

        // The three histograms of interest
        const char* histNames[] = {
            "hGeneratedPhotons_4-fold",
            "hPMTPhotons_4-fold",
            "hDetectedPhotons_4-fold"
        };

        // Process each histogram
        for (auto histName : histNames) {
            TH1* h = (TH1*)file->Get(histName);
            if (!h) {
                std::cout << "⚠️  Histogram not found: " << histName << std::endl;
                continue;
            }

            int nbins = h->GetNbinsX();
            long long totalPhotons = 0;
            long long totalEvents  = 0;

            // Loop over all bins including underflow(0) and overflow(nbins+1)
            for (int bin = 0; bin <= nbins + 1; ++bin) {
                double binContent = h->GetBinContent(bin);  // number of events in this bin
                double binCenter  = h->GetBinCenter(bin);   // photon count (since bin width = 1)

                totalPhotons += static_cast<long long>(binCenter * binContent);
                totalEvents  += static_cast<long long>(binContent);
            }

            std::cout << "→ " << histName << std::endl;
            std::cout << "   Total events (entries): " << totalEvents << std::endl;
            std::cout << "   Total photons detected : " << totalPhotons << std::endl;

            if (totalEvents > 0)
                std::cout << "   Avg photons per event  : " 
                          << static_cast<double>(totalPhotons) / totalEvents 
                          << std::endl;
            std::cout << std::endl;
        }

        file->Close();
        delete file;
    }

    std::cout << "\n✅ Photon summary (bin width = 1) completed.\n" << std::endl;
}

