#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TMath.h>
#include <TLegend.h>

void convert_energy_to_beta() {
    // Open file and get original histogram
    TFile *f = TFile::Open("all_gen_PE_9.root");  // change filename
    TH1D *hE = (TH1D*)f->Get("secPhotonVsEnergy");

    // Electron mass in GeV
    const double me = 0.000511;

    // New histogram in beta
    TH1D *hBeta = new TH1D("hBeta","Electron distribution in terms of #beta;#beta;Counts",
                           100,0,1);

    // Loop over energy histogram bins
    for (int i=1; i<=hE->GetNbinsX(); i++) {
        double KE = hE->GetBinCenter(i);    // kinetic energy (GeV)
        double counts = hE->GetBinContent(i);

        double Etot = KE + me;
        if (Etot > me) {
            double p = sqrt(Etot*Etot - me*me);
            double beta = p / Etot;
            hBeta->Fill(beta, counts);
        }
    }

    // Draw both for comparison
    TCanvas *c1 = new TCanvas("c1","Kinetic Energy vs Beta",800,600);
    c1->Divide(1,2);

    c1->cd(1);
    hE->GetXaxis()->SetTitle("Kinetic Energy (GeV)");
    hE->GetYaxis()->SetTitle("Counts");
    hE->Draw("E");  // with error bars
    gPad->SetGrid();

    c1->cd(2);
    hBeta->Draw("E");
    gPad->SetGrid();

    // Legend with formula
    TLegend *leg = new TLegend(0.15,0.75,0.55,0.9);
    leg->AddEntry(hBeta,"#beta = p/E_{tot},  p = #sqrt{E_{tot}^{2}-m_{e}^{2}}","l");
    leg->Draw();

    c1->SaveAs("Energy_vs_Beta.png");
}

