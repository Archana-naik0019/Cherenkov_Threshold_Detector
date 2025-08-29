void Overlay_Gen_PMT_Det() {
    // Open the ROOT file
    
     for (int i = 2; i <= 16; i += 1) {   // loop from 2 to 16 in steps of 2
        TString fname = Form("output_4foldtest_%d.root", i);
        TFile* file = new TFile(fname, "READ");
    //TFile* file = new TFile(" output_4foldtest_15.root");

    // Retrieve the histograms
    TH1F* h1 = (TH1F*)file->Get("hGeneratedPhotons");
    TH1F* h2 = (TH1F*)file->Get("hPMTPhotons");
    TH1F* h3 = (TH1F*)file->Get("hDetectedPhotons");

    // Check if all histograms exist
    if (!h1 || !h2 || !h3) {
    //if (!h1) {
        std::cerr << "Error: One or more histograms not found!" << std::endl;
        return;
    }

    // Create a canvas
    //TCanvas* c1 = new TCanvas("c1", "Overlay Line Histograms", 800, 600);
    TCanvas* c1 = new TCanvas(Form("c%d", i), Form("Overlay %d atm", i), 800, 600);
    
     //c1->SetLogy();

    // Set line styles instead of bar/filled
    h1->SetLineColor(kBlue);
    h1->SetLineWidth(2);
    h1->SetMaximum(450);
    h1->SetTitle(Form("Photon Distribution(%d atm);Number of Photons;Number of Events",i));
    h1->Draw("HIST");

    h2->SetLineColor(kRed);
    h2->SetLineWidth(2);
    h2->SetMaximum(450);
    h2->Draw("HIST SAME");

    h3->SetLineColor(kGreen+2);
    h3->SetLineWidth(2);
    h3->SetMaximum(450);
    h3->Draw("HIST SAME");

    // Add legend
    TLegend* legend = new TLegend(0.7, 0.7, 0.88, 0.88);
    legend->AddEntry(h1, "Generated", "l");
    legend->AddEntry(h2, "Reached PMT", "l");
    legend->AddEntry(h3, "Detected", "l");
    legend->Draw();

    c1->Update();
}
}
