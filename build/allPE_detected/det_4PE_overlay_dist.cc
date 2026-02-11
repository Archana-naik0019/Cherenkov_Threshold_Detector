#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <vector>

void Overlay_DetectedPhotons() {

    gStyle->SetOptStat(0);

    const int iMin = 2;
    const int iMax = 16;

    std::vector<int> colors = {
        kRed, kBlue, kGreen+2, kMagenta, kCyan+2,
        kOrange+7, kViolet, kSpring+5, kTeal+2,
        kPink+7, kAzure+2, kYellow+2,
        kGray+2, kRed+2, kBlue+2
    };

    TCanvas *c1 = new TCanvas("c1",
        "Detected Photons (4-fold)", 900, 700);
    c1->SetGrid();

    TLegend *leg = new TLegend(0.62, 0.60, 0.88, 0.88);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.035);

    bool first = true;
    int colorIndex = 0;
    int step = 4;

    for (int i = iMin; i <= iMax; i += step) {

        TString fname = Form("allPE_%d.root", i);
        TFile *f = TFile::Open(fname);

        if (!f || f->IsZombie()) {
            std::cerr << "⚠️  Could not open " << fname << std::endl;
            continue;
        }

        TH1 *hTmp = (TH1*)f->Get("hDetectedPhotons_4-fold");
        if (!hTmp) {
            std::cerr << "⚠️  Histogram not found in " << fname << std::endl;
            f->Close();
            continue;
        }

        // 🔑 Clone and detach from file
        TH1 *h = (TH1*)hTmp->Clone(
            Form("hDetected_%d", i));
        h->SetDirectory(nullptr);
        
        // =====================================================
// Percentage of events above 4 PE (FULL RANGE)
// =====================================================

// Use ORIGINAL histogram for physics calculation
int bin4 = hTmp->GetXaxis()->FindBin(4.0);

// Total entries including true ROOT overflow
double total = hTmp->Integral(1, hTmp->GetNbinsX())
             + hTmp->GetBinContent(hTmp->GetNbinsX() + 1);

// Entries strictly above 4 PE (including overflow)
double above4 = hTmp->Integral(bin4 + 1, hTmp->GetNbinsX())
              + hTmp->GetBinContent(hTmp->GetNbinsX() + 1);

double percent_above4 = (total > 0.0)
    ? 100.0 * above4 / total
    : 0.0;

// Terminal print
std::cout << "Pressure " << i << " atm : "
          << above4 << " / " << total
          << "  = " << percent_above4
          << " % events above 4 PE"
          << std::endl;

        
       // --- Manual overflow above 60 PE ---
int bin60 = h->GetXaxis()->FindBin(60.0);

double overflow = 0.0;
double overflowErr2 = 0.0;

for (int b = bin60 + 1; b <= h->GetNbinsX(); ++b) {
    overflow     += h->GetBinContent(b);
    overflowErr2 += std::pow(h->GetBinError(b), 2);
}

// Add to bin at 60
h->AddBinContent(bin60, overflow);
h->SetBinError(bin60,
    std::sqrt(std::pow(h->GetBinError(bin60), 2) + overflowErr2));

// Zero bins above 60 (optional, but clean)
for (int b = bin60 + 1; b <= h->GetNbinsX(); ++b) {
    h->SetBinContent(b, 0.0);
    h->SetBinError(b, 0.0);
}


        

        h->SetLineColor(colors[colorIndex % colors.size()]);
        h->SetLineWidth(2);

        h->GetXaxis()->SetTitle("Number of detected photons");
        h->GetYaxis()->SetTitle("Events");

        if (first) {
        
            h->GetXaxis()->SetRangeUser(0.0, 60.0);
            h->GetYaxis()->SetRangeUser(0.0, 50.0);
        
            h->Draw("hist");
            first = false;
        } else {
            h->Draw("hist same");
        }

        leg->AddEntry(h,
            Form("%d atm (OF = %.0f)", i, overflow), "l");

        colorIndex++;
        f->Close();
    }

    // --- Vertical line at x = 4 ---
gPad->Update();   // IMPORTANT

double yMax = gPad->GetFrame()->GetY2();

TLine *line = new TLine(4.0, 0.0, 4.0, yMax);
line->SetLineStyle(2);
line->SetLineWidth(2);
line->SetLineColor(kBlack);
line->Draw("same");

leg->AddEntry(line, "4 photon threshold", "l");
leg->SetTextSize(0.025);
leg->Draw();
}

