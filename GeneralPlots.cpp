#include "GeneralPlots.h"

using namespace std;

GeneralPlots::GeneralPlots(TFile* file, std::string tdirname, My2DAna* myana)
{
  // Initialize variables
  _tfile    = file;
  _tdirname = tdirname;
  _ana      = myana;
  
  // Create TDirectory containing the plots
  _tfile->cd();
  _tdir = _tfile->mkdir(tdirname.c_str(),tdirname.c_str());
  _tdir->cd();

  // Create histograms
  createHistos();
  _tfile->cd();
}

GeneralPlots::~GeneralPlots()
{
}

void GeneralPlots::createHistos()
{
  
  _tdir->cd();
  
  TH1::SetDefaultSumw2(true);

  // Create Histos
  std::string basename = _tdirname + "_";

  J1_PT  = new TH1F((basename+"J1_PT").c_str(), (basename+"J1_PT").c_str(),300,0.,1500.);
  J1_ETA = new TH1F((basename+"J1_ETA").c_str(),(basename+"J1_ETA").c_str(),100,-5.,5);
  J1_PHI = new TH1F((basename+"J1_PHI").c_str(),(basename+"J1_PHI").c_str(),64,-3.2,3.2);  

  J2_PT  = new TH1F((basename+"J2_PT").c_str(), (basename+"J2_PT").c_str(),300,0.,1500.);
  J2_ETA = new TH1F((basename+"J2_ETA").c_str(),(basename+"J2_ETA").c_str(),100,-5.,5);
  J2_PHI = new TH1F((basename+"J2_PHI").c_str(),(basename+"J2_PHI").c_str(),64,-3.2,3.2);  

  P_PT  = new TH1F((basename+"P_PT").c_str(), (basename+"P_PT").c_str(),300,0.,1500.);
  P_ETA = new TH1F((basename+"P_ETA").c_str(),(basename+"P_ETA").c_str(),100,-5.,5);
  P_PHI = new TH1F((basename+"P_PHI").c_str(),(basename+"P_PHI").c_str(),64,-3.2,3.2);  

  M_PT  = new TH1F((basename+"M_PT").c_str(), (basename+"M_PT").c_str(),300,0.,1500.);
  M_PHI = new TH1F((basename+"M_PHI").c_str(),(basename+"M_PHI").c_str(),64,-3.2,3.2);    

  Alpha = new TH1F((basename+"Alpha").c_str(),(basename+"Alpha").c_str(),50,0.,1.);
  Rmpf  = new TH1F((basename+"Rmpf").c_str(),(basename+"Rmpf").c_str(),50,0,2);

  ParalleleMetPt     = new TH1F((basename+"ParalleleMetPt").c_str(),(basename+"ParalleleMetPt").c_str(),50,-200,200);
  PerpendicularMetPt = new TH1F((basename+"PerpendicularMetPt").c_str(),(basename+"PerpendicularMetPt").c_str(),50,-140,140);

  DeltaPhi_j1gamma = new TH1F((basename+"DeltaPhi_j1gamma").c_str(),(basename+"DeltaPhi_j1gamma").c_str(),200,0,4);
  DeltaPhi_j2gamma = new TH1F((basename+"DeltaPhi_j2gamma").c_str(),(basename+"DeltaPhi_j2gamma").c_str(),200,0,4);
  DeltaPhi_j1j2    = new TH1F((basename+"DeltaPhi_j1j2").c_str(),(basename+"DeltaPhi_j1j2").c_str(),200,0,4);
  DeltaR_j1gamma   = new TH1F((basename+"DeltaR_j1gamma").c_str(),(basename+"DeltaR_j1gamma").c_str(),200,0,8);
  DeltaR_j2gamma   = new TH1F((basename+"DeltaR_j2gamma").c_str(),(basename+"DeltaR_j2gamma").c_str(),200,0,8);
  DeltaR_j1j2      = new TH1F((basename+"DeltaR_j1j2").c_str(),(basename+"DeltaR_j1j2").c_str(),200,0,8);
  
  J1_QGTAGGER = new TH1F((basename+"J1_QGTAGGER").c_str(),(basename+"J1_QGTAGGER").c_str(),50,0,1);
  J1_BTAGGER  = new TH1F((basename+"J1_BTAGGER").c_str(),(basename+"J1_BTAGGER").c_str(),50,0,1);
  J2_QGTAGGER = new TH1F((basename+"J2_QGTAGGER").c_str(),(basename+"J2_QGTAGGER").c_str(),50,0,1);
  J2_BTAGGER  = new TH1F((basename+"J2_BTAGGER").c_str(),(basename+"J2_BTAGGER").c_str(),50,0,1);

  NVERTEX = new TH1F((basename+"NVERTEX").c_str(),(basename+"NVERTEX").c_str(),100,0,100);

  P_HADTOWOVEREM  = new TH1F((basename+"P_HADTOWOVEREM").c_str(),(basename+"P_HADTOWOVEREM").c_str(),120,0,0.06);
  P_SIGMAIETAIETA = new TH1F((basename+"P_SIGMAIETAIETA").c_str(),(basename+"P_SIGMAIETAIETA").c_str(),120,0,0.012);
  P_RHO           = new TH1F((basename+"P_RHO").c_str(),(basename+"P_RHO").c_str(),100,0,50);
  P_EREGRES       = new TH1F((basename+"P_EREGRES").c_str(),(basename+"P_EREGRES").c_str(),200,-0.5,0.5);
  P_CHISO         = new TH1F((basename+"P_CHISO").c_str(),(basename+"P_CHISO").c_str(),250,0,1.);
  P_NHISO         = new TH1F((basename+"P_NHISO").c_str(),(basename+"P_NHISO").c_str(),250,0,25.);
  P_PHISO         = new TH1F((basename+"P_PHISO").c_str(),(basename+"P_PHISO").c_str(),250,0,5.);

  _tfile->cd();
}

