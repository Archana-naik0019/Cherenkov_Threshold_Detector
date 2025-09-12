#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <vector>

void plotMuonEnergyOverlays() {
    // List of histogram names
    std::vector<TString> histNames = {
        "MuonEnergyWithGeneratedCherenkov_4-fold",
        "MuonEnergyWithDetectedCherenkov_4-fold_1PE",
        "MuonEnergyWithDetectedCherenkov_4-fold_2PE",
        "MuonEnergyWithDetectedCherenkov_4-fold_3PE",
        "MuonEnergyWithDetectedCherenkov_4-fold_4PE",
        "MuonEnergyWithDetectedCherenkov_4-fold_4.5PE"
    };

    std::vector<int> colors = {kBlack, kRed, kBlue, kGreen+2, kMagenta, kOrange+1};
    std::vector<int> styles = {1, 2, 2, 2, 2, 2}; // line styles

    // Loop over files all_gen_PE_2.root to all_gen_PE_16.root
    for (int i = 2; i <= 16; i++) {
        TString fileName = Form("all_gen_PE_%d.root", i);
        TFile *f = TFile::Open(fileName, "READ");
        if (!f || f->IsZombie()) {
            printf("Could not open %s\n", fileName.Data());
            continue;
        }

        // Create a canvas
        TCanvas *c = new TCanvas(Form("c%d", i), Form("Muon Energy - PE_%d", i), 800, 600);

        TLegend *leg = new TLegend(0.55, 0.65, 0.88, 0.88);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);

        bool first = true;
        for (size_t j = 0; j < histNames.size(); j++) {
            TH1 *h = (TH1*)f->Get(histNames[j]);
            if (!h) {
                printf("Histogram %s not found in %s\n", histNames[j].Data(), fileName.Data());
                continue;
            }

            h->SetLineColor(colors[j]);
            h->SetLineStyle(styles[j]);
            h->SetLineWidth(2);

            if (first) {
                h->SetTitle(Form("Muon Energy Distributions (PE_%d)", i));
                h->GetXaxis()->SetTitle("Muon Energy (GeV)");
                h->GetYaxis()->SetTitle("Counts");
                h->Draw("HIST");
                first = false;
            } else {
                h->Draw("HIST SAME");
            }
            leg->AddEntry(h, histNames[j], "l");
        }

        leg->Draw();
        c->Update();

        // Optional: save plots
        c->SaveAs(Form("MuonEnergyOverlay_PE%d.png", i));

        f->Close();
    }
}

