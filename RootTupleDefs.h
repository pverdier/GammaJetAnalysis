#ifndef RootTupleDefs_h
#define RootTupleDefs_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>

#include <TClonesArray.h>

class RootTupleDefs {
 public :

  //--------------------------------------------------------
  // List of TChain :
  //--------------------------------------------------------

  // Data and MC
  TTree* first_jetChain;
  TTree* second_jetChain;
  TTree* photonChain; 
  TTree* metChain;
  TTree* electronsChain;
  TTree* muonsChain;
  TTree* miscChain;
  TTree* rhoChain;
  // RAW
  TTree* first_jet_rawChain;
  TTree* second_jet_rawChain;
  TTree* met_rawChain;
  // MC only
  TTree* first_jet_genChain;
  TTree* second_jet_genChain;
  TTree* met_genChain;
  TTree* photon_genChain;
  
  Int_t           fCurrent; //!current Tree number in a TChain

   // New function to set branch status and adress at the same time

  void SetBranchStatusAndAddress (TTree* _subtree ,const char* _branchname , void* _branchptr ){
     _subtree->SetBranchStatus(_branchname,1);
     _subtree->SetBranchAddress(_branchname, _branchptr, NULL);
  }
  
  //--------------------------------------------------------
  // List of leaf
  //--------------------------------------------------------
  
