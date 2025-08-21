void Overlay_Gen_PMT_Det() {
    // Open the ROOT file
    TFile* file = new TFile("output_16.root");

    // Retrieve the histograms
    //TH1F* h1 = (TH1F*)file->Get("hGeneratedWavelength");
    //TH1F* h1 = (TH1F*)file->Get("hAtPMTWavelength");
    TH1F* h1 = (TH1F*)file->Get("hDetectedWavelength");

    // Check if all histograms exist
    //if (!h1 || !h2 || !h3) {
    if (!h1) {
        std::cerr << "Error: One or more histograms not found!" << std::endl;
        return;
    }

    // Create a canvas
    TCanvas* c1 = new TCanvas("c1", "Overlay Line Histograms", 800, 600);

    // Set line styles instead of bar/filled
    h1->SetLineColor(kGreen+2);
    h1->SetLineWidth(2);
    h1->SetTitle("Wavelength distribution of Detected Photons(16atm);Wavelength (nm);# Detected Photons");
    h1->Draw("HIST");

    //h2->SetLineColor(kGreen+2);
    //h2->SetLineWidth(2);
    //h2->Draw("HIST SAME");

    //h3->SetLineColor(kGreen+2);
    //h3->SetLineWidth(2);
    //h3->Draw("HIST SAME");

    // Add legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.88, 0.88);
    //legend->AddEntry(h1, "Generated", "l");
    //legend->AddEntry(h1, "Reached PMT", "l");
    legend->AddEntry(h1, "Detected", "l");
    legend->Draw();

    c1->Update();
}

