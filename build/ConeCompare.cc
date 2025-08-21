void overlay_histos() {
    gStyle->SetOptStat(0);

    for (int n = 2; n <= 16; n += 2) {
        TString fname1 = Form("output_withCone_%d.root", n);
        TString fname2 = Form("output_%d.root", n);

        TFile *f1 = TFile::Open(fname1);
        TFile *f2 = TFile::Open(fname2);

        // List of histograms to compare
        std::vector<TString> histNames = {"hPMTPhotons", "hDetectedPhotons"};

        for (auto hname : histNames) {
            TH1 *h1 = (TH1*) f1->Get(hname);
            TH1 *h2 = (TH1*) f2->Get(hname);

            if (!h1 || !h2) {
                std::cout << "Missing histogram " << hname << " in " << fname1 << " or " << fname2 << std::endl;
                continue;
            }

            // Restrict h2 to 0–1000 (make a clone with h1 binning)
            TH1 *h2_common = (TH1*) h1->Clone("h2_common");
            h2_common->Reset();

            for (int i=1; i<=h1->GetNbinsX(); i++) {
                double xc = h1->GetXaxis()->GetBinCenter(i);
                if (xc > 1000) continue;
                int bin2 = h2->FindBin(xc);
                h2_common->SetBinContent(i, h2->GetBinContent(bin2));
                h2_common->SetBinError(i,   h2->GetBinError(bin2));
            }

            // Style
            h1->SetLineColor(kRed);
            h1->SetLineWidth(2);
            h2_common->SetLineColor(kBlue);
            h2_common->SetLineWidth(2);

            // Draw
            TCanvas *c = new TCanvas(Form("c_%s_%d", hname.Data(), n),
                                     Form("%s comparison n=%d", hname.Data(), n), 800, 600);
            h1->Draw("HIST");
            h2_common->Draw("HIST SAME");

            TLegend *leg = new TLegend(0.65,0.7,0.9,0.9);
            leg->AddEntry(h1, Form("%s withCone_%d", hname.Data(), n), "l");
            leg->AddEntry(h2_common, Form("%s noCone_%d", hname.Data(), n), "l");
            leg->Draw();

            // Save
            c->SaveAs(Form("mnt/d/Cherenkov_plots/ConeCompare/%s_compare_%d.png", hname.Data(), n));
        }

        f1->Close();
        f2->Close();
    }
}

