void Overlay_Gen_PMT_Det() {
    static std::vector<TCanvas*> canvases;

    gStyle->SetOptStat(1111);
    gStyle->SetStatX(0.88);
    gStyle->SetStatY(0.88);
    gStyle->SetStatW(0.2);
    gStyle->SetStatH(0.15);

    for (int i = 2; i <= 16; i++) {
        TString fname = Form("cumulative_%d.root", i);
        TFile* file = new TFile(fname, "READ");

        if (!file || file->IsZombie()) {
            std::cerr << "Error: Could not open " << fname << std::endl;
            continue;
        }

        TH1F* h1 = (TH1F*)file->Get("hGeneratedPhotons_4-fold");
        TH1F* h2 = (TH1F*)file->Get("hPMTPhotons_4-fold");
        TH1F* h3 = (TH1F*)file->Get("hDetectedPhotons_4-fold");
        
        // Rebin histograms (make coarser)
int rebinFactor = 5;  // change this as needed
h1->Rebin(rebinFactor);
h2->Rebin(rebinFactor);
h3->Rebin(rebinFactor);


        if (!h1 || !h2 || !h3) {
            std::cerr << "Error: Missing histogram(s) in " << fname << std::endl;
            file->Close();
            continue;
        }

        // ✅ Keep histograms alive after file close
        h1->SetDirectory(0);
        h2->SetDirectory(0);
        h3->SetDirectory(0);
        file->Close();

        // Create canvas
        TCanvas* c1 = new TCanvas(Form("c%d", i), Form("Overlay %d atm", i), 800, 600);
        canvases.push_back(c1);

        // Styling
        h1->SetLineColor(kBlue);
        h2->SetLineColor(kRed);
        h3->SetLineColor(kGreen + 2);
        h1->SetLineWidth(2);
        h2->SetLineWidth(2);
        h3->SetLineWidth(2);

        double maxVal = std::max({h1->GetMaximum(), h2->GetMaximum(), h3->GetMaximum()});
        h1->SetMaximum(1.2 * maxVal);
        h1->SetTitle(Form("Photon Distribution (%d atm);Number of Photons;Number of Events", i));

        // Draw and decorate
        h1->Draw("HIST");
        h2->Draw("HIST SAME");
        h3->Draw("HIST SAME");
        gPad->Update();

        TPaveStats* st1 = (TPaveStats*)h1->FindObject("stats");
        TPaveStats* st2 = (TPaveStats*)h2->FindObject("stats");
        TPaveStats* st3 = (TPaveStats*)h3->FindObject("stats");

        if (st1 && st2 && st3) {
            st1->SetTextColor(kBlue);
            st2->SetTextColor(kRed);
            st3->SetTextColor(kGreen + 2);
            st1->SetY1NDC(0.88); st1->SetY2NDC(0.73);
            st2->SetY1NDC(0.73); st2->SetY2NDC(0.58);
            st3->SetY1NDC(0.58); st3->SetY2NDC(0.43);
            st1->SetX1NDC(0.70); st1->SetX2NDC(0.88);
            st2->SetX1NDC(0.70); st2->SetX2NDC(0.88);
            st3->SetX1NDC(0.70); st3->SetX2NDC(0.88);
        }

        TLegend* legend = new TLegend(0.15, 0.75, 0.35, 0.88);
        legend->AddEntry(h1, "Generated", "l");
        legend->AddEntry(h2, "Reached PMT", "l");
        legend->AddEntry(h3, "Detected", "l");
        legend->Draw();

        c1->Modified();
        c1->Update();
    }
}

