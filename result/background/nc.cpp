void nc(){
   
    TFile *file = new TFile("CrystalEyeMC_AG1.root");  // Замените на имя вашего файла
    TTree *tree = (TTree*)file->Get("PrimaryParameter");  // Замените на имя вашего дерева

    // Предположим, что у вас есть две ветви: branch1 и branch2
    Int_t branch1_value;
    Int_t branch2_value;
    Int_t branch3_value;
    int nbins = 1024;
    double xlow = -200;
    double xup = 200;

    tree->SetBranchAddress("Int_x", &branch1_value);  // Замените "branch1" на имя вашей первой ветви
    tree->SetBranchAddress("Int_y", &branch2_value);  // Замените "branch2" на имя вашей второй ветви
    tree->SetBranchAddress("Int_z", &branch3_value);
    
    TH1F *h1 = new TH1F("h1", "Histogram from branch1", nbins, xlow, xup);  // Укажите правильные параметры для вашей гистограммы
    TH1F *h2 = new TH1F("h2", "Histogram from branch2", nbins, xlow, xup);  // Укажите правильные параметры для вашей гистограммы

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i=0; i<nentries; i++) {
        tree->GetEntry(i);
        if (branch1_value > -800) h1->Fill(branch1_value);
        if (branch2_value > -800) h2->Fill(branch2_value);
    }
    
    TH2F *h2d = new TH2F("h2d", "2D Histogram;X axis;Y axis",
                         100, -200, 200,
                         100, -200, 200);

    Long64_t nentries1 = tree->GetEntries();
    for (Long64_t i=0; i<nentries1; i++) {
        tree->GetEntry(i);
       // if (branch1_value > -800 && branch2_value > -800 && branch3_value > 0) {
        if (branch1_value > -800 && branch2_value > -800) {
            h2d->Fill(branch1_value, branch2_value);
        }
    }

    
    TFile *outfile = new TFile("output_AG1changebin.root", "RECREATE");  // Имя "output.root" можно заменить на желаемое
    h1->Write();
    h2->Write();
    h2d->Write();
    outfile->Close();

    file->Close();
    // Затем, используя код из предыдущего сообщения, вы можете комбинировать h1 и h2 в h2d

}
