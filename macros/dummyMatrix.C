{

  TFile *f = TFile::Open("PhotonJet_MC_TOT_PFlowAK5chs.root");
  gDirectory->cd("matrix");

  // Get the matrix and invert it
  TMatrixF m(4,4);
  TMatrixF minv(4,4);
  m = *((TMatrixF*)gDirectory->Get("matrix4x4_ptPhot_100_200"));

  cout << "============================================================" << endl;
  cout << " The flavour fraction matrix :" << endl;
  cout << "============================================================" << endl;
  
  m.Print();

  // Normalize the matrix
  
  for (int i=0; i<4; ++i) {

    // Compute the sum :
    Float_t sum = 0.;
    for (int j=0; j<4; ++j) 
      sum += m(i,j);
    cout << " Sum row " << i << " = " << sum << endl;

    // Normalize sum of each row to 1
    for (int j=0; j<4; ++j) 
      m(i,j) = m(i,j)/sum;

    // Check the sum:
    Float_t sum2 = 0.;
    for (int j=0; j<4; ++j) 
      sum2 += m(i,j);
    cout << " Sum2 row " << i << " = " << sum2 << endl;

  }

  cout << "============================================================" << endl;
  cout << " The inverted flavour matrix :" << endl;
  cout << "============================================================" << endl;
  
  minv = ((TMatrixF*)m->Clone())->Invert();
  minv.Print();

  cout << "============================================================" << endl;
  cout << " The response per zone" << endl;
  cout << "============================================================" << endl;

  f->cd();
  TH1F* hResp[4];
  hResp[0] = (TH1F*)f->Get("response_Zone_Pt/Rmpf/Rmpf_zone1Q_ptPhot_100_200");
  hResp[1] = (TH1F*)f->Get("response_Zone_Pt/Rmpf/Rmpf_zone2G_ptPhot_100_200");
  hResp[2] = (TH1F*)f->Get("response_Zone_Pt/Rmpf/Rmpf_zone3C_ptPhot_100_200");
  hResp[3] = (TH1F*)f->Get("response_Zone_Pt/Rmpf/Rmpf_zone4B_ptPhot_100_200");
  
  TMatrix mRzone(4,1);
  mRzone(0,0) = hResp[0]->GetMean();
  mRzone(1,0) = hResp[1]->GetMean();
  mRzone(2,0) = hResp[2]->GetMean();
  mRzone(3,0) = hResp[3]->GetMean();

  mRzone.Print();

  cout << "============================================================" << endl;
  cout << " The 2DTagging response:" << endl;
  cout << "============================================================" << endl;

  TMatrix mRflavour_2Dtagging(4,1);
  mRflavour_2Dtagging = minv*mRzone;

  mRflavour_2Dtagging.Print();
  

}