  // Electrons:
   Int_t           e_n;
   Int_t           e_id[10];   //[n]
   Float_t         e_isolation[10];   //[n]
   Float_t         e_pt[10];   //[n]
   Float_t         e_px[10];   //[n]
   Float_t         e_py[10];   //[n]
   Float_t         e_pz[10];   //[n]
   Float_t         e_eta[10];   //[n]
   Float_t         e_phi[10];   //[n]
   Int_t           e_charge[10];   //[n]
  // Muons
   Int_t           m_n;
   Int_t           m_id[10];   //[n]
   Float_t         m_relative_isolation[10];   //[n]
   Float_t         m_delta_beta_relative_isolation[10];   //[n]
   Float_t         m_pt[10];   //[n]
   Float_t         m_px[10];   //[n]
   Float_t         m_py[10];   //[n]
   Float_t         m_pz[10];   //[n]
   Float_t         m_eta[10];   //[n]
   Float_t         m_phi[10];   //[n]
   Int_t           m_charge[10];   //[n]
  // First_jet
   Int_t           j1_is_present;
   Float_t         j1_et;
   Float_t         j1_pt;
   Float_t         j1_eta;
   Float_t         j1_phi;
   Float_t         j1_px;
   Float_t         j1_py;
   Float_t         j1_pz;
   Float_t         j1_e;
   Float_t         j1_jet_area;
   Float_t         j1_btag_tc_high_eff;
   Float_t         j1_btag_tc_high_pur;
   Float_t         j1_btag_ssv_high_eff;
   Float_t         j1_btag_ssv_high_pur;
   Float_t         j1_btag_jet_probability;
   Float_t         j1_btag_jet_b_probability;
   Float_t         j1_btag_csv;
   Float_t         j1_qg_tag_mlp;
   Float_t         j1_qg_tag_likelihood;
   Float_t         j1_jet_CHEn;
   Float_t         j1_jet_NHEn;
   Float_t         j1_jet_PhEn;
   Float_t         j1_jet_ElEn;
   Float_t         j1_jet_MuEn;
   Float_t         j1_jet_CEEn;
   Float_t         j1_jet_NEEn;
   Int_t           j1_jet_PhMult;
   Int_t           j1_jet_NHMult;
   Int_t           j1_jet_ElMult;
   Int_t           j1_jet_CHMult;
  // First_jet_gen
   Int_t           j1g_is_present;
   Float_t         j1g_et;
   Float_t         j1g_pt;
   Float_t         j1g_eta;
   Float_t         j1g_phi;
   Float_t         j1g_px;
   Float_t         j1g_py;
   Float_t         j1g_pz;
   Float_t         j1g_e;
   TClonesArray    *j1g_neutrinos;
   TClonesArray    *j1g_neutrinos_pdg_id;
   Int_t           j1g_parton_pdg_id;
   Int_t           j1g_parton_flavour;
  // First_jet_raw
   Int_t           j1r_is_present;
   Float_t         j1r_et;
   Float_t         j1r_pt;
   Float_t         j1r_eta;
   Float_t         j1r_phi;
   Float_t         j1r_px;
   Float_t         j1r_py;
   Float_t         j1r_pz;
   Float_t         j1r_e;
   Float_t         j1r_jet_area;
   Float_t         j1r_btag_tc_high_eff;
   Float_t         j1r_btag_tc_high_pur;
   Float_t         j1r_btag_ssv_high_eff;
   Float_t         j1r_btag_ssv_high_pur;
   Float_t         j1r_btag_jet_probability;
   Float_t         j1r_btag_jet_b_probability;
   Float_t         j1r_btag_csv;
   Float_t         j1r_qg_tag_mlp;
   Float_t         j1r_qg_tag_likelihood;
   Float_t         j1r_jet_CHEn;
   Float_t         j1r_jet_NHEn;
   Float_t         j1r_jet_PhEn;
   Float_t         j1r_jet_ElEn;
   Float_t         j1r_jet_MuEn;
   Float_t         j1r_jet_CEEn;
   Float_t         j1r_jet_NEEn;
   Int_t           j1r_jet_PhMult;
   Int_t           j1r_jet_NHMult;
   Int_t           j1r_jet_ElMult;
   Int_t           j1r_jet_CHMult;
  // Met
   Int_t           met_is_present;
   Float_t         met_et;
   Float_t         met_pt;
   Float_t         met_eta;
   Float_t         met_phi;
   Float_t         met_px;
   Float_t         met_py;
   Float_t         met_pz;
   Float_t         met_e;
  // Met_gen
   Int_t           metg_is_present;
   Float_t         metg_et;
   Float_t         metg_pt;
   Float_t         metg_eta;
   Float_t         metg_phi;
   Float_t         metg_px;
   Float_t         metg_py;
   Float_t         metg_pz;
   Float_t         metg_e;
  // Met_raw 
   Int_t           metr_is_present;
   Float_t         metr_et;
   Float_t         metr_pt;
   Float_t         metr_eta;
   Float_t         metr_phi;
   Float_t         metr_px;
   Float_t         metr_py;
   Float_t         metr_pz;
   Float_t         metr_e;
  // Misc
   UInt_t          run;
   UInt_t          lumi_block;
   UInt_t          event;
   UInt_t          nvertex;
   Float_t         ntrue_interactions;
   Int_t           pu_nvertex;
   Float_t         event_weight;
   Double_t        generator_weight;
   std::vector<std::string>  *trigger_names;
   std::vector<bool>    *trigger_results;
  // Photon
   Int_t           p_is_present;
   Float_t         p_et;
   Float_t         p_pt;
   Float_t         p_eta;
   Float_t         p_phi;
   Float_t         p_px;
   Float_t         p_py;
   Float_t         p_pz;
   Float_t         p_e;
   Bool_t          p_has_pixel_seed;
   Float_t         p_hadTowOverEm;
   Float_t         p_sigmaIetaIeta;
   Float_t         p_rho;
   Bool_t          p_hasMatchedPromptElectron;
   Float_t         p_regressionEnergy;
   Float_t         p_originalEnergy;
   Float_t         p_footprintMExCorr;
   Float_t         p_footprintMEyCorr;
   Float_t         p_chargedHadronsIsolation;
   Float_t         p_neutralHadronsIsolation;
   Float_t         p_photonIsolation;
  // Photon_gen
   Int_t           pg_is_present;
   Float_t         pg_et;
   Float_t         pg_pt;
   Float_t         pg_eta;
   Float_t         pg_phi;
   Float_t         pg_px;
   Float_t         pg_py;
   Float_t         pg_pz;
   Float_t         pg_e;
  // Second_jet
   Int_t           j2_is_present;
   Float_t         j2_et;
   Float_t         j2_pt;
   Float_t         j2_eta;
   Float_t         j2_phi;
   Float_t         j2_px;
   Float_t         j2_py;
   Float_t         j2_pz;
   Float_t         j2_e;
   Float_t         j2_jet_area;
   Float_t         j2_btag_tc_high_eff;
   Float_t         j2_btag_tc_high_pur;
   Float_t         j2_btag_ssv_high_eff;
   Float_t         j2_btag_ssv_high_pur;
   Float_t         j2_btag_jet_probability;
   Float_t         j2_btag_jet_b_probability;
   Float_t         j2_btag_csv;
   Float_t         j2_qg_tag_mlp;
   Float_t         j2_qg_tag_likelihood;
   Float_t         j2_jet_CHEn;
   Float_t         j2_jet_NHEn;
   Float_t         j2_jet_PhEn;
   Float_t         j2_jet_ElEn;
   Float_t         j2_jet_MuEn;
   Float_t         j2_jet_CEEn;
   Float_t         j2_jet_NEEn;
   Int_t           j2_jet_PhMult;
   Int_t           j2_jet_NHMult;
   Int_t           j2_jet_ElMult;
   Int_t           j2_jet_CHMult;
  // Second_jet_gen
   Int_t           j2g_is_present;
   Float_t         j2g_et;
   Float_t         j2g_pt;
   Float_t         j2g_eta;
   Float_t         j2g_phi;
   Float_t         j2g_px;
   Float_t         j2g_py;
   Float_t         j2g_pz;
   Float_t         j2g_e;
   TClonesArray    *j2g_neutrinos;
   TClonesArray    *j2g_neutrinos_pdg_id;
   Int_t           j2g_parton_pdg_id;
   Int_t           j2g_parton_flavour;
  // Second_jet_raw
   Int_t           j2r_is_present;
   Float_t         j2r_et;
   Float_t         j2r_pt;
   Float_t         j2r_eta;
   Float_t         j2r_phi;
   Float_t         j2r_px;
   Float_t         j2r_py;
   Float_t         j2r_pz;
   Float_t         j2r_e;
   Float_t         j2r_jet_area;
   Float_t         j2r_btag_tc_high_eff;
   Float_t         j2r_btag_tc_high_pur;
   Float_t         j2r_btag_ssv_high_eff;
   Float_t         j2r_btag_ssv_high_pur;
   Float_t         j2r_btag_jet_probability;
   Float_t         j2r_btag_jet_b_probability;
   Float_t         j2r_btag_csv;
   Float_t         j2r_qg_tag_mlp;
   Float_t         j2r_qg_tag_likelihood;
   Float_t         j2r_jet_CHEn;
   Float_t         j2r_jet_NHEn;
   Float_t         j2r_jet_PhEn;
   Float_t         j2r_jet_ElEn;
   Float_t         j2r_jet_MuEn;
   Float_t         j2r_jet_CEEn;
   Float_t         j2r_jet_NEEn;
   Int_t           j2r_jet_PhMult;
   Int_t           j2r_jet_NHMult;
   Int_t           j2r_jet_ElMult;
   Int_t           j2r_jet_CHMult;
  // Rho
   Double_t        rho;

