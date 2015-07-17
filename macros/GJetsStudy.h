#ifndef GJetsStudy_h
#define GJetsStudy_h

#include <TROOT.h>
#include <TFile.h>
#include "TH2.h"
#include "THStack.h"
#include "TF1.h"
#include "TString.h"
#include "TCanvas.h"
#include "TPad.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

#include "../common.h"
#include "../ptBinning.cpp"

void setLogy(int log);
void setGrid(int grid);

class GJetsStudy
{
 public:
  GJetsStudy();
  GJetsStudy(const TString& fData, const TString& fMC_TOT, const TString& fMC_G, const TString& fMC_QCD);

  ~GJetsStudy();

  void init();

  //Setters
  
  void SetDebugMode(const bool& debug) { _debug = debug; }
  void SetLumi(const double& lumi) { _lumi = lumi; }
  void SetPlotRatio(const double& plotratio) { _plotratio = plotratio; }

  // Modifiers

  void delHistos();
  bool getHisto(const TString& histName);

  bool printInfo();
  bool printInfoFlavour();

  //Standard data/MC plots:

  bool plotBase(const TString& histName, bool toLumi=true, Int_t rebin=1);

  void plotAll(const TString& histName, bool toLumi=true, Int_t rebin=1, TString xtitle="dummy", TString legpos="right");
  void plotTot(const TString& histName, bool toLumi=true, Int_t rebin=1, TString xtitle="dummy", TString legpos="right");
  void plotInd(const TString& histName, bool toLumi=true, Int_t rebin=1, TString xtitle="dummy", TString legpos="right");

  // plots with MC flavour:

  void drawFlavour(const std::string& title, bool toLumi=true);
  void drawFlavourNoRatio(const std::string& title, bool toLumi=true);

  void drawTagger(const std::string& aTaggerName, const int& ptBin, int toLumi=1, int mode=0);
  void drawRmpfPerZone(const std::string& zoneName, const int& ptBin, int toLumi=1, int mode=0);
  // MC only plots
  void drawVarFlavour(const std::string& varName, int zoneBin=-1, std::string xtitle="",  int toLumi=1, int mode=0);

  // must be called after drawVarFlavour()
  void doFlavourRatio(std::string xtitle="p_{t}^{#gamma} [GeV/c]");

  void setXaxisRange(TH1F* h, const double& xmin, const double& xmax,bool lowpad=false);
  void setYaxisRange(TH1F* h, const double& ymin, const double& ymax, bool lowpad=false);

 public:

  // The TFile's
  TFile* _fData;
  TFile* _fMC_TOT;
  TFile* _fMC_G;
  TFile* _fMC_QCD;

  // General variables :
  int _nflavours;
  std::vector<int> _histoColor;
  ptBinning* _ptBinning;

  // Name of histogram currently being processed
  TString _hName; // Histo name
  bool _isOK;     // flag that histos are effectively there

  // Histos being processed
  TH1F* _hData;
  TH1F* _hMC_TOT;
  TH1F* _hMC_G;
  TH1F* _hMC_QCD;
  TH1F* _hDiv;
  std::vector<TH1F*> _vHisto;
  THStack* _hStack;

 public:

  // various variables
  std::ostream* _aStream;
  bool _debug;
  bool _plotratio;
  double _lumi;
  
  TPad* _pad_hi;
  TPad* _pad_lo;
  
};

#endif
