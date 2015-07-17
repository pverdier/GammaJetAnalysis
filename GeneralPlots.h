#ifndef GENERALPLOTS_H
#define GENERALPLOTS_H

#include <My2DAna.h>
#include <string>

#include <TROOT.h>
#include "TFile.h"
#include "TDirectory.h"
#include "TH1F.h"
#include "TH2F.h"
#include <TStyle.h>

class GeneralPlots
{
 public:
  GeneralPlots(TFile* file, std::string tdirname, My2DAna* myana);
  ~GeneralPlots();
  
  // The functions
  void createHistos();
  void fillHistos(const Float_t& wgt);
  
 private:
  
  // Control variables

  My2DAna*      _ana;
  TFile*       _tfile;
  TDirectory * _tdir;
  std::string  _tdirname;

  // Histos

  TH1F* J1_PT;
  TH1F* J1_ETA;
  TH1F* J1_PHI;
  TH1F* J2_PT;
  TH1F* J2_ETA;
  TH1F* J2_PHI;
  TH1F* P_PT;
  TH1F* P_ETA;
  TH1F* P_PHI;
  TH1F* M_PT;
  TH1F* M_PHI;

  TH1F* Alpha;
  TH1F* Rmpf;
  TH1F* ParalleleMetPt;
  TH1F* PerpendicularMetPt;
  TH1F* DeltaPhi_j1gamma;
  TH1F* DeltaPhi_j2gamma;
  TH1F* DeltaPhi_j1j2;
  TH1F* DeltaR_j1gamma;
  TH1F* DeltaR_j2gamma;
  TH1F* DeltaR_j1j2;
  
  TH1F* J1_QGTAGGER;
  TH1F* J1_BTAGGER;
  TH1F* J2_QGTAGGER;
  TH1F* J2_BTAGGER;

  TH1F* NVERTEX;

  TH1F* P_HADTOWOVEREM;
  TH1F* P_SIGMAIETAIETA;
  TH1F* P_RHO;
  TH1F* P_EREGRES;
  TH1F* P_CHISO;
  TH1F* P_NHISO;
  TH1F* P_PHISO;

};

#endif