  //-------------------------------------------------------
  // Functions
  //-------------------------------------------------------

   RootTupleDefs(Bool_t doMC=false, Bool_t doRAW=false);

   virtual ~RootTupleDefs();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree* _first_jetChain,
			 TTree* _second_jetChain,
			 TTree* _photonChain,
			 TTree* _metChain,
			 TTree* _electronsChain,
			 TTree* _muonsChain,
			 TTree* _miscChain,
			 TTree* _rhoChain,
			 // raw Info
			 TTree* _first_jet_rawChain = 0,
			 TTree* _second_jet_rawChain = 0,
			 TTree* _met_rawChain = 0,
			 // MC only
			 TTree* _first_jet_genChain = 0,
			 TTree* _second_jet_genChain = 0,
			 TTree* _met_genChain = 0,
			 TTree* _photon_genChain = 0);

   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

  //-------------------------------------------------------
  // Specific functions
  //-------------------------------------------------------
   
    void SetDoMC(bool doMC)   { _doMC  = doMC; }
    void SetDoRAW(bool doRAW) { _doRAW = doRAW; }
    
 protected:
    bool _doMC;
    bool _doRAW;
};

#endif

//===============================================================================================================
//
//  THE FUNCTIONS
//
//===============================================================================================================

#ifdef My2DAna_cpp
RootTupleDefs::RootTupleDefs(bool doMC, bool doRAW)
{
  
  _doMC  = doMC;
  _doRAW = doRAW;

  // Init should be called in the main code just after the constructor
}

RootTupleDefs::~RootTupleDefs()
{
   if (!first_jetChain) return;
   delete first_jetChain->GetCurrentFile();//no need to do it for other trees because there is only one file
}

Int_t RootTupleDefs::GetEntry(Long64_t entry)
{
// Read contents of entry for all 5 trees
   if (!first_jetChain) return 0;

   first_jetChain->GetEntry(entry);
   second_jetChain->GetEntry(entry);
   photonChain->GetEntry(entry);
   metChain->GetEntry(entry);
   electronsChain->GetEntry(entry);
   muonsChain->GetEntry(entry);
   miscChain->GetEntry(entry);
   rhoChain->GetEntry(entry);
   if ( _doRAW ) {
     first_jet_rawChain->GetEntry(entry);
     second_jet_rawChain->GetEntry(entry);
     met_rawChain->GetEntry(entry);
   }
   if ( _doMC ) {
     first_jet_genChain->GetEntry(entry);
     second_jet_genChain->GetEntry(entry);
     met_genChain->GetEntry(entry);
     photon_genChain->GetEntry(entry);
   }
 
   return first_jetChain->GetEntry(entry); //return muon tree size to be consistent with the old make class structure 
}

