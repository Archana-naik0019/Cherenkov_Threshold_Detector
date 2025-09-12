void plotTotalWavelength() {
    std::ifstream infile("secondary10_info.txt");
    std::string line;

    TH1D* hTotal = new TH1D("hTotal", "Total Wavelength Distribution;Wavelength (nm);# Photons", 150, 100, 950);

    int undershootCount = 0;
    int overshootCount = 0;

    while (std::getline(infile, line)) {
        if (line.find("λ:") != std::string::npos) {
            size_t pos = line.find("λ:");
            double lambda_nm = std::stod(line.substr(pos + 3));

            if (lambda_nm < 100) undershootCount++;
            else if (lambda_nm > 950) overshootCount++;
            else hTotal->Fill(lambda_nm);
        }
    }

    TCanvas* c1 = new TCanvas("c1", "Total Wavelength Distribution", 1200, 800);

    gStyle->SetOptStat(1110);  // Show entries, mean, std dev
    hTotal->SetLineColor(kBlue);
    hTotal->SetLineWidth(2);
    hTotal->Draw("HIST");

    // Force stats box to be drawn so we can access it
    c1->Update();

    TPaveStats* stats = (TPaveStats*)hTotal->FindObject("stats");
    stats->SetX1NDC(0.7); // reposition if needed
    stats->SetX2NDC(0.9);
    stats->SetY1NDC(0.7);
    stats->SetY2NDC(0.9);

    // Add overshoot/undershoot info inside stats box
    TString newText = Form("Undershoot (<100nm) = %d\nOvershoot (>950nm) = %d", undershootCount, overshootCount);
    stats->AddText(newText);

    c1->Modified();  // Force update with new stats box content
    c1->SaveAs("TotalWavelength.png");
}