void GeneralPlots::fillHistos(const Float_t& wgt)
{
  J1_PT ->Fill(_ana->j1_pt,wgt);
  J1_ETA->Fill(_ana->j1_eta,wgt);
  J1_PHI->Fill(_ana->j1_phi,wgt);
  
  P_PT ->Fill(_ana->p_pt,wgt);
  P_ETA->Fill(_ana->p_eta,wgt);
  P_PHI->Fill(_ana->p_phi,wgt);

  M_PT ->Fill(_ana->met_pt,wgt);
  M_PHI->Fill(_ana->met_phi,wgt);
  
  Rmpf              ->Fill(_ana->Rmpf,wgt);
  Alpha             ->Fill(_ana->alpha,wgt);
  ParalleleMetPt    ->Fill(_ana->metParal,wgt);
  PerpendicularMetPt->Fill(_ana->metPerpend,wgt);
  DeltaPhi_j1gamma  ->Fill(_ana->DeltaPhi_j1gamma,wgt);
  DeltaR_j1gamma    ->Fill(_ana->DeltaR_j1gamma,wgt);
  
  J1_QGTAGGER->Fill(_ana->j1_qg_tag_likelihood,wgt);
  J1_BTAGGER ->Fill(_ana->j1_btag_csv,wgt);

  if ( _ana->j2_is_present == 1 ) {
    J2_PT->Fill(_ana->j2_pt,wgt);
    J2_ETA->Fill(_ana->j2_eta,wgt);
    J2_PHI->Fill(_ana->j2_phi,wgt);
    
    DeltaPhi_j2gamma  ->Fill(_ana->DeltaPhi_j2gamma,wgt);
    DeltaPhi_j1j2     ->Fill(_ana->DeltaPhi_j1j2,wgt);
    DeltaR_j2gamma    ->Fill(_ana->DeltaR_j2gamma,wgt);
    DeltaR_j1j2       ->Fill(_ana->DeltaR_j1j2,wgt);

    J2_QGTAGGER->Fill(_ana->j2_qg_tag_likelihood,wgt);
    J2_BTAGGER ->Fill(_ana->j2_btag_csv,wgt);
  }

  NVERTEX->Fill((Float_t)_ana->nvertex,wgt);

  P_HADTOWOVEREM->Fill(_ana->p_hadTowOverEm,wgt);
  P_SIGMAIETAIETA->Fill(_ana->p_sigmaIetaIeta,wgt);
  P_RHO->Fill(_ana->p_rho,wgt);
  P_EREGRES->Fill((_ana->p_regressionEnergy-_ana->p_originalEnergy)/_ana->p_originalEnergy,wgt);
  P_CHISO->Fill(_ana->p_chargedHadronsIsolation,wgt);
  P_NHISO->Fill(_ana->p_neutralHadronsIsolation,wgt);
  P_PHISO->Fill(_ana->p_photonIsolation,wgt);

}
