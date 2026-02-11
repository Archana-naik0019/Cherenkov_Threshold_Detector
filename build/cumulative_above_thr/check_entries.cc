void check_entries() {
    TFile* f = new TFile("cumulative_16.root");
    TIter iter(f->GetListOfKeys());  // Changed from 'next' to 'iter'
    TKey* key;
    
    while ((key = (TKey*)iter())) {  // Changed from 'next()' to 'iter()'
        TObject* obj = key->ReadObj();
        
        // Check if it's a histogram (1D, 2D, etc.)
        if (obj->InheritsFrom("TH1")) {
            TH1* h1 = (TH1*)obj;  // Use TH1 base class pointer
            cout << key->GetName() << ": " << h1->GetEntries() << " entries" << endl;
        }
        // Check if it's a tree
        else if (obj->InheritsFrom("TTree")) {
            TTree* t = (TTree*)obj;
            cout << key->GetName() << ": " << t->GetEntries() << " entries" << endl;
        }
    }
    f->Close();
}
