{

  TFile *f = TFile::Open("PhotonJet_MC_TOT_PFlowAK5chs.root");
  gDirectory->cd("matrix");

  // Get the matrix and invert it
  TMatrixF m(4,4);
  TMatrixF minv(4,4);
  m = *((TMatrixF*)gDirectory->Get("matrix4x4_ptPhot_40_100"));

  cout << "============================================================" << endl;
  cout << " The flavour fraction matrix :" << endl;
  cout << "============================================================" << endl;
  
  m.Print();

  // Create a 5D Matrix
  
  TMatrixF m5d(5,5);

  for (int i=0; i<4; ++i) {

    // Compute the sum :
    Float_t sum = 0.;
    for (int j=0; j<4; ++j) {
      m5d(i,j) = m(i,j);
      sum += m(i,j);
    }
    cout << " Sum row " << i << " = " << sum << endl;
    m5d(i,4) = 1.-sum;

    // Normalize sum of each row to 1
    for (int j=0; j<4; ++j) 
      m(i,j) = m(i,j)/sum;

    // Check the sum:
    Float_t sum2 = 0.;
    for (int j=0; j<4; ++j) 
      sum2 += m(i,j);
    cout << " Sum2 row " << i << " = " << sum2 << endl;

  }
  
  m5d(4,0) = 0.;
  m5d(4,1) = 0.;
  m5d(4,2) = 0.;
  m5d(4,3) = 0.;
  m5d(4,4) = 1.;

  m5d.Print();


  cout << "============================================================" << endl;
  cout << " The inverted flavour matrix :" << endl;
  cout << "============================================================" << endl;

  minv = ((TMatrixF*)m->Clone())->Invert();
  minv.Print();

  TMatrixF m5dinv(5,5);
  m5dinv = ((TMatrixF*)m5d->Clone())->Invert();
  m5dinv.Print();


  cout << "============================================================" << endl;
  cout << " The response per zone" << endl;
  cout << "============================================================" << endl;

  f->cd();
  TH1F* hResp[5];
  hResp[0] = (TH1F*)f->Get("response_Zone_Pt/Rmpf/Rmpf_zone1Q_ptPhot_40_100");
  hResp[1] = (TH1F*)f->Get("response_Zone_Pt/Rmpf/Rmpf_zone2G_ptPhot_40_100");
  hResp[2] = (TH1F*)f->Get("response_Zone_Pt/Rmpf/Rmpf_zone3C_ptPhot_40_100");
  hResp[3] = (TH1F*)f->Get("response_Zone_Pt/Rmpf/Rmpf_zone4B_ptPhot_40_100");
  hResp[4] = (TH1F*)f->Get("response_Flavour_Pt/Rmpf/Rmpf_noMatched_ptPhot_40_100");
  
  TMatrix mRzone(4,1);
  mRzone(0,0) = hResp[0]->GetMean();
  mRzone(1,0) = hResp[1]->GetMean();
  mRzone(2,0) = hResp[2]->GetMean();
  mRzone(3,0) = hResp[3]->GetMean();
  mRzone.Print();

  TMatrix mRzone5d(5,1);
  mRzone5d(0,0) = hResp[0]->GetMean();
  mRzone5d(1,0) = hResp[1]->GetMean();
  mRzone5d(2,0) = hResp[2]->GetMean();
  mRzone5d(3,0) = hResp[3]->GetMean();
  mRzone5d(4,0) = hResp[4]->GetMean();
  mRzone5d.Print();

  cout << "============================================================" << endl;
  cout << " The 2DTagging response:" << endl;
  cout << "============================================================" << endl;

  TMatrix mRflavour_2Dtagging(4,1);
  mRflavour_2Dtagging = minv*mRzone;
  mRflavour_2Dtagging.Print();
  
  TMatrix mRflavour_2Dtagging5d(5,1);
  mRflavour_2Dtagging5d = m5dinv*mRzone5d;
  mRflavour_2Dtagging5d.Print();

}
