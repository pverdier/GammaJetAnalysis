#define My2DAna_cpp

#include <memory>
#include "My2DAna.h"

#include "GeneralPlots.h"
#include "prescaleParser.h"

#include "binning.h"
#include "common.h"
#include "ptBinning.h"
#include "alphaBinning.h"
#include "QGSyst.h"

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h>
#include <iostream>
#include <iomanip>
#include <set>
#include <map>
#include "TRandom3.h"
#include "TVector3.h"

using namespace std;

My2DAna::My2DAna(Bool_t doMC, Bool_t doRAW)
  :RootTupleDefs(doMC,doRAW)
{
   _isMC     = false;
   _debug    = false;
   _rootName = "output.root";
   _doSkim   = false;
   _nevt     = -1; 
}

My2DAna::~My2DAna()
{
}

void My2DAna::Loop()
{
  cout << " => Running on MC     ? " << boolalpha << _isMC   << endl;
  cout << " => Read RAW info     ? " << boolalpha << _doRAW  << endl;
  cout << " => Read MC  info     ? " << boolalpha << _doMC   << endl;

  _newfile = new TFile(_rootName.c_str(),"RECREATE");

  //================================================================================================
  // HISTOGRAMS
  //================================================================================================

  TH1::SetDefaultSumw2(true);

  TH1F* ICUT = new TH1F("ICUT","ICUT",100,0.,100.);

  GeneralPlots InclusivePlots(_newfile,"Inclusive",this);
  GeneralPlots InclusivePer1(_newfile,"Inclusive1",this);
  GeneralPlots InclusivePer2(_newfile,"Inclusive2",this);
  GeneralPlots InclusivePer3(_newfile,"Inclusive3",this);

  GeneralPlots ptBin1Plots(_newfile,"ptbin1",this);
  GeneralPlots ptBin2Plots(_newfile,"ptbin2",this);
  GeneralPlots ptBin3Plots(_newfile,"ptbin3",this);
  GeneralPlots ptBin4Plots(_newfile,"ptbin4",this);

  // Data/MC comparisons in zones

  GeneralPlots zone1QPlots(_newfile,getZoneBinName(0),this);
  GeneralPlots zone2GPlots(_newfile,getZoneBinName(1),this);
  GeneralPlots zone3CPlots(_newfile,getZoneBinName(2),this);
  GeneralPlots zone4BPlots(_newfile,getZoneBinName(3),this);
  GeneralPlots zone5QGPlots(_newfile,getZoneBinName(4),this);
  GeneralPlots zone6LCPlots(_newfile,getZoneBinName(5),this);
  GeneralPlots notInZonePlots(_newfile,"notInZone",this);

  // MC plots
  
  TH1F* MC_RTRUE = new TH1F("MC_RTRUE","MC_RTRUE",60,-1.,2.);

  TH1F* MC_DPHI_J1J1G = new TH1F("MC_DPHI_J1J1G","MC_DPHI_J1J1G",200,0,4);
  TH1F* MC_DR_J1J1G   = new TH1F("MC_DR_J1J1G","MC_DR_J1J1G",200,0,8);

  //-------------------------------------------------------------------------------------------------
  // histograms definition
  //-------------------------------------------------------------------------------------------------

  // For MC simulation
  QGSyst qgsyst;
  qgsyst.ReadDatabase("SystDatabase.txt");
  qgsyst.SetTagger("QGLHisto");

  ptBinning myPtBinning;
  int isFor2DTagging = 1;
  
  ptBinning my2DTaggingPtBinning(isFor2DTagging);
  int n2DTaggingPtBins = my2DTaggingPtBinning.getSize();
	
  //usefull variables
  Double_t xlow = getHistoXlow(); // = 0.
  Double_t xup = getHistoXup();   // = 2.
  //Double_t    binrange = 0.1;
  //Int_t    nbinsx = (xup - xlow)/binrange;
  Int_t    nbinsx = getHistoNbinsx(); // = 30
  
  Double_t xlow2D = 0;
  Double_t xup2D = 1;
  Int_t    nbinsx2D = 20;
  
  Double_t ylow2D = 0;
  Double_t yup2D = 1;
  Int_t    nbinsy2D = 20;
  
  int nzones = getZoneNumber();
  int nflavours = getFlavourNumber(); //uds, g, c, b, noMatched, all

  //-------------------------------
  // Rmpf histos for data and MC 
  //-------------------------------

  //vectors for 2D tagging study
  //responses per zone per pt

  _newfile->cd();
  TDirectory *response_Zone_PtDir = _newfile->mkdir("response_Zone_Pt","response_Zone_Pt");
  TDirectory *Rmpf_Zone_PtDir     = response_Zone_PtDir->mkdir("Rmpf","Rmpf");
  Rmpf_Zone_PtDir->cd();
  vector<vector<TH1F*> > vRmpf_ZonePt = buildZonePtVectorH1(my2DTaggingPtBinning,"Rmpf",nbinsx,xlow,xup) ;
  
  //-------------------------------
  // Histos for data only
  //-------------------------------

  vector<TH1F*> vQGL_Pt;
  vector<TH1F*> vCSV_Pt;

  _newfile->cd();
  TDirectory *tagger_PtDir = _newfile->mkdir("tagger_Pt","tagger_Pt");

  TDirectory *CSV_PtDir = tagger_PtDir->mkdir("CSV","CSV");
  CSV_PtDir->cd();
  vCSV_Pt = buildPtVectorH1(my2DTaggingPtBinning,"CSV",nbinsx2D,xlow2D,xup2D) ;

  TDirectory *QGL_Flavour_PtDir = tagger_PtDir->mkdir("QGL","QGL");
  QGL_Flavour_PtDir->cd();  
  vQGL_Pt = buildPtVectorH1(my2DTaggingPtBinning,"QGL",nbinsx2D,xlow2D,xup2D) ;

  _newfile->cd();

  //-------------------------------
  // Histos for MC only
  //-------------------------------
 
  vector<TMatrixF> v4x4MatrixPt;
  vector<TMatrixF> v6x4MatrixPt;

  //vectors for 2D tagging study
  // gammapt per flavour per zone
  std::vector<std::vector<TH1F*> > vGammaPt_ZoneFlavour;
  std::vector<std::vector<TH1F*> > vFirstJetPt_ZoneFlavour;
  //flavour fractions per pt
  vector<TH1F*> vFractionHisto_Pt;
  //Rtrue per zone per pt
  vector<vector<TH1F*> > vRtrue_ZonePt;
  //QG-likelihood per flavour per pt
  vector<vector<TH1F*> > vQGL_FlavourPt;
  //Btag-CSV per flavour per pt
  vector<vector<TH1F*> > vCSV_FlavourPt;
  //responses per flavour per pt
  vector<vector<TH1F*> > vRmpf_FlavourPt;
  vector<vector<TH1F*> > vRtrue_FlavourPt;
  //responses per flavour per pt when we are in one of the 2D tagging zones
  vector<vector<TH1F*> > vRmpf_in2DTaggingZone_FlavourPt;
  vector<vector<TH1F*> > vRtrue_in2DTaggingZone_FlavourPt;
  //Gammapt per flavour 
  vector<TH1F*> vGammapt_Flavour;
  //responses per zone per flavour per pt
  vector<vector<vector<TH1F*> > > vRmpf_ZoneFlavourPt;
  vector<vector<vector<TH1F*> > > vRtrue_ZoneFlavourPt;
  //2D tagging plans per flavour per pt
  vector<vector<TH2F*> > v2DTaggingPlan_FlavourPt;
  vector<vector<TH2F*> > v2DTaggingPlan_FlavourPt_divided;
  //flavour number per zone per flavour per pt
  vector<vector<vector<float> > > vFlavourNumber;
  //flavour number per zone per flavour per pt
  vector<vector<vector<float> > > vFlavourFraction;

  if ( _isMC ) {
    v4x4MatrixPt = buildSquareMatrixPtVector(my2DTaggingPtBinning);
    v6x4MatrixPt = buildMatrixPtVector(my2DTaggingPtBinning);
    
    //flavour fractions per pt
    vFractionHisto_Pt = buildPtVectorH1(my2DTaggingPtBinning,"FractionHisto",nflavours-1,0,nflavours-1) ;

    //flavour number per zone per flavour per pt
    vFlavourNumber = build_fraction_ZoneFlavourPtVector(my2DTaggingPtBinning) ;
    //flavour number per zone per flavour per pt
    vFlavourFraction = build_fraction_ZoneFlavourPtVector(my2DTaggingPtBinning) ;

    //-----------------

    //vectors for 2D tagging study
    // gammapt per flavour per zone
    TDirectory *gammaPtDir = _newfile->mkdir("gammaPtPerZonePerFlavour","gammaPtPerZonePerFlavour");
    gammaPtDir->cd();
    vGammaPt_ZoneFlavour = buildZoneFlavourVectorH1("Gammapt",30,0,800);
    _newfile->cd();
    
    //-----------------

    TDirectory *firstJetPtPtDir = _newfile->mkdir("firstJetPtPerZonePerFlavour","firstJetPtPerZonePerFlavour");
    firstJetPtPtDir->cd();
    vFirstJetPt_ZoneFlavour = buildZoneFlavourVectorH1("FirstJetPt",160,0,800);
    _newfile->cd();

    //-----------------

    //Rtrue per zone per pt
    TDirectory *Rtrue_Zone_PtDir = response_Zone_PtDir->mkdir("Rtrue","Rtrue");
    Rtrue_Zone_PtDir->cd();
    vRtrue_ZonePt = buildZonePtVectorH1(my2DTaggingPtBinning,"Rtrue",nbinsx,xlow,xup) ;
    _newfile->cd();

    //-----------------

    TDirectory *tagger_Flavour_PtDir = _newfile->mkdir("tagger_Flavour_Pt","tagger_Flavour_Pt");
    //QG-likelihood per flavour per pt
    TDirectory *CSV_Flavour_PtDir = tagger_Flavour_PtDir->mkdir("CSV","CSV");
    CSV_Flavour_PtDir->cd();
    vCSV_FlavourPt = buildFlavourPtVectorH1(my2DTaggingPtBinning,"CSV",nbinsx2D,xlow2D,xup2D) ;
    //Btag-CSV per flavour per pt
    TDirectory *QGL_Flavour_PtDir = tagger_Flavour_PtDir->mkdir("QGL","QGL");
    QGL_Flavour_PtDir->cd();
    vQGL_FlavourPt = buildFlavourPtVectorH1(my2DTaggingPtBinning,"QGL",nbinsx2D,xlow2D,xup2D) ;
    _newfile->cd();
    
    //-----------------

    //responses per flavour per pt when we are in one of the 2D tagging zones
    TDirectory *response_Flavour_PtDir = _newfile->mkdir("response_Flavour_Pt","response_Flavour_Pt");
    TDirectory *Rmpf_Flavour_PtDir = response_Flavour_PtDir->mkdir("Rmpf","Rmpf");
    Rmpf_Flavour_PtDir->cd();

    vRmpf_FlavourPt                 = buildFlavourPtVectorH1(my2DTaggingPtBinning,"Rmpf",nbinsx,xlow,xup) ;
    vRmpf_in2DTaggingZone_FlavourPt = buildFlavourPtVectorH1(my2DTaggingPtBinning,"Rmpf_in2DTaggingZone",nbinsx,xlow,xup) ;
    TDirectory *Rtrue_Flavour_PtDir = response_Flavour_PtDir->mkdir("Rtrue","Rtrue");
    Rtrue_Flavour_PtDir->cd();
    vRtrue_FlavourPt                 = buildFlavourPtVectorH1(my2DTaggingPtBinning,"Rtrue",nbinsx,xlow,xup) ;
    vRtrue_in2DTaggingZone_FlavourPt = buildFlavourPtVectorH1(my2DTaggingPtBinning,"Rtrue_in2DTaggingZone",nbinsx,xlow,xup) ;
    _newfile->cd();

    //-----------------

    //Gammapt per flavour 
    TDirectory *gammapt_FlavourDir = _newfile->mkdir("gammapt_Flavour","gammapt_Flavour");
    gammapt_FlavourDir->cd();
    vGammapt_Flavour = buildFlavourVectorH1("Gammapt",30,0,800);
    _newfile->cd();

    //-----------------

    //responses per zone per flavour per pt
    TDirectory *response_Zone_Flavour_PtDir = _newfile->mkdir("response_Zone_Flavour_Pt","response_Zone_Flavour_Pt");
    TDirectory *Rmpf_Zone_Flavour_PtDir = response_Zone_Flavour_PtDir->mkdir("Rmpf","Rmpf");
    Rmpf_Zone_Flavour_PtDir->cd();
    vRmpf_ZoneFlavourPt = buildZoneFlavourPtVectorH1(my2DTaggingPtBinning,"Rmpf",nbinsx,xlow,xup) ;
    TDirectory *Rtrue_Zone_Flavour_PtDir = response_Zone_Flavour_PtDir->mkdir("Rtrue","Rtrue");
    Rtrue_Zone_Flavour_PtDir->cd();
    vRtrue_ZoneFlavourPt = buildZoneFlavourPtVectorH1(my2DTaggingPtBinning,"Rtrue",nbinsx,xlow,xup) ;
    _newfile->cd();

    //-----------------

    //2D tagging plans per flavour per pt
    TDirectory *plan2DTaggingDir = _newfile->mkdir("plan2DTagging","plan2DTagging");
    plan2DTaggingDir->cd();
    v2DTaggingPlan_FlavourPt = buildFlavourPtVectorH2(my2DTaggingPtBinning,"2DTaggingPlan",nbinsx2D,xlow2D,xup2D,nbinsy2D,ylow2D,yup2D) ;
    v2DTaggingPlan_FlavourPt_divided = buildFlavourPtVectorH2(my2DTaggingPtBinning,"2DTaggingPlan_divided",nbinsx2D,xlow2D,xup2D,nbinsy2D,ylow2D,yup2D) ;
    _newfile->cd();

  }

  //================================================================================================
  // Clone the tree
  //================================================================================================


  if ( _doSkim ) {
    GetEntry(0);

    _new_first_jetChain  = first_jetChain->GetTree()->CloneTree(0);
    _new_second_jetChain = second_jetChain->GetTree()->CloneTree(0);
    _new_photonChain     = photonChain->GetTree()->CloneTree(0); 
    _new_metChain        = metChain->GetTree()->CloneTree(0);
    _new_electronsChain  = electronsChain->GetTree()->CloneTree(0);
    _new_muonsChain      = muonsChain->GetTree()->CloneTree(0);
    _new_miscChain       = miscChain->GetTree()->CloneTree(0);
    _new_rhoChain        = rhoChain->GetTree()->CloneTree(0);
    
    // RAW
    if ( _doRAW ) { 
      _new_first_jet_rawChain  = first_jet_rawChain->GetTree()->CloneTree(0);
      _new_second_jet_rawChain = second_jet_rawChain->GetTree()->CloneTree(0);
      _new_met_rawChain        = met_rawChain->GetTree()->CloneTree(0);
    }
    // MC only
    if ( _doMC ) {
      _new_first_jet_genChain  = first_jet_genChain->GetTree()->CloneTree(0);
      _new_second_jet_genChain = second_jet_genChain->GetTree()->CloneTree(0);
      _new_met_genChain        = met_genChain->GetTree()->CloneTree(0);
      _new_photon_genChain     = photon_genChain->GetTree()->CloneTree(0);
    }

  }

  //================================================================================================
  // Start the Loop
  //================================================================================================

  // INIT :
  //-------

  TRandom3 rnd(1234);

  int binPt;//bin en pt
  int bin2DTaggingPt;//gros bins en pt pour l etude 2D tagging
  int binZone;// 2D tagging zone bin
  int binFlavour;
  string type;

  if (first_jetChain == 0) return;

  Int_t nentries = Int_t(first_jetChain->GetEntriesFast());
  if ( _nevt < 0 ) _nevt = (int)nentries; //nevt initialised to -1 unless -num option is requested  

  int nwrite = 0;
  int nselected = 0;
  
  TString cutName[100]; for (int i=0;i<100;++i) cutName[i] = "";

  TString currentfile = "";
  Int_t nbytes = 0, nb = 0;

  // The LOOP :
  //-----------

  for (Int_t jentry=0; jentry<_nevt;jentry++) { //Starting events loop
    nb = GetEntry(jentry);   nbytes += nb;
    
    if (jentry % 10000 == 0) 
      cout << "Loop: processing event  " << jentry <<endl;     

    TString thisfile = first_jetChain->GetCurrentFile()->GetName();
    if ( thisfile != currentfile ) {
      currentfile = thisfile;
      cout << " => Start processing : " << currentfile << endl;
    }
    
    Float_t WEIGHT = event_weight;

    int icut = 0;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Starting"; ++icut;

    //----------------------------------------------------------------------------
    // Apply MC reweighting
    //----------------------------------------------------------------------------

    if ( !_isMC ) {
      Float_t gwgt = 1.;
      if      ( p_pt > -5 && p_pt  < 0 ) gwgt = 1 ;
      else if ( p_pt > 0 && p_pt  < 5 ) gwgt = 1 ;
      else if ( p_pt > 5 && p_pt  < 10 ) gwgt = 1 ;
      else if ( p_pt > 10 && p_pt  < 15 ) gwgt = 1 ;
      else if ( p_pt > 15 && p_pt  < 20 ) gwgt = 1 ;
      else if ( p_pt > 20 && p_pt  < 25 ) gwgt = 1 ;
      else if ( p_pt > 25 && p_pt  < 30 ) gwgt = 1 ;
      else if ( p_pt > 30 && p_pt  < 35 ) gwgt = 1 ;
      else if ( p_pt > 35 && p_pt  < 40 ) gwgt = 1 ;
      else if ( p_pt > 40 && p_pt  < 45 ) gwgt = 1 ;
      else if ( p_pt > 45 && p_pt  < 50 ) gwgt = 1 ;
      else if ( p_pt > 50 && p_pt  < 55 ) gwgt = 1 ;
      else if ( p_pt > 55 && p_pt  < 60 ) gwgt = 1 ;
      else if ( p_pt > 60 && p_pt  < 65 ) gwgt = 0.865177 ;
      else if ( p_pt > 65 && p_pt  < 70 ) gwgt = 0.917209 ;
      else if ( p_pt > 70 && p_pt  < 75 ) gwgt = 0.940264 ;
      else if ( p_pt > 75 && p_pt  < 80 ) gwgt = 0.909658 ;
      else if ( p_pt > 80 && p_pt  < 85 ) gwgt = 0.900368 ;
      else if ( p_pt > 85 && p_pt  < 90 ) gwgt = 1.02279 ;
      else if ( p_pt > 90 && p_pt  < 95 ) gwgt = 1.0597 ;
      else if ( p_pt > 95 && p_pt  < 100 ) gwgt = 1.04088 ;
      else if ( p_pt > 100 && p_pt  < 110 ) gwgt = 0.926821 ;
      else if ( p_pt > 110 && p_pt  < 120 ) gwgt = 1.00239 ;
      else if ( p_pt > 120 && p_pt  < 130 ) gwgt = 1.08018 ;
      else if ( p_pt > 130 && p_pt  < 140 ) gwgt = 1.01694 ;
      else if ( p_pt > 140 && p_pt  < 150 ) gwgt = 1.04814 ;
      else if ( p_pt > 150 && p_pt  < 160 ) gwgt = 1.08887 ;
      else if ( p_pt > 160 && p_pt  < 170 ) gwgt = 1.11745 ;
      else if ( p_pt > 170 && p_pt  < 180 ) gwgt = 1.09598 ;
      else if ( p_pt > 180 && p_pt  < 190 ) gwgt = 1.12893 ;
      else if ( p_pt > 190 && p_pt  < 200 ) gwgt = 1.13615 ;
      else if ( p_pt > 200 && p_pt  < 210 ) gwgt = 1.11277 ;
      else if ( p_pt > 210 && p_pt  < 220 ) gwgt = 1.15021 ;
      else if ( p_pt > 220 && p_pt  < 230 ) gwgt = 1.15836 ;
      else if ( p_pt > 230 && p_pt  < 240 ) gwgt = 1.14901 ;
      else if ( p_pt > 240 && p_pt  < 250 ) gwgt = 1.19403 ;
      else if ( p_pt > 250 && p_pt  < 260 ) gwgt = 1.1351 ;
      else if ( p_pt > 260 && p_pt  < 270 ) gwgt = 1.19648 ;
      else if ( p_pt > 270 && p_pt  < 280 ) gwgt = 1.14765 ;
      else if ( p_pt > 280 && p_pt  < 290 ) gwgt = 1.19637 ;
      else if ( p_pt > 290 && p_pt  < 300 ) gwgt = 1.17638 ;
      else if ( p_pt > 300 && p_pt  < 310 ) gwgt = 1.15385 ;
      else if ( p_pt > 310 && p_pt  < 320 ) gwgt = 1.18764 ;
      else if ( p_pt > 320 && p_pt  < 330 ) gwgt = 1.20748 ;
      else if ( p_pt > 330 && p_pt  < 340 ) gwgt = 1.15163 ;
      else if ( p_pt > 340 && p_pt  < 350 ) gwgt = 1.15706 ;
      else if ( p_pt > 350 && p_pt  < 360 ) gwgt = 1.19518 ;
      else if ( p_pt > 360 && p_pt  < 370 ) gwgt = 1.17887 ;
      else if ( p_pt > 370 && p_pt  < 380 ) gwgt = 1.15905 ;
      else if ( p_pt > 380 && p_pt  < 390 ) gwgt = 1.20251 ;
      else if ( p_pt > 390 && p_pt  < 400 ) gwgt = 1.201 ;
      else if ( p_pt > 400 && p_pt  < 500 ) gwgt = 1.23645 ;
      else if ( p_pt > 500 && p_pt  < 600 ) gwgt = 1.24389 ;
      else if ( p_pt > 600 && p_pt  < 700 ) gwgt = 1.19998 ;
      else if ( p_pt > 700 && p_pt  < 800 ) gwgt = 1.32009 ;
      else if ( p_pt > 800 && p_pt  < 1000) gwgt = 0.951556 ;
      else if ( p_pt > 1000               ) gwgt = 1.92875 ;
      //WEIGHT *= gwgt;
    }

    //----------------------------------------------------------------------------
    // Trigger
    //----------------------------------------------------------------------------

    bool pass_PER1_Photon30_CaloIdVL_IsoL = false;
    bool pass_PER1_Photon50_CaloIdVL_IsoL = false;
    bool pass_PER1_Photon90_CaloIdVL_IsoL = false;
    bool pass_PER1_Photon135 = false;
    
    bool pass_PER2_Photon30_CaloIdVL_IsoL = false;
    bool pass_PER2_Photon50_CaloIdVL_IsoL = false;
    bool pass_PER2_Photon90_CaloIdVL_IsoL = false;
    bool pass_PER2_Photon135 = false;
    bool pass_PER2_Photon150 = false;
    
    bool pass_PER3_Photon30_CaloIdVL = false;
    bool pass_PER3_Photon50_CaloIdVL_IsoL = false;
    bool pass_PER3_Photon90_CaloIdVL = false;
    bool pass_PER3_Photon135 = false;
    
    for ( unsigned int i=0; i<trigger_names->size(); ++i) {
      if ( !trigger_results->at(i) ) continue;
      TString ThistrigName= (TString) trigger_names->at(i);
      if ( !_isMC ) {
	if ( run >= 190450 && run <= 193832 ) {
	  if ( ThistrigName.Contains("HLT_Photon30_CaloIdVL_IsoL_v") && p_pt >=  40 ) { pass_PER1_Photon30_CaloIdVL_IsoL = true; }
	  if ( ThistrigName.Contains("HLT_Photon50_CaloIdVL_IsoL_v") && p_pt >=  60 ) { pass_PER1_Photon50_CaloIdVL_IsoL = true; }
	  if ( ThistrigName.Contains("HLT_Photon90_CaloIdVL_IsoL_v") && p_pt >= 100 ) { pass_PER1_Photon90_CaloIdVL_IsoL = true; }
	  if ( ThistrigName.Contains("HLT_Photon135_v")              && p_pt >= 155 ) { pass_PER1_Photon135 = true; }
	} else if ( run >= 193833 && run <= 199608 ) {
	  if ( ThistrigName.Contains("HLT_Photon30_CaloIdVL_IsoL_v") && p_pt >=  40 ) { pass_PER2_Photon30_CaloIdVL_IsoL = true; }
	  if ( ThistrigName.Contains("HLT_Photon50_CaloIdVL_IsoL_v") && p_pt >=  60 ) { pass_PER2_Photon50_CaloIdVL_IsoL = true; }
	  if ( ThistrigName.Contains("HLT_Photon90_CaloIdVL_IsoL_v") && p_pt >= 100 ) { pass_PER2_Photon90_CaloIdVL_IsoL = true; }
	  if ( ThistrigName.Contains("HLT_Photon135_v")              && p_pt >= 155 ) { pass_PER2_Photon135 = true; }
	  if ( ThistrigName.Contains("HLT_Photon150_v")              && p_pt >= 180 ) { pass_PER2_Photon150 = true; }
	} else if ( run >= 199609 && run <= 208686 ) {
	  if ( ThistrigName.Contains("HLT_Photon30_CaloIdVL_v")      && p_pt >=  40) { pass_PER3_Photon30_CaloIdVL = true; }
	  if ( ThistrigName.Contains("HLT_Photon50_CaloIdVL_IsoL_v") && p_pt >=  60 ) { pass_PER3_Photon50_CaloIdVL_IsoL = true; }
	  if ( ThistrigName.Contains("HLT_Photon90_CaloIdVL_v")      && p_pt >= 100 ) { pass_PER3_Photon90_CaloIdVL = true; }
	  if ( ThistrigName.Contains("HLT_Photon135_v")              && p_pt >= 155 ) { pass_PER3_Photon135 = true; }
	}
      } else {
	if ( ThistrigName.Contains("HLT_Photon30_CaloIdVL_IsoL_v") && p_pt >=  40 ) { pass_PER1_Photon30_CaloIdVL_IsoL = true; pass_PER2_Photon30_CaloIdVL_IsoL = true; }
	if ( ThistrigName.Contains("HLT_Photon30_CaloIdVL_v")      && p_pt >=  40 ) { pass_PER3_Photon30_CaloIdVL = true; }
	if ( ThistrigName.Contains("HLT_Photon50_CaloIdVL_IsoL_v") && p_pt >=  60 ) { pass_PER1_Photon50_CaloIdVL_IsoL = true; pass_PER2_Photon50_CaloIdVL_IsoL = true; pass_PER3_Photon50_CaloIdVL_IsoL = true; }
	if ( ThistrigName.Contains("HLT_Photon90_CaloIdVL_IsoL_v") && p_pt >= 100 ) { pass_PER1_Photon90_CaloIdVL_IsoL = true; pass_PER2_Photon90_CaloIdVL_IsoL = true; }
	if ( ThistrigName.Contains("HLT_Photon90_CaloIdVL_v")      && p_pt >= 100 ) { pass_PER3_Photon90_CaloIdVL = true; }
	if ( ThistrigName.Contains("HLT_Photon135_v")              && p_pt >= 155 ) { pass_PER1_Photon135 = true; pass_PER2_Photon135 = true; pass_PER3_Photon135 = true; }
	if ( ThistrigName.Contains("HLT_Photon150_v")              && p_pt >= 180 ) { pass_PER2_Photon150 = true; }
      }
    } // end loop over triggers
    
    // Apply trigger to MC :
    //----------------------

    bool mcPassTriger = true;
    if ( _isMC ) {

      // Determine luminosity based on the integrated lumi
      // per1: 876.165   -> 4.44505606006798185e-02
      // per2: 6145.0    -> 3.56205418294353426e-01
      // per3: 12690     -> 1.
      // total = 19711.
      
      unsigned int per = 0;
      Double_t r = rnd.Uniform();
      if      ( r < 4.44505606006798185e-02                                 ) per = 1;
      else if ( r >= 4.44505606006798185e-02 && r < 3.56205418294353426e-01 ) per = 2;
      else if ( r >= 3.56205418294353426e-01                                ) per = 3;

      if ( per < 1 || per > 3 ) {
	cout << "WARNING !! Periode not found !!" << endl;
	continue;
      }
      
      if ( per == 1 ) {
	if ( p_pt >=  40 && p_pt <  60 && !pass_PER1_Photon30_CaloIdVL_IsoL ) mcPassTriger = false;
	if ( p_pt >=  60 && p_pt < 100 && !pass_PER1_Photon50_CaloIdVL_IsoL ) mcPassTriger = false;
	if ( p_pt >= 100 && p_pt < 155 && !pass_PER1_Photon90_CaloIdVL_IsoL ) mcPassTriger = false;
	if ( p_pt >= 155               && !pass_PER1_Photon135              ) mcPassTriger = false;
      } else if (per == 2 ) {
	if ( p_pt >=  40 && p_pt <  60 && !pass_PER2_Photon30_CaloIdVL_IsoL ) mcPassTriger = false;
	if ( p_pt >=  60 && p_pt < 100 && !pass_PER2_Photon50_CaloIdVL_IsoL ) mcPassTriger = false;
	if ( p_pt >= 100 && p_pt < 155 && !pass_PER2_Photon90_CaloIdVL_IsoL ) mcPassTriger = false;
	if ( p_pt >= 155 && p_pt < 180 && !pass_PER2_Photon135              ) mcPassTriger = false;
	if ( p_pt >= 180               && !pass_PER2_Photon150              ) mcPassTriger = false;
      } else if (per == 3 ) {
	if ( p_pt >=  40 && p_pt <  60 && !pass_PER3_Photon30_CaloIdVL      ) mcPassTriger = false;
	if ( p_pt >=  60 && p_pt < 100 && !pass_PER3_Photon50_CaloIdVL_IsoL ) mcPassTriger = false;
	if ( p_pt >= 100 && p_pt < 155 && !pass_PER3_Photon90_CaloIdVL      ) mcPassTriger = false;
	if ( p_pt >= 155               && !pass_PER3_Photon135              ) mcPassTriger = false;
      }

    } // endif _isMC

    if (!mcPassTriger) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "MC triggers"; ++icut;

    //----------------------------------------------------------------------------
    // Selection cuts
    //----------------------------------------------------------------------------
    
    // Redefine j2_is_present with an eta cut
    if ( j2_is_present == 1 ) {
      if ( j2_pt        < 10. ) j2_is_present = 0;
      if ( fabs(j2_eta) > 2.4 ) j2_is_present = 0; // PV
    }

    // Compute Rmpf and alpha
    Rmpf = 1 + (p_px*met_px + p_py*met_py)/(pow(p_pt,2));
    alpha = 0.;
    if ( j2_is_present == 1 ) alpha = (j2_pt)/(p_pt);

    // Angles vs jet 1 :
    //------------------

    //DeltaPhi between the 1st jet and the gamma calculation
    DeltaPhi_j1gamma = TMath::Abs((p_phi) - (j1_phi));
    if(DeltaPhi_j1gamma>TMath::Pi())
      DeltaPhi_j1gamma = 2*TMath::Pi()-DeltaPhi_j1gamma;
    
    //DeltaPhi between the first jet and the met
    DeltaPhi_j1met = TMath::Abs((met_phi) - (j1_phi));

    //DeltaEta between the first jet and the gamma calculation
    DeltaEta_j1gamma = TMath::Abs((p_eta) - (j1_eta));

    //DeltaR between the first jet and the gamma calculation
    DeltaR_j1gamma = sqrt ( pow(DeltaEta_j1gamma,2) + pow(DeltaPhi_j1gamma,2) );

    // Angles vs jet 2 :
    //------------------

    DeltaPhi_j2gamma = -10.;
    DeltaPhi_j1j2    = -10.;
    DeltaEta_j2gamma = -10.;
    DeltaEta_j1j2    = -10.;
    DeltaR_j2gamma   = -10.;
    DeltaR_j1j2      = -10.;

    if ( j2_is_present == 1) {

      //DeltaPhi between the 2nd jet and the gamma calculation
      DeltaPhi_j2gamma = TMath::Abs((p_phi) - (j2_phi));
      if(DeltaPhi_j2gamma>TMath::Pi())
	DeltaPhi_j2gamma = 2*TMath::Pi()-DeltaPhi_j2gamma;
      
      //DeltaPhi between the first jet and the second jet calculation
      DeltaPhi_j1j2 = TMath::Abs((j2_phi) - (j1_phi));
      if(DeltaPhi_j1j2>TMath::Pi())
	DeltaPhi_j1j2 = 2*TMath::Pi()-DeltaPhi_j1j2;
      
      //DeltaEta between the second jet and the gamma calculation
      DeltaEta_j2gamma = TMath::Abs((p_eta) - (j2_eta));
      
      //DeltaEta between the first jet and the second jet calculation
      DeltaEta_j1j2 = TMath::Abs((j2_eta) - (j1_eta));
      
      //DeltaR between the second jet and the gamma calculation
      DeltaR_j2gamma = sqrt ( pow(DeltaEta_j2gamma,2) + pow(DeltaPhi_j2gamma,2) );
      
      //DeltaR between the first jet and the second jet calculation
      DeltaR_j1j2 = sqrt ( pow(DeltaEta_j1j2,2) + pow(DeltaPhi_j1j2,2) );
      
    }

    // Met composants vs J1 :
    //-----------------------

    metParal   = met_pt * TMath::Cos(DeltaPhi_j1met);
    metPerpend = met_pt * TMath::Sin(DeltaPhi_j1met);

    // MC variables :
    //---------------

    Rtrue      = -1.;
    Dphi_j1j1g = -1.;
    Deta_j1j1g = -1.;
    DR_j1j1g   = -1.;
    
    if ( _isMC ) {
      if ( j1g_is_present == 1 ) {
	
	Rtrue = j1_pt/j1g_pt;
	
	Dphi_j1j1g = TMath::Abs((j1_phi) - (j1g_phi));
	if ( Dphi_j1j1g>TMath::Pi() ) Dphi_j1j1g = 2*TMath::Pi()-Dphi_j1j1g;
	Deta_j1j1g = TMath::Abs(j1_eta - j1g_eta);
	DR_j1j1g   = sqrt(pow(Dphi_j1j1g,2)+pow(Deta_j1j1g,2));

      }
    }
    
    // Selection cuts :
    //-----------------

    if ( !p_is_present ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "At least one photon"; ++icut;

    /*
    if ( p_hadTowOverEm > 0.05 ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Photon: hadTowOverEm < 0.05"; ++icut;

    if ( p_sigmaIetaIeta > 0.011 ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Photon: sigmaIetaIeta < 0.011"; ++icut;

    if ( p_hasMatchedPromptElectron ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Photon: !hasMatchedPromptElectron"; ++icut;

    if ( p_chargedHadronsIsolation > 0.7 ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Photon: chargedHadronsIsolation < 0.7"; ++icut;
    
    if ( p_neutralHadronsIsolation > 0.4 + 0.04*p_pt ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Photon: neutralHadronsIsolation < 0.4 + 0.04*p_pt"; ++icut;

    if ( p_photonIsolation > 0.5 + 0.005*p_pt ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Photon: photonIsolation < 0.5 + 0.005*p_pt"; ++icut;
    */

    if ( !j1_is_present || fabs(j1_pt) < 12.) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "First Jet Pt > 12"; ++icut;
    
    if ( DeltaPhi_j1gamma < 2.8 ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "DeltaPhi(j1,gamma) >2.8"; ++icut;

    if(p_has_pixel_seed == true) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Photon with pixel seed"; ++icut;
    
    if ( fabs(j1_eta) > 1.3 ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "First jet |eta|<1.3"; ++icut;

    if ( m_n > 0 ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Isolated Muon veto"; ++icut;
 
    if ( e_n > 0 ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Isolated Electron veto"; ++icut;

    bool secondJetOK = !j2_is_present || (j2_pt < 10 || j2_pt < 0.1 * p_pt);
    if ( !secondJetOK) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Alpha<0.1 OR 2nd jet pT<10 GeV"; ++icut;

    if ( p_pt < 60. ) continue;
    ICUT->Fill((Float_t)icut,WEIGHT); cutName[icut] = "Photon > 60 GeV"; ++icut;

    //----------------------------------------------------------------------------
    // Data/MC treatment
    //----------------------------------------------------------------------------
    
    binPt = myPtBinning.getPtBin(p_pt);
    
    if(binPt == -1) continue;
    ICUT->Fill((Double_t)icut,WEIGHT); cutName[icut] = "Photon Pt bin not found"; ++icut;

    bin2DTaggingPt = my2DTaggingPtBinning.getPtBin(p_pt);
    
    // Specific to MC : QGL smearing and bin flavour
    if ( _isMC ) {
      if      (TMath::Abs(j1g_parton_pdg_id) == 21) type = "gluon";
      else if (TMath::Abs(j1g_parton_pdg_id) ==  4) type = "quark";
      else if (TMath::Abs(j1g_parton_pdg_id) ==  5) type = "quark";
      else if (TMath::Abs(j1g_parton_pdg_id) ==  1 || 
	       TMath::Abs(j1g_parton_pdg_id) ==  2 || 
	       TMath::Abs(j1g_parton_pdg_id) ==  3) type = "quark";
      else                                         type = "gluon";
      
      //j1_qg_tag_likelihood = qgsyst.Smear(j1_pt, j1_eta, rho, j1_qg_tag_likelihood, type);
      binFlavour = getFlavourBin(j1g_parton_pdg_id);
    }

    // Do the cuts here bbecause MC hase to be smeared :

    if ( j1_btag_csv < 0. || j1_btag_csv > 1. ) continue;
    ICUT->Fill((Double_t)icut,WEIGHT); cutName[icut] = "CSV output in [0,1]"; ++icut;

    if ( j1_qg_tag_likelihood < 0. || j1_qg_tag_likelihood > 1. ) continue;
    ICUT->Fill((Double_t)icut,WEIGHT); cutName[icut] = "QGL output in [0,1]"; ++icut;

    binZone = getZoneBin(j1_btag_csv, j1_qg_tag_likelihood);

    //----------------------------------------------------------------------------
    // Data/MC Comparison Plots
    //----------------------------------------------------------------------------
    
    if ( !_isMC ) {
      if ( run >= 190450 && run <= 193832 ) {
	InclusivePer1.fillHistos(WEIGHT);
      } else if ( run >= 193833 && run <= 199608 ) {
	InclusivePer2.fillHistos(WEIGHT);
      } else if ( run >= 199609 && run <= 208686 ) {
	InclusivePer3.fillHistos(WEIGHT);
      }
    } else {
      InclusivePer1.fillHistos(WEIGHT);
      InclusivePer2.fillHistos(WEIGHT);
      InclusivePer3.fillHistos(WEIGHT);
    }
    
    // inclusive Plots

    InclusivePlots.fillHistos(WEIGHT);

    // Plots per Pt Bin

    if ( p_pt >=  40 && p_pt <  60 ) ptBin1Plots.fillHistos(WEIGHT);
    if ( p_pt >=  60 && p_pt < 100 ) ptBin2Plots.fillHistos(WEIGHT);
    if ( p_pt >= 100 && p_pt < 155 ) ptBin3Plots.fillHistos(WEIGHT);
    if ( p_pt >= 155               ) ptBin4Plots.fillHistos(WEIGHT);

    // Plots per flavour zone

    if      ( binZone == 0 ) zone1QPlots.fillHistos(WEIGHT);
    else if ( binZone == 1 ) zone2GPlots.fillHistos(WEIGHT);
    else if ( binZone == 2 ) zone3CPlots.fillHistos(WEIGHT);
    else if ( binZone == 3 ) zone4BPlots.fillHistos(WEIGHT);
    else if ( binZone == 4 ) zone5QGPlots.fillHistos(WEIGHT);
    else if ( binZone == 5 ) zone6LCPlots.fillHistos(WEIGHT);
    else                     notInZonePlots.fillHistos(WEIGHT);

    //----------------------------------------------------------------------------
    // Plots for future analysis
    //----------------------------------------------------------------------------

    if ( _isMC ) {
      vGammapt_Flavour[binFlavour]->Fill(p_pt,WEIGHT);	
      vGammapt_Flavour[nflavours-1]->Fill(p_pt,WEIGHT);
    }
    
    //protection contre les jets non matches
    if(binZone!=-1) {

      // Rmpf for data and MC
      vRmpf_ZonePt[binZone][bin2DTaggingPt]->Fill(Rmpf,WEIGHT);

      // Tagger in zones
      vQGL_Pt[bin2DTaggingPt]->Fill(j1_qg_tag_likelihood,WEIGHT);
      vCSV_Pt[bin2DTaggingPt]->Fill(j1_btag_csv,WEIGHT);

      // MC specific
      if ( _isMC ) {
	vGammaPt_ZoneFlavour[binZone][binFlavour]->Fill(p_pt,WEIGHT);
	vGammaPt_ZoneFlavour[binZone][nflavours-1]->Fill(p_pt,WEIGHT);
	if(p_pt>200.) {
	  vFirstJetPt_ZoneFlavour[binZone][binFlavour]->Fill(j1_pt,WEIGHT);
	  vFirstJetPt_ZoneFlavour[binZone][nflavours-1]->Fill(j1_pt,WEIGHT);
	}
	vRtrue_ZonePt[binZone][bin2DTaggingPt]->Fill(Rtrue,WEIGHT);
	
	vRmpf_ZoneFlavourPt[binZone][binFlavour][bin2DTaggingPt]->Fill(Rmpf,WEIGHT);
	vRtrue_ZoneFlavourPt[binZone][binFlavour][bin2DTaggingPt]->Fill(Rtrue,WEIGHT);
	vRmpf_ZoneFlavourPt[binZone][nflavours-1][bin2DTaggingPt]->Fill(Rmpf,WEIGHT);
	vRtrue_ZoneFlavourPt[binZone][nflavours-1][bin2DTaggingPt]->Fill(Rtrue,WEIGHT);
	
	vRmpf_in2DTaggingZone_FlavourPt[binFlavour][bin2DTaggingPt]->Fill(Rmpf,WEIGHT);
	vRtrue_in2DTaggingZone_FlavourPt[binFlavour][bin2DTaggingPt]->Fill(Rtrue,WEIGHT);
	vRmpf_in2DTaggingZone_FlavourPt[nflavours-1][bin2DTaggingPt]->Fill(Rmpf,WEIGHT);
	vRtrue_in2DTaggingZone_FlavourPt[nflavours-1][bin2DTaggingPt]->Fill(Rtrue,WEIGHT);
	
	vFlavourNumber[binZone][binFlavour][bin2DTaggingPt] = vFlavourNumber[binZone][binFlavour][bin2DTaggingPt] + WEIGHT;
	vFlavourNumber[binZone][nflavours-1][bin2DTaggingPt] = vFlavourNumber[binZone][nflavours-1][bin2DTaggingPt] + WEIGHT;
      }
    }

    if ( _isMC ) {
      vRmpf_FlavourPt[binFlavour][bin2DTaggingPt]->Fill(Rmpf,WEIGHT);
      vRtrue_FlavourPt[binFlavour][bin2DTaggingPt]->Fill(Rtrue,WEIGHT);
      vRmpf_FlavourPt[nflavours-1][bin2DTaggingPt]->Fill(Rmpf,WEIGHT);
      vRtrue_FlavourPt[nflavours-1][bin2DTaggingPt]->Fill(Rtrue,WEIGHT);
      vQGL_FlavourPt[binFlavour][bin2DTaggingPt]->Fill(j1_qg_tag_likelihood,WEIGHT);
      vCSV_FlavourPt[binFlavour][bin2DTaggingPt]->Fill(j1_btag_csv,WEIGHT);
      vQGL_FlavourPt[nflavours-1][bin2DTaggingPt]->Fill(j1_qg_tag_likelihood,WEIGHT);
      vCSV_FlavourPt[nflavours-1][bin2DTaggingPt]->Fill(j1_btag_csv,WEIGHT);
      v2DTaggingPlan_FlavourPt[binFlavour][bin2DTaggingPt]->Fill(j1_btag_csv,j1_qg_tag_likelihood,WEIGHT);
      v2DTaggingPlan_FlavourPt[nflavours-1][bin2DTaggingPt]->Fill(j1_btag_csv,j1_qg_tag_likelihood,WEIGHT);//nflavours-1 : corresponds to total
    } 

    //----------------------------------------------------------------------------
    // MC Plots
    //----------------------------------------------------------------------------
    
    if ( _isMC ) {
      
      if ( j1g_is_present == 1 ) {

	MC_RTRUE->Fill(Rtrue,WEIGHT);
	MC_DPHI_J1J1G->Fill(Dphi_j1j1g,WEIGHT);
	MC_DR_J1J1G->Fill(DR_j1j1g,WEIGHT);
	
      }

    }
    
    //----------------------------------------------------------------------------
    // Write new tree :
    //----------------------------------------------------------------------------

    if (_doSkim) {
      _new_first_jetChain->Fill();
      _new_second_jetChain->Fill();
      _new_photonChain->Fill(); 
      _new_metChain->Fill();
      _new_electronsChain->Fill();
      _new_muonsChain->Fill();
      _new_miscChain->Fill();
      _new_rhoChain->Fill();
      // RAW
      if ( _doRAW ) { 
	_new_first_jet_rawChain->Fill();
	_new_second_jet_rawChain->Fill();
	_new_met_rawChain->Fill();
      }
      // MC only
      if ( _doMC ) {
	_new_first_jet_genChain->Fill();
	_new_second_jet_genChain->Fill();
	_new_met_genChain->Fill();
	_new_photon_genChain->Fill();
      }
      nwrite++;
    }

    ++nselected;
  }

  //================================================================================================
  // Print out Jet flavour fraction : for MC only
  //================================================================================================

  if ( _isMC ) {

  float Nuds,Ng,Nc,Nb,NnoMatched,Ntot;
  std::string nameFraction = "images2DTagging/FlavourFractions/";
  std::string nameHistoFraction;

  for(int j=0;j<n2DTaggingPtBins; j++) {
    for(int i=0; i<nzones; i++) {
      Nuds       = vFlavourNumber[i][0][j];
      Ng         = vFlavourNumber[i][1][j];
      Nc         = vFlavourNumber[i][2][j];
      Nb         = vFlavourNumber[i][3][j];
      NnoMatched = vFlavourNumber[i][4][j];
      Ntot       = vFlavourNumber[i][5][j];
      //Ntot       = vFlavourNumber[i][0][j]+vFlavourNumber[i][1][j]+vFlavourNumber[i][2][j]+vFlavourNumber[i][3][j];
      nameHistoFraction = vFractionHisto_Pt[j]->GetName();
      nameFraction += nameHistoFraction ;
      nameFraction += ".png";
      computeFlavourRatio(Nuds, Ng, Nc, Nb, NnoMatched, Ntot,vFractionHisto_Pt[j], nameFraction, my2DTaggingPtBinning, i, j);
      nameFraction = "";
      nameHistoFraction = "";
    }
  } 
  
  for(int j=0;j<n2DTaggingPtBins; j++) {
    for(int i=0; i<nzones; i++) {
      Nuds       = vFlavourNumber[i][0][j];
      Ng         = vFlavourNumber[i][1][j];
      Nc         = vFlavourNumber[i][2][j];
      Nb         = vFlavourNumber[i][3][j];
      NnoMatched = vFlavourNumber[i][4][j];
      Ntot       = vFlavourNumber[i][5][j];
      printLatexTableFlavourRatio(Nuds, Ng, Nc, Nb, NnoMatched, Ntot,vFractionHisto_Pt[j], nameFraction, my2DTaggingPtBinning, i, j);
    }
  }
  
  for(int k=0; k<n2DTaggingPtBins; k++) {
    for(int j=0; j<nflavours ; j++) {
      for(int i=0; i<nzones; i++) {
	vFlavourFraction[i][j][k] = vFlavourNumber[i][j][k];
      }
    }
  }
  
  cout<<""<<endl<<endl;
  
  for(int k=0; k<n2DTaggingPtBins; k++) {
    for(int j=0; j<nflavours ; j++) {
      for(int i=0; i<nzones; i++) {
	//Ntot       = vFlavourNumber[i][0][k]+vFlavourNumber[i][1][k]+vFlavourNumber[i][2][k]+vFlavourNumber[i][3][k];
	//vFlavourFraction[i][j][k] = vFlavourFraction[i][j][k]/Ntot;
	vFlavourFraction[i][j][k] = vFlavourFraction[i][j][k]/vFlavourFraction[i][nflavours-1][k];
	cout<<"vFlavourFraction["<<i<<"]["<<j<<"]["<<k<<"] : "<<vFlavourFraction[i][j][k]*100.<<" %"<<endl;
      }
    }
  }

  //================================================================================================
  // Write the matrix : for MC only
  //================================================================================================
  
  cout<<""<<endl<<endl;
  
  // filling the matrices

  for(int k=0; k<n2DTaggingPtBins; k++) {
    for(int j=0; j<nflavours-2; j++) {
      for(int i=0; i<nzones-2; i++) {
	v4x4MatrixPt[k](i,j) = vFlavourFraction[i][j][k];
	cout<<"v4x4MatrixPt["<<k<<"]("<<i<<","<<j<<") :  "<<v4x4MatrixPt[k](i,j)*100.<<" %"<<endl;
      }
    }
  }
  
  cout<<""<<endl<<endl;
  
  for(int k=0; k<n2DTaggingPtBins; k++) {
    for(int j=0; j<nflavours-2; j++) {
      for(int i=0; i<nzones; i++) {
	v6x4MatrixPt[k](i,j) = vFlavourFraction[i][j][k];
	cout<<"v6x4MatrixPt["<<k<<"]("<<i<<","<<j<<") : "<<v6x4MatrixPt[k](i,j)*100.<<" %"<<endl;
      }
    }
  }

  // write into the output matrix rootfile

  _newfile->cd();
  TDirectory *matrixDir = _newfile->mkdir("matrix","matrix");
  matrixDir->cd();
  for(int k=0; k<n2DTaggingPtBins; k++) {
    string matrix6x4Name;
    string matrix4x4Name;
    matrix6x4Name = "matrix6x4_" + my2DTaggingPtBinning.getName(k);
    matrix4x4Name = "matrix4x4_" + my2DTaggingPtBinning.getName(k);
    v6x4MatrixPt[k].Write(matrix6x4Name.c_str());
    v4x4MatrixPt[k].Write(matrix4x4Name.c_str());
  }

  } // endif _isMC

  //================================================================================================
  // Finish
  //================================================================================================

  cout << "======================================================================" << endl;
  for ( Int_t i=0; i<30 ; i++ ){
  cout << "Number of events after cut " 
       << setw(35) << cutName[i] 
       << " : " << std::fixed << ICUT->GetBinContent(i+1) << endl;
  }
  
  cout << "======================================================================" << endl;
  cout << "Total Number of events selected = "  << nselected                       << endl;
  cout << "Total Number of events skimmed = "  << nwrite                          << endl;
  cout << "======================================================================" << endl;

  _newfile->Write();
  _newfile->Close();

}
