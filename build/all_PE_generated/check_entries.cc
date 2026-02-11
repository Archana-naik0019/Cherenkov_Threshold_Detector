void check_entries() {
    TFile* f = TFile::Open("all_gen_PE_2.root");
    if (!f) {
        cout << "Error opening file!" << endl;
        return;
    }
    
    // Print file info
    cout << "File: " << f->GetName() << endl;
    cout << "================================================" << endl;
    
    // Get all keys
    TList* keys = f->GetListOfKeys();
    TIter myIter(keys);  // Unique variable name
    TKey* key;
    
    // Loop through all objects
    while ((key = (TKey*)myIter())) {
        TString name = key->GetName();
        TString className = key->GetClassName();
        
        // Read the object
        TObject* obj = f->Get(name);
        if (!obj) continue;
        
        // Handle different object types
        if (obj->InheritsFrom("TH1")) {
            TH1* hist = (TH1*)obj;
            cout << Form("Histogram: %-40s = %10.0f entries", name.Data(), hist->GetEntries()) << endl;
        }
        else if (obj->InheritsFrom("TTree")) {
            TTree* tree = (TTree*)obj;
            cout << Form("Tree:      %-40s = %10lld entries", name.Data(), tree->GetEntries()) << endl;
        }
        else {
            cout << Form("Other:     %-40s = %s", name.Data(), className.Data()) << endl;
        }
        
        // Clean up
        delete obj;
    }
    
    cout << "================================================" << endl;
    f->Close();
}
