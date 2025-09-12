//void overlay_histos(double xmin=0, double xmax=500) {
    // Open both ROOT files
    //TFile *f1 = TFile::Open("output_2.root");
    //TFile *f2 = TFile::Open("output_withCone_2.root");

void overlay_histos(double xmin=0, double xmax=250, double det_xmin=0, double det_xmax=60) {

TFile *outfile = new TFile("comparison_plots.root", "RECREATE");

    for (int i = 2; i <= 16; i += 1) {   // loop from 2 to 16 in steps of 2
        TString file1 = Form("output_%d.root", i);
        TString file2 = Form("output_withCone_%d.root", i);

        // Open both ROOT files
        TFile *f1 = TFile::Open(file1);
        TFile *f2 = TFile::Open(file2);

        if (!f1 || !f2) {
            printf("Could not open files for index %d\n", i);
            continue;
        }

    // Get histograms
    TH1F *h1a = (TH1F*) f1->Get("hPMTPhotons");
    TH1F *h2a = (TH1F*) f2->Get("hPMTPhotons");
    TH1F *h1b = (TH1F*) f1->Get("hDetectedPhotons");
    TH1F *h2b = (TH1F*) f2->Get("hDetectedPhotons");
    
    if (!h1a || !h2a || !h1b || !h2b) {
            printf("Missing histograms for index %d\n", i);
            continue;
        }

    // Canvas
    TCanvas *c = new TCanvas(Form("c_%d", i), Form("Comparison %d atm", i), 1200, 600);

    //TCanvas *c = new TCanvas("c","Comparison_%d",1200,600);
    c->Divide(2,1);

    // First panel
    c->cd(1);
    
    gPad->SetLeftMargin(0.15);   // extra space for Y-axis title
    gPad->SetBottomMargin(0.15); // extra space for X-axis title
    gPad->SetRightMargin(0.05);  // avoid cutting legend

    
    h1a->SetLineColor(kBlue);
    h2a->SetLineColor(kRed);
    h1a->GetXaxis()->SetRangeUser(xmin,xmax);  // set range before drawing
    h2a->GetXaxis()->SetRangeUser(xmin,xmax);
    h1a->GetXaxis()->SetTitle("Number of photons reaching PMT");
    h1a->GetYaxis()->SetTitle("Number of Events");
    
    // Adjust font size to avoid overlap
    h1a->GetXaxis()->SetTitleSize(0.04);
    h1a->GetYaxis()->SetTitleSize(0.04);
    h1a->GetXaxis()->SetLabelSize(0.018);
    h1a->GetYaxis()->SetLabelSize(0.018);
    
    h1a->Draw("HIST");
    
    gPad->Update();  // build stat box for h1a
TPaveStats *st1a = (TPaveStats*)h1a->FindObject("stats");
if(st1a){
    st1a->SetTextColor(kBlue);
    st1a->SetLineColor(kBlue);
    st1a->SetX1NDC(0.15); st1a->SetX2NDC(0.35);
    st1a->SetY1NDC(0.75); st1a->SetY2NDC(0.90);
}
    
    h2a->Draw("HIST SAMES");
    
    gPad->Update();  // build stat box for h2a
TPaveStats *st2a = (TPaveStats*)h2a->FindObject("stats");
if(st2a){
    st2a = (TPaveStats*)st2a->Clone();
    st2a->SetTextColor(kRed);
    st2a->SetLineColor(kRed);
    st2a->SetX1NDC(0.15); st2a->SetX2NDC(0.35);
    st2a->SetY1NDC(0.55); st2a->SetY2NDC(0.70);
    gPad->GetListOfPrimitives()->Add(st2a);
}
    
    //gPad->BuildLegend();
    // Custom legend
TLegend *leg1 = new TLegend(0.65,0.75,0.88,0.88); // adjust position if needed
leg1->AddEntry(h1a, Form("No Cone (%d atm)", i), "l");
leg1->AddEntry(h2a, Form("With Cone (%d atm)", i), "l");
leg1->Draw();


/////////////////////////////////////////////////////////////////////
    // Second panel
    c->cd(2);
    
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.05);
    
    h1b->SetLineColor(kBlue);
    h2b->SetLineColor(kRed);
    h1b->GetXaxis()->SetRangeUser(det_xmin,det_xmax);
    h1b->GetXaxis()->SetRangeUser(det_xmin,det_xmax);
    h1b->GetXaxis()->SetTitle("Number of Detected photons");
    h1b->GetYaxis()->SetTitle("Number of Events");
    
    h1b->GetXaxis()->SetTitleSize(0.04);
    h1b->GetYaxis()->SetTitleSize(0.04);
    h1b->GetXaxis()->SetLabelSize(0.018);
    h1b->GetYaxis()->SetLabelSize(0.018);
    
    h1b->Draw("HIST");
    
    gPad->Update();  // build stat box for h1b
TPaveStats *st1b = (TPaveStats*)h1b->FindObject("stats");
if(st1b){
    st1b->SetTextColor(kBlue);
    st1b->SetLineColor(kBlue);
    st1b->SetX1NDC(0.15); st1b->SetX2NDC(0.35);
    st1b->SetY1NDC(0.75); st1b->SetY2NDC(0.90);
}
    
    h2b->Draw("HIST SAMES");
    
    gPad->Update();  // build stat box for h2b
TPaveStats *st2b = (TPaveStats*)h2b->FindObject("stats");
if(st2b){
    st2b = (TPaveStats*)st2b->Clone();
    st2b->SetTextColor(kRed);
    st2b->SetLineColor(kRed);
    st2b->SetX1NDC(0.15); st2b->SetX2NDC(0.35);
    st2b->SetY1NDC(0.55); st2b->SetY2NDC(0.70);
    gPad->GetListOfPrimitives()->Add(st2b);
}
    
    //gPad->BuildLegend();
// Custom legend
TLegend *leg2 = new TLegend(0.65,0.75,0.88,0.88);
leg2->AddEntry(h1b, Form("No Cone (%d atm)", i), "l");
leg2->AddEntry(h2b, Form("With Cone (%d atm)", i), "l");
leg2->Draw();


    c->Update();
   outfile->cd(); 
   c->Write(); 
}
outfile->Close();
}