Long64_t RootTupleDefs::LoadTree(Long64_t entry) //not used in myana
{
// Set the environment to read one entry
   if (!first_jetChain) return -5;
   Long64_t centry = first_jetChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (first_jetChain->GetTreeNumber() != fCurrent) {
      fCurrent = first_jetChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void RootTupleDefs::Init(TTree* _first_jetChain,
			 TTree* _second_jetChain,
			 TTree* _photonChain,
			 TTree* _metChain,
			 TTree* _electronsChain,
			 TTree* _muonsChain,
			 TTree* _miscChain,
			 TTree* _rhoChain,
			 // raw Info
			 TTree* _first_jet_rawChain,
			 TTree* _second_jet_rawChain,
			 TTree* _met_rawChain,
			 // MC only
			 TTree* _first_jet_genChain,
			 TTree* _second_jet_genChain,
			 TTree* _met_genChain,
			 TTree* _photon_genChain)
{

  //-----------------------------------
  // Process inputs:
  //-----------------------------------

  first_jetChain  = _first_jetChain;
  second_jetChain = _second_jetChain;
  photonChain     = _photonChain;
  metChain        = _metChain;
  electronsChain  = _electronsChain;
  muonsChain      = _muonsChain;
  miscChain       = _miscChain;
  rhoChain        = _rhoChain;
  if ( _doRAW ) {
    first_jet_rawChain  = _first_jet_rawChain;
    second_jet_rawChain = _second_jet_rawChain;
    met_rawChain        = _met_rawChain;
  }
  if ( _doMC ) {
    first_jet_genChain  = _first_jet_genChain;
    second_jet_genChain = _second_jet_genChain;
    met_genChain        = _met_genChain;
    photon_genChain     = _photon_genChain;
  }
  
  //-----------------------------------
  // Leafs for all
  //-----------------------------------

  // First_jet
  fCurrent = -1;
  first_jetChain->SetMakeClass(1);
  first_jetChain->SetBranchStatus("*",0); //desactivate all branches
  
  SetBranchStatusAndAddress(first_jetChain,"is_present", &j1_is_present);
  SetBranchStatusAndAddress(first_jetChain,"et", &j1_et);
  SetBranchStatusAndAddress(first_jetChain,"pt", &j1_pt);
  SetBranchStatusAndAddress(first_jetChain,"eta", &j1_eta);
  SetBranchStatusAndAddress(first_jetChain,"phi", &j1_phi);
  SetBranchStatusAndAddress(first_jetChain,"px", &j1_px);
  SetBranchStatusAndAddress(first_jetChain,"py", &j1_py);
  SetBranchStatusAndAddress(first_jetChain,"pz", &j1_pz);
  SetBranchStatusAndAddress(first_jetChain,"e", &j1_e);
  SetBranchStatusAndAddress(first_jetChain,"jet_area", &j1_jet_area);
  SetBranchStatusAndAddress(first_jetChain,"btag_tc_high_eff", &j1_btag_tc_high_eff);
  SetBranchStatusAndAddress(first_jetChain,"btag_tc_high_pur", &j1_btag_tc_high_pur);
  SetBranchStatusAndAddress(first_jetChain,"btag_ssv_high_eff", &j1_btag_ssv_high_eff);
  SetBranchStatusAndAddress(first_jetChain,"btag_ssv_high_pur", &j1_btag_ssv_high_pur);
  SetBranchStatusAndAddress(first_jetChain,"btag_jet_probability", &j1_btag_jet_probability);
  SetBranchStatusAndAddress(first_jetChain,"btag_jet_b_probability", &j1_btag_jet_b_probability);
  SetBranchStatusAndAddress(first_jetChain,"btag_csv", &j1_btag_csv);
  SetBranchStatusAndAddress(first_jetChain,"qg_tag_mlp", &j1_qg_tag_mlp);
  SetBranchStatusAndAddress(first_jetChain,"qg_tag_likelihood", &j1_qg_tag_likelihood);
  SetBranchStatusAndAddress(first_jetChain,"jet_CHEn", &j1_jet_CHEn);
  SetBranchStatusAndAddress(first_jetChain,"jet_NHEn", &j1_jet_NHEn);
  SetBranchStatusAndAddress(first_jetChain,"jet_PhEn", &j1_jet_PhEn);
  SetBranchStatusAndAddress(first_jetChain,"jet_ElEn", &j1_jet_ElEn);
  SetBranchStatusAndAddress(first_jetChain,"jet_MuEn", &j1_jet_MuEn);
  SetBranchStatusAndAddress(first_jetChain,"jet_CEEn", &j1_jet_CEEn);
  SetBranchStatusAndAddress(first_jetChain,"jet_NEEn", &j1_jet_NEEn);
  SetBranchStatusAndAddress(first_jetChain,"jet_PhMult", &j1_jet_PhMult);
  SetBranchStatusAndAddress(first_jetChain,"jet_NHMult", &j1_jet_NHMult);
  SetBranchStatusAndAddress(first_jetChain,"jet_ElMult", &j1_jet_ElMult);
  SetBranchStatusAndAddress(first_jetChain,"jet_CHMult", &j1_jet_CHMult);

  // Second_jet
  fCurrent = -1;
  second_jetChain->SetMakeClass(1);
  second_jetChain->SetBranchStatus("*",0); //desactivate all branches
  
  SetBranchStatusAndAddress(second_jetChain,"is_present", &j2_is_present);
  SetBranchStatusAndAddress(second_jetChain,"et", &j2_et);
  SetBranchStatusAndAddress(second_jetChain,"pt", &j2_pt);
  SetBranchStatusAndAddress(second_jetChain,"eta", &j2_eta);
  SetBranchStatusAndAddress(second_jetChain,"phi", &j2_phi);
  SetBranchStatusAndAddress(second_jetChain,"px", &j2_px);
  SetBranchStatusAndAddress(second_jetChain,"py", &j2_py);
  SetBranchStatusAndAddress(second_jetChain,"pz", &j2_pz);
  SetBranchStatusAndAddress(second_jetChain,"e", &j2_e);
  SetBranchStatusAndAddress(second_jetChain,"jet_area", &j2_jet_area);
  SetBranchStatusAndAddress(second_jetChain,"btag_tc_high_eff", &j2_btag_tc_high_eff);
  SetBranchStatusAndAddress(second_jetChain,"btag_tc_high_pur", &j2_btag_tc_high_pur);
  SetBranchStatusAndAddress(second_jetChain,"btag_ssv_high_eff", &j2_btag_ssv_high_eff);
  SetBranchStatusAndAddress(second_jetChain,"btag_ssv_high_pur", &j2_btag_ssv_high_pur);
  SetBranchStatusAndAddress(second_jetChain,"btag_jet_probability", &j2_btag_jet_probability);
  SetBranchStatusAndAddress(second_jetChain,"btag_jet_b_probability", &j2_btag_jet_b_probability);
  SetBranchStatusAndAddress(second_jetChain,"btag_csv", &j2_btag_csv);
  SetBranchStatusAndAddress(second_jetChain,"qg_tag_mlp", &j2_qg_tag_mlp);
  SetBranchStatusAndAddress(second_jetChain,"qg_tag_likelihood", &j2_qg_tag_likelihood);
  SetBranchStatusAndAddress(second_jetChain,"jet_CHEn", &j2_jet_CHEn);
  SetBranchStatusAndAddress(second_jetChain,"jet_NHEn", &j2_jet_NHEn);
  SetBranchStatusAndAddress(second_jetChain,"jet_PhEn", &j2_jet_PhEn);
  SetBranchStatusAndAddress(second_jetChain,"jet_ElEn", &j2_jet_ElEn);
  SetBranchStatusAndAddress(second_jetChain,"jet_MuEn", &j2_jet_MuEn);
  SetBranchStatusAndAddress(second_jetChain,"jet_CEEn", &j2_jet_CEEn);
  SetBranchStatusAndAddress(second_jetChain,"jet_NEEn", &j2_jet_NEEn);
  SetBranchStatusAndAddress(second_jetChain,"jet_PhMult", &j2_jet_PhMult);
  SetBranchStatusAndAddress(second_jetChain,"jet_NHMult", &j2_jet_NHMult);
  SetBranchStatusAndAddress(second_jetChain,"jet_ElMult", &j2_jet_ElMult);
  SetBranchStatusAndAddress(second_jetChain,"jet_CHMult", &j2_jet_CHMult);

  // Photon
  fCurrent = -1;
  photonChain->SetMakeClass(1);
  photonChain->SetBranchStatus("*",0); //desactivate all branches
  
  SetBranchStatusAndAddress(photonChain,"is_present", &p_is_present);
  SetBranchStatusAndAddress(photonChain,"et", &p_et);
  SetBranchStatusAndAddress(photonChain,"pt", &p_pt);
  SetBranchStatusAndAddress(photonChain,"eta", &p_eta);
  SetBranchStatusAndAddress(photonChain,"phi", &p_phi);
  SetBranchStatusAndAddress(photonChain,"px", &p_px);
  SetBranchStatusAndAddress(photonChain,"py", &p_py);
  SetBranchStatusAndAddress(photonChain,"pz", &p_pz);
  SetBranchStatusAndAddress(photonChain,"e", &p_e);
  SetBranchStatusAndAddress(photonChain,"has_pixel_seed", &p_has_pixel_seed);
  SetBranchStatusAndAddress(photonChain,"hadTowOverEm", &p_hadTowOverEm);
  SetBranchStatusAndAddress(photonChain,"sigmaIetaIeta", &p_sigmaIetaIeta);
  SetBranchStatusAndAddress(photonChain,"rho", &p_rho);
  SetBranchStatusAndAddress(photonChain,"hasMatchedPromptElectron", &p_hasMatchedPromptElectron);
  SetBranchStatusAndAddress(photonChain,"regressionEnergy", &p_regressionEnergy);
  SetBranchStatusAndAddress(photonChain,"originalEnergy", &p_originalEnergy);
  SetBranchStatusAndAddress(photonChain,"footprintMExCorr", &p_footprintMExCorr);
  SetBranchStatusAndAddress(photonChain,"footprintMEyCorr", &p_footprintMEyCorr);
  SetBranchStatusAndAddress(photonChain,"chargedHadronsIsolation", &p_chargedHadronsIsolation);
  SetBranchStatusAndAddress(photonChain,"neutralHadronsIsolation", &p_neutralHadronsIsolation);
  SetBranchStatusAndAddress(photonChain,"photonIsolation", &p_photonIsolation);

  // Met
  fCurrent = -1;
  metChain->SetMakeClass(1);
  metChain->SetBranchStatus("*",0); //desactivate all branches

  SetBranchStatusAndAddress(metChain,"is_present", &met_is_present);
  SetBranchStatusAndAddress(metChain,"et", &met_et);
  SetBranchStatusAndAddress(metChain,"pt", &met_pt);
  SetBranchStatusAndAddress(metChain,"eta", &met_eta);
  SetBranchStatusAndAddress(metChain,"phi", &met_phi);
  SetBranchStatusAndAddress(metChain,"px", &met_px);
  SetBranchStatusAndAddress(metChain,"py", &met_py);
  SetBranchStatusAndAddress(metChain,"pz", &met_pz);
  SetBranchStatusAndAddress(metChain,"e", &met_e);
  
  // Electrons
  fCurrent = -1;
  electronsChain->SetMakeClass(1);
  electronsChain->SetBranchStatus("*",0); //desactivate all branches

  SetBranchStatusAndAddress(electronsChain,"n", &e_n);
  SetBranchStatusAndAddress(electronsChain,"id", &e_id);
  SetBranchStatusAndAddress(electronsChain,"isolation", &e_isolation);
  SetBranchStatusAndAddress(electronsChain,"pt", &e_pt);
  SetBranchStatusAndAddress(electronsChain,"px", &e_px);
  SetBranchStatusAndAddress(electronsChain,"py", &e_py);
  SetBranchStatusAndAddress(electronsChain,"pz", &e_pz);
  SetBranchStatusAndAddress(electronsChain,"eta", &e_eta);
  SetBranchStatusAndAddress(electronsChain,"phi", &e_phi);
  SetBranchStatusAndAddress(electronsChain,"charge", &e_charge);

  // Muons
  fCurrent = -1;
  muonsChain->SetMakeClass(1);
  muonsChain->SetBranchStatus("*",0); //desactivate all branches
  
  SetBranchStatusAndAddress(muonsChain,"n", &m_n);
  SetBranchStatusAndAddress(muonsChain,"id", &m_id);
  SetBranchStatusAndAddress(muonsChain,"relative_isolation", &m_relative_isolation);
  SetBranchStatusAndAddress(muonsChain,"delta_beta_relative_isolation", &m_delta_beta_relative_isolation);
  SetBranchStatusAndAddress(muonsChain,"pt", &m_pt);
  SetBranchStatusAndAddress(muonsChain,"px", &m_px);
  SetBranchStatusAndAddress(muonsChain,"py", &m_py);
  SetBranchStatusAndAddress(muonsChain,"pz", &m_pz);
  SetBranchStatusAndAddress(muonsChain,"eta", &m_eta);
  SetBranchStatusAndAddress(muonsChain,"phi", &m_phi);
  SetBranchStatusAndAddress(muonsChain,"charge", &m_charge);
  
  // Misc
  fCurrent = -1;
  miscChain->SetMakeClass(1);
  miscChain->SetBranchStatus("*",0); //desactivate all branches
  
  trigger_names   = NULL;
  trigger_results = NULL;

  SetBranchStatusAndAddress(miscChain,"run", &run);
  SetBranchStatusAndAddress(miscChain,"lumi_block", &lumi_block);
  SetBranchStatusAndAddress(miscChain,"event", &event);
  SetBranchStatusAndAddress(miscChain,"nvertex", &nvertex);
  SetBranchStatusAndAddress(miscChain,"ntrue_interactions",&ntrue_interactions);
  SetBranchStatusAndAddress(miscChain,"pu_nvertex", &pu_nvertex);
  SetBranchStatusAndAddress(miscChain,"event_weight", &event_weight);
  SetBranchStatusAndAddress(miscChain,"generator_weight", &generator_weight);
  SetBranchStatusAndAddress(miscChain,"trigger_names", &trigger_names);
  SetBranchStatusAndAddress(miscChain,"trigger_results", &trigger_results);

  // Rho
  fCurrent = -1;
  rhoChain->SetMakeClass(1);
  rhoChain->SetBranchStatus("*",0); //desactivate all branches
  
  SetBranchStatusAndAddress(rhoChain,"rho", &rho);

  //-----------------------------------
  // Leafs for RAW
  //-----------------------------------

  if ( _doRAW ) {

  // First_jet_raw
  fCurrent = -1;
  first_jet_rawChain->SetMakeClass(1);
  first_jet_rawChain->SetBranchStatus("*",0); //desactivate all branches
  
  SetBranchStatusAndAddress(first_jet_rawChain,"is_present", &j1r_is_present);
  SetBranchStatusAndAddress(first_jet_rawChain,"et", &j1r_et);
  SetBranchStatusAndAddress(first_jet_rawChain,"pt", &j1r_pt);
  SetBranchStatusAndAddress(first_jet_rawChain,"eta", &j1r_eta);
  SetBranchStatusAndAddress(first_jet_rawChain,"phi", &j1r_phi);
  SetBranchStatusAndAddress(first_jet_rawChain,"px", &j1r_px);
  SetBranchStatusAndAddress(first_jet_rawChain,"py", &j1r_py);
  SetBranchStatusAndAddress(first_jet_rawChain,"pz", &j1r_pz);
  SetBranchStatusAndAddress(first_jet_rawChain,"e", &j1r_e);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_area", &j1r_jet_area);
  SetBranchStatusAndAddress(first_jet_rawChain,"btag_tc_high_eff", &j1r_btag_tc_high_eff);
  SetBranchStatusAndAddress(first_jet_rawChain,"btag_tc_high_pur", &j1r_btag_tc_high_pur);
  SetBranchStatusAndAddress(first_jet_rawChain,"btag_ssv_high_eff", &j1r_btag_ssv_high_eff);
  SetBranchStatusAndAddress(first_jet_rawChain,"btag_ssv_high_pur", &j1r_btag_ssv_high_pur);
  SetBranchStatusAndAddress(first_jet_rawChain,"btag_jet_probability", &j1r_btag_jet_probability);
  SetBranchStatusAndAddress(first_jet_rawChain,"btag_jet_b_probability", &j1r_btag_jet_b_probability);
  SetBranchStatusAndAddress(first_jet_rawChain,"btag_csv", &j1r_btag_csv);
  SetBranchStatusAndAddress(first_jet_rawChain,"qg_tag_mlp", &j1r_qg_tag_mlp);
  SetBranchStatusAndAddress(first_jet_rawChain,"qg_tag_likelihood", &j1r_qg_tag_likelihood);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_CHEn", &j1r_jet_CHEn);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_NHEn", &j1r_jet_NHEn);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_PhEn", &j1r_jet_PhEn);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_ElEn", &j1r_jet_ElEn);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_MuEn", &j1r_jet_MuEn);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_CEEn", &j1r_jet_CEEn);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_NEEn", &j1r_jet_NEEn);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_PhMult", &j1r_jet_PhMult);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_NHMult", &j1r_jet_NHMult);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_ElMult", &j1r_jet_ElMult);
  SetBranchStatusAndAddress(first_jet_rawChain,"jet_CHMult", &j1r_jet_CHMult);

  // Second_jet_raw
  fCurrent = -1;
  second_jet_rawChain->SetMakeClass(1);
  second_jet_rawChain->SetBranchStatus("*",0); //desactivate all branches
  
  SetBranchStatusAndAddress(second_jet_rawChain,"is_present", &j2r_is_present);
  SetBranchStatusAndAddress(second_jet_rawChain,"et", &j2r_et);
  SetBranchStatusAndAddress(second_jet_rawChain,"pt", &j2r_pt);
  SetBranchStatusAndAddress(second_jet_rawChain,"eta", &j2r_eta);
  SetBranchStatusAndAddress(second_jet_rawChain,"phi", &j2r_phi);
  SetBranchStatusAndAddress(second_jet_rawChain,"px", &j2r_px);
  SetBranchStatusAndAddress(second_jet_rawChain,"py", &j2r_py);
  SetBranchStatusAndAddress(second_jet_rawChain,"pz", &j2r_pz);
  SetBranchStatusAndAddress(second_jet_rawChain,"e", &j2r_e);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_area", &j2r_jet_area);
  SetBranchStatusAndAddress(second_jet_rawChain,"btag_tc_high_eff", &j2r_btag_tc_high_eff);
  SetBranchStatusAndAddress(second_jet_rawChain,"btag_tc_high_pur", &j2r_btag_tc_high_pur);
  SetBranchStatusAndAddress(second_jet_rawChain,"btag_ssv_high_eff", &j2r_btag_ssv_high_eff);
  SetBranchStatusAndAddress(second_jet_rawChain,"btag_ssv_high_pur", &j2r_btag_ssv_high_pur);
  SetBranchStatusAndAddress(second_jet_rawChain,"btag_jet_probability", &j2r_btag_jet_probability);
  SetBranchStatusAndAddress(second_jet_rawChain,"btag_jet_b_probability", &j2r_btag_jet_b_probability);
  SetBranchStatusAndAddress(second_jet_rawChain,"btag_csv", &j2r_btag_csv);
  SetBranchStatusAndAddress(second_jet_rawChain,"qg_tag_mlp", &j2r_qg_tag_mlp);
  SetBranchStatusAndAddress(second_jet_rawChain,"qg_tag_likelihood", &j2r_qg_tag_likelihood);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_CHEn", &j2r_jet_CHEn);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_NHEn", &j2r_jet_NHEn);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_PhEn", &j2r_jet_PhEn);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_ElEn", &j2r_jet_ElEn);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_MuEn", &j2r_jet_MuEn);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_CEEn", &j2r_jet_CEEn);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_NEEn", &j2r_jet_NEEn);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_PhMult", &j2r_jet_PhMult);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_NHMult", &j2r_jet_NHMult);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_ElMult", &j2r_jet_ElMult);
  SetBranchStatusAndAddress(second_jet_rawChain,"jet_CHMult", &j2r_jet_CHMult);

  // Met_raw
  fCurrent = -1;
  met_rawChain->SetMakeClass(1);
  met_rawChain->SetBranchStatus("*",0); //desactivate all branches

  SetBranchStatusAndAddress(met_rawChain,"is_present", &metr_is_present);
  SetBranchStatusAndAddress(met_rawChain,"et", &metr_et);
  SetBranchStatusAndAddress(met_rawChain,"pt", &metr_pt);
  SetBranchStatusAndAddress(met_rawChain,"eta", &metr_eta);
  SetBranchStatusAndAddress(met_rawChain,"phi", &metr_phi);
  SetBranchStatusAndAddress(met_rawChain,"px", &metr_px);
  SetBranchStatusAndAddress(met_rawChain,"py", &metr_py);
  SetBranchStatusAndAddress(met_rawChain,"pz", &metr_pz);
  SetBranchStatusAndAddress(met_rawChain,"e", &metr_e);
  
  } // end _doRAW

  //-----------------------------------
  // Leafs for MC
  //-----------------------------------

  if ( _doMC ) {

  // First_jet_gen
  fCurrent = -1;
  first_jet_genChain->SetMakeClass(1);
  first_jet_genChain->SetBranchStatus("*",0); //desactivate all branches
  
  j1g_neutrinos        = NULL;
  j1g_neutrinos_pdg_id = NULL;
  
  SetBranchStatusAndAddress(first_jet_genChain,"is_present", &j1g_is_present);
  SetBranchStatusAndAddress(first_jet_genChain,"et", &j1g_et);
  SetBranchStatusAndAddress(first_jet_genChain,"pt", &j1g_pt);
  SetBranchStatusAndAddress(first_jet_genChain,"eta", &j1g_eta);
  SetBranchStatusAndAddress(first_jet_genChain,"phi", &j1g_phi);
  SetBranchStatusAndAddress(first_jet_genChain,"px", &j1g_px);
  SetBranchStatusAndAddress(first_jet_genChain,"py", &j1g_py);
  SetBranchStatusAndAddress(first_jet_genChain,"pz", &j1g_pz);
  SetBranchStatusAndAddress(first_jet_genChain,"e", &j1g_e);
  //SetBranchStatusAndAddress(first_jet_genChain,"neutrinos", &j1g_neutrinos);
  //  SetBranchStatusAndAddress(first_jet_genChain,"neutrinos_pdg_id", &j1g_neutrinos_pdg_id);
  SetBranchStatusAndAddress(first_jet_genChain,"parton_pdg_id", &j1g_parton_pdg_id);
  SetBranchStatusAndAddress(first_jet_genChain,"parton_flavour", &j1g_parton_flavour);

  // Second_jet_gen
  fCurrent = -1;
  second_jet_genChain->SetMakeClass(1);
  second_jet_genChain->SetBranchStatus("*",0); //desactivate all branches
  
  j2g_neutrinos        = NULL;
  j2g_neutrinos_pdg_id = NULL;
  
  SetBranchStatusAndAddress(second_jet_genChain,"is_present", &j2g_is_present);
  SetBranchStatusAndAddress(second_jet_genChain,"et", &j2g_et);
  SetBranchStatusAndAddress(second_jet_genChain,"pt", &j2g_pt);
  SetBranchStatusAndAddress(second_jet_genChain,"eta", &j2g_eta);
  SetBranchStatusAndAddress(second_jet_genChain,"phi", &j2g_phi);
  SetBranchStatusAndAddress(second_jet_genChain,"px", &j2g_px);
  SetBranchStatusAndAddress(second_jet_genChain,"py", &j2g_py);
  SetBranchStatusAndAddress(second_jet_genChain,"pz", &j2g_pz);
  SetBranchStatusAndAddress(second_jet_genChain,"e", &j2g_e);
  //SetBranchStatusAndAddress(second_jet_genChain,"neutrinos", &j2g_neutrinos);
  //SetBranchStatusAndAddress(second_jet_genChain,"neutrinos_pdg_id", &j2g_neutrinos_pdg_id);
  SetBranchStatusAndAddress(second_jet_genChain,"parton_pdg_id", &j2g_parton_pdg_id);
  SetBranchStatusAndAddress(second_jet_genChain,"parton_flavour", &j2g_parton_flavour);

  // Met_gen
  fCurrent = -1;
  met_genChain->SetMakeClass(1);
  met_genChain->SetBranchStatus("*",0); //desactivate all branches

  SetBranchStatusAndAddress(met_genChain,"is_present", &metg_is_present);
  SetBranchStatusAndAddress(met_genChain,"et", &metg_et);
  SetBranchStatusAndAddress(met_genChain,"pt", &metg_pt);
  SetBranchStatusAndAddress(met_genChain,"eta", &metg_eta);
  SetBranchStatusAndAddress(met_genChain,"phi", &metg_phi);
  SetBranchStatusAndAddress(met_genChain,"px", &metg_px);
  SetBranchStatusAndAddress(met_genChain,"py", &metg_py);
  SetBranchStatusAndAddress(met_genChain,"pz", &metg_pz);
  SetBranchStatusAndAddress(met_genChain,"e", &metg_e);

  // Photon_gen
  fCurrent = -1;
  photon_genChain->SetMakeClass(1);
  photon_genChain->SetBranchStatus("*",0); //desactivate all branches
  
  SetBranchStatusAndAddress(photon_genChain,"is_present", &pg_is_present);
  SetBranchStatusAndAddress(photon_genChain,"et", &pg_et);
  SetBranchStatusAndAddress(photon_genChain,"pt", &pg_pt);
  SetBranchStatusAndAddress(photon_genChain,"eta", &pg_eta);
  SetBranchStatusAndAddress(photon_genChain,"phi", &pg_phi);
  SetBranchStatusAndAddress(photon_genChain,"px", &pg_px);
  SetBranchStatusAndAddress(photon_genChain,"py", &pg_py);
  SetBranchStatusAndAddress(photon_genChain,"pz", &pg_pz);
  SetBranchStatusAndAddress(photon_genChain,"e", &pg_e);

  } // end _doMC

  Notify();
}

Bool_t RootTupleDefs::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void RootTupleDefs::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!muonsChain) return;
   muonsChain->Show(entry);
}
Int_t RootTupleDefs::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef RootTupleDefs_cxx
