void PlotAndOverlay_SameFile() {
    gStyle->SetOptStat(0);

    // --- Open the ROOT file ---
    TFile *file = TFile::Open(" bel_thr_15.root");
    if (!file || file->IsZombie()) {
        cout << "Error opening  bel_thr_15.root" << endl;
        return;
    }

    // --- Get both histograms ---
    TH1 *h1 = (TH1*)file->Get("primPhotonfromMuonVsEnergy");
    TH1 *h2 = (TH1*)file->Get("PrimPhotons_4-fold");

    if (!h1 || !h2) {
        cout << "Error: one or both histograms not found in file!" << endl;
        return;
    }

    // --- Style ---
    h1->SetLineColor(kBlue);
    h1->SetLineWidth(2);
    h1->SetTitle("Muon energy (photon from a muon parent) without 4-fold");
    h1->GetXaxis()->SetTitle("Energy (GeV)");
    h1->GetYaxis()->SetTitle("Event counts");

    h2->SetLineColor(kRed);
    h2->SetLineWidth(2);
    
    TString customTitle = "Muon energy (photon from a muon parent) with 4-fold";
    TString xAxisTitle  = "Energy (GeV)";
    TString yAxisTitle  = "Event counts";

    h2->SetTitle(customTitle);
    h2->GetXaxis()->SetTitle(xAxisTitle);
    h2->GetYaxis()->SetTitle(yAxisTitle);

    // --- Canvas 1: Separate Plots ---
    TCanvas *c_sep = new TCanvas("c_sep", "Separate Histograms", 1200, 600);
    c_sep->Divide(2, 1);

    c_sep->cd(1);
    h1->Draw("HIST");
    gPad->SetGrid();

    c_sep->cd(2);
    h2->Draw("HIST");
    gPad->SetGrid();

    c_sep->SaveAs("Separate_Hists.png");

    // --- Canvas 2: Overlay Plot ---
    TCanvas *c_overlay = new TCanvas("c_overlay", "Overlay of with and without 4-fold trigger", 800, 600);
    h1->Draw("HIST");
    h2->Draw("HIST SAME");

    auto legend = new TLegend(0.6, 0.7, 0.88, 0.88);
    legend->AddEntry(h1, "without 4-fold trigger", "l");
    legend->AddEntry(h2, "with 4-fold trigger", "l");
    legend->Draw();

    c_overlay->SaveAs("Overlay_Hists.png");

    cout << "Plots saved: Separate_Hists.png and Overlay_Hists.png" << endl;
}

