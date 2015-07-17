#ifndef MY2DANA_h
#define MY2DANA_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <set>
#include "RootTupleDefs.h"
#include "TLorentzVector.h"

class My2DAna : public RootTupleDefs {

public :
  My2DAna(Bool_t doMC, Bool_t doRAW);
  ~My2DAna();
  
  void   Loop();
  
  void SetDebugMode(bool debug)                 { _debug    = debug;    }   
  void SetMCmode(bool isMC)                     { _isMC     = isMC;     }
  void SetRootName(const std::string& rootName) { _rootName = rootName; }
  void SetDoSkim(bool doSkim)                   { _doSkim   = doSkim;   }
  void SetNevent(int nevent)                    { _nevt = nevent;       }

  TLorentzVector* GetP4(TClonesArray* _TClonesArray, int _i) {
    return (TLorentzVector*) (*_TClonesArray)[_i];
  }

  // Additional event Variables computed in the loop
  Float_t Rmpf;
  Float_t alpha;

  Float_t DeltaPhi_j1gamma;
  Float_t DeltaPhi_j1met;
  Float_t DeltaEta_j1gamma;
  Float_t DeltaR_j1gamma;
  
  Float_t DeltaPhi_j2gamma;
  Float_t DeltaPhi_j1j2;
  Float_t DeltaEta_j2gamma;
  Float_t DeltaEta_j1j2;
  Float_t DeltaR_j2gamma;
  Float_t DeltaR_j1j2;

  Float_t metParal;
  Float_t metPerpend;

  Float_t Rtrue;
  Float_t Dphi_j1j1g;
  Float_t Deta_j1j1g;
  Float_t DR_j1j1g;

private:
  
  // Control flags
  bool _debug;
  bool _isMC;
  bool _doSkim;
  int _nevt;

  // Output file
  TFile* _newfile;
  std::string _rootName;

  // For Skimming
  TTree* _new_first_jetChain;
  TTree* _new_second_jetChain;
  TTree* _new_photonChain; 
  TTree* _new_metChain;
  TTree* _new_electronsChain;
  TTree* _new_muonsChain;
  TTree* _new_miscChain;
  TTree* _new_rhoChain;
  // RAW
  TTree* _new_first_jet_rawChain;
  TTree* _new_second_jet_rawChain;
  TTree* _new_met_rawChain;
  // MC only
  TTree* _new_first_jet_genChain;
  TTree* _new_second_jet_genChain;
  TTree* _new_met_genChain;
  TTree* _new_photon_genChain;

};

#endif
