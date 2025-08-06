void plotTotalWavelength() {
    std::ifstream infile("secondary10_info.txt");
    std::string line;

    // Create single combined histogram
    TH1D* hTotal = new TH1D("hTotal", "Total Wavelength Distribution;Wavelength (nm);# Photons", 150, 100, 950);

    while (std::getline(infile, line)) {
        if (line.find("λ:") != std::string::npos) {
            size_t pos = line.find("λ:");
            double lambda_nm = std::stod(line.substr(pos + 3)); // extract value after "λ:"
            hTotal->Fill(lambda_nm);
        }
    }

    // Draw on canvas
    TCanvas* c1 = new TCanvas("c1", "Total Wavelength Distribution", 1200, 800);
    hTotal->SetLineColor(kBlue);
    hTotal->SetLineWidth(2);
    hTotal->Draw("HIST");

    c1->SaveAs("TotalWavelength.png");
}
