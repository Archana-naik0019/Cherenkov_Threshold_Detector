Double_t CountEntriesAboveX(TH1* hist, Double_t xmin) {
    if (!hist) return 0;
    
    TAxis* xaxis = hist->GetXaxis();
    Int_t first_bin_above = xaxis->FindBin(xmin);
    Int_t last_bin = xaxis->GetNbins() + 1; // includes overflow
    
    Double_t entries_above = 0;
    for (Int_t bin = first_bin_above; bin <= last_bin; bin++) {
        entries_above += hist->GetBinContent(bin);
    }
    
    return entries_above;
}

// Usage:
//auto* h = (TH1*) gDirectory->Get("MuonEnergyWithDetectedCherenkov_4-fold_4PE");
//Double_t entries_above = CountEntriesAboveX(h, 100.0); // Example: count above 100
//std::cout << "Entries above 100: " << entries_above << std::endl;
