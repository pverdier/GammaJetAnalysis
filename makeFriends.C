TTree* makeFriends()
{

  TTree* t = (TTree*)gDirectory->Get("first_jet");
  
  TTree* ft[14];
  ft[0] = (TTree*)gDirectory->Get("misc");
  ft[1] = (TTree*)gDirectory->Get("first_jet_raw");
  ft[2] = (TTree*)gDirectory->Get("second_jet");
  ft[3] = (TTree*)gDirectory->Get("second_jet_raw");
  ft[4] = (TTree*)gDirectory->Get("photon");
  ft[5] = (TTree*)gDirectory->Get("met_raw");
  ft[6] = (TTree*)gDirectory->Get("met");
  ft[7] = (TTree*)gDirectory->Get("rho");
  ft[8] = (TTree*)gDirectory->Get("muons");
  ft[9] = (TTree*)gDirectory->Get("electrons");
 
  //  ft[10] = (TTree*)gDirectory->Get("first_jet_gen");
  //  ft[11] = (TTree*)gDirectory->Get("second_jet_gen");
  //  ft[12] = (TTree*)gDirectory->Get("met_gen");
  //  ft[13] = (TTree*)gDirectory->Get("photon_gen");

  for (unsigned int i=0; i<10; ++i ) {
    t->AddFriend(ft[i]);
  }
  
  return t;
}

