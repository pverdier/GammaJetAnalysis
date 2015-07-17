#include "GJetsStudy.h"

//#include <cmath>

#include "TLegend.h"

// TDR Style plots: from https://ghm.web.cern.ch/ghm/plots
#include "tdrstyle.C"
#include "CMS_lumi.C"

using namespace std;

void myh1_style(TH1 *h,
            int line_width=3,
            int line_color=1,
            int line_style=1,
            int fill_style=1001,
            int fill_color=50,
	    //float y_min=-1111.,
            //float y_max=-1111.,
            int ndivx=510,
            int ndivy=510,
            int marker_style=20,
            int marker_color=1,
            float marker_size=1.2,
            int optstat=0) {

   h->SetLineWidth(line_width);
   h->SetLineColor(line_color);
   h->SetLineStyle(line_style);
   h->SetFillColor(fill_color);
   h->SetFillStyle(fill_style);
   //h->SetMaximum(y_max);
   //h->SetMinimum(y_min);
   h->GetXaxis()->SetNdivisions(ndivx);
   h->GetYaxis()->SetNdivisions(ndivy);

   h->SetMarkerStyle(marker_style);
   h->SetMarkerColor(marker_color);
   h->SetMarkerSize(marker_size);
   h->SetStats(optstat);

   //h->SetLabelFont(62,"X");       // 42
   //h->SetLabelFont(62,"Y");       // 42
   //h->SetLabelOffset(0.01,"X");  // D=0.005
   //h->SetLabelOffset(0.01,"Y");  // D=0.005
   //h->SetLabelSize(0.045,"X");
   //h->SetLabelSize(0.045,"Y");
   h->SetTitleOffset(1.2,"X");
   h->SetTitleOffset(1.7,"Y");
   //h->SetTitleSize(0.06,"X");
   //h->SetTitleSize(0.06,"Y");
   h->SetTitle(0);
}

//==============================================================================
GJetsStudy::GJetsStudy()
//==============================================================================
{
  init();

  _fData   = new TFile("PhotonJet_Photon_Run2012.root");
  _fMC_TOT = new TFile("PhotonJet_MC_TOT_PFlowAK5chs.root");
  _fMC_G   = new TFile("PhotonJet_MC_G_PFlowAK5chs.root");
  _fMC_QCD = new TFile("PhotonJet_MC_QCD_PFlowAK5chs.root");

  setTDRStyle();
}

//==============================================================================
GJetsStudy::GJetsStudy(const TString& fData, const TString& fMC_TOT, const TString& fMC_G, const TString& fMC_QCD)
//==============================================================================
{
  init();

  _fData   = new TFile(fData.Data());
  _fMC_TOT = new TFile(fMC_TOT.Data());
  _fMC_G   = new TFile(fMC_G.Data());
  _fMC_QCD = new TFile(fMC_QCD.Data());
  
   setTDRStyle(); 
}

//==============================================================================
GJetsStudy::~GJetsStudy()
//==============================================================================
{
  *_aStream << "GJetsStudy::GJetsStudy() : Finished" << endl;
  //delHistos();
  _fData->Close();
  _fMC_TOT->Close();
  _fMC_G->Close();
  _fMC_QCD->Close();

  if ( _ptBinning != 0 ) delete _ptBinning;
}

//==============================================================================
void GJetsStudy::init()
//==============================================================================
{
  _isOK      = false;
  _aStream   = &cout;
  _debug     = false;
  _plotratio = false;
  _lumi      = 19711.;
  _pad_hi    = NULL;
  _pad_lo    = NULL;
  _hData     = NULL;
  _hMC_TOT   = NULL;
  _hMC_G     = NULL;
  _hMC_QCD   = NULL;
  _hDiv      = NULL;
  _hStack    = NULL;

  _ptBinning = new ptBinning(1); // to define correcly the binning
  
  _nflavours  = getFlavourNumber();
  _histoColor = HistoColor();
}

//==============================================================================
void GJetsStudy::delHistos()
//==============================================================================
{
  *_aStream << "GJetsStudy::delHistos()" << endl;

  if (_hData   != 0) { delete _hData;   _hData   = NULL; }
  if (_hMC_TOT != 0) { delete _hMC_TOT; _hMC_TOT = NULL; }
  if (_hMC_G   != 0) { delete _hMC_G;   _hMC_G   = NULL; }
  if (_hMC_QCD != 0) { delete _hMC_QCD; _hMC_QCD = NULL; }
  if (_hDiv    != 0) { delete _hDiv;    _hDiv    = NULL; }

  if (_pad_hi != 0) { delete _pad_hi; _pad_hi = NULL; }
  if (_pad_lo != 0) { delete _pad_lo; _pad_lo = NULL; }

  for (unsigned int i=0; i<_vHisto.size(); ++i ) {
    if ( _vHisto[i] != 0 ) { delete _vHisto[i]; _vHisto[i] = NULL; }
  }
  _vHisto.clear();
  if (_hStack != 0) { delete _hStack; _hStack = NULL; } // WARNING

  _isOK = false;
}

//==============================================================================
bool GJetsStudy::getHisto(const TString& histName)
//==============================================================================
{
  *_aStream << "GJetsStudy::getHisto()" << endl;

  // Delete previous histos
  delHistos();
  
  _hName = histName;

  // Get the histograms
  _hData   = (TH1F*)_fData  ->Get(_hName.Data())->Clone("HDATA");
  _hMC_TOT = (TH1F*)_fMC_TOT->Get(_hName.Data())->Clone("HMC_TOT");
  _hMC_G   = (TH1F*)_fMC_G  ->Get(_hName.Data())->Clone("HMC_G");
  _hMC_QCD = (TH1F*)_fMC_QCD->Get(_hName.Data())->Clone("HMC_QCD");

  if ( _hData != 0 && _hMC_TOT != 0 && _hMC_G != 0 && _hMC_QCD != 0 ) _isOK = true;
  return _isOK;
}

//==============================================================================
bool GJetsStudy::printInfo()
//==============================================================================
{
  *_aStream << "GJetsStudy::printInfo()" << endl;

  if ( !_isOK ) {
    *_aStream << "GJetsStudy::printInfo(): _isOK = false" << endl;
    return false;
  }

  Int_t binlo = 0;
  Int_t binhi = _hData->GetNbinsX();
  
  Double_t edata = 0.;
  Double_t ndata = _hData->IntegralAndError(binlo,binhi,edata);
  Double_t emctot = 0.;
  Double_t nmctot = _hMC_TOT->IntegralAndError(binlo,binhi,emctot);
  Double_t emcg = 0.;
  Double_t nmcg = _hMC_G->IntegralAndError(binlo,binhi,emcg);
  Double_t emcqcd = 0.;
  Double_t nmcqcd = _hMC_QCD->IntegralAndError(binlo,binhi,emcqcd);

  *_aStream << "############################################################################" << endl
	    << " GJetsStudy::printInfo() for Histo Name: " << _hData->GetName()               << endl;
  _aStream->width(20);  *_aStream << "Data : ";
  _aStream->width(15);  *_aStream << ndata;
  _aStream->width(5);   *_aStream << " +/- ";
  _aStream->width(15);  *_aStream << edata << endl;

  _aStream->width(20);  *_aStream << "MC tot. : ";
  _aStream->width(15);  *_aStream << nmctot;
  _aStream->width(5);   *_aStream << " +/- ";
  _aStream->width(15);  *_aStream << emctot << endl;

  _aStream->width(20);  *_aStream << "MC gamma+Jets : ";
  _aStream->width(15);  *_aStream << nmcg;
  _aStream->width(5);   *_aStream << " +/- ";
  _aStream->width(15);  *_aStream << emcg << endl;

  _aStream->width(20);  *_aStream << "MC QCD : ";
  _aStream->width(15);  *_aStream << nmcqcd;
  _aStream->width(5);   *_aStream << " +/- ";
  _aStream->width(15);  *_aStream << emcqcd << endl;
  *_aStream << "############################################################################" << endl;

  return true;
}

//==============================================================================
bool GJetsStudy::printInfoFlavour()
//==============================================================================
{
  *_aStream << "GJetsStudy::printInfoFlavour()" << endl;

  Int_t binlo = 0;
  Int_t binhi = _hData->GetNbinsX();
  
  Double_t edata = 0.;
  Double_t ndata = _hData->IntegralAndError(binlo,binhi,edata);

  *_aStream << "############################################################################" << endl
	    << " GJetsStudy::printInfoFlavour() :"                                            << endl;
  _aStream->width(20);  *_aStream << "Data : ";
  _aStream->width(15);  *_aStream << ndata;
  _aStream->width(5);   *_aStream << " +/- ";
  _aStream->width(15);  *_aStream << edata << endl;

  for (int i=0; i<_nflavours; ++i) {

    Double_t error  = 0.;
    Double_t number = _vHisto[i]->IntegralAndError(binlo,binhi,error);

    _aStream->width(17);  *_aStream << getFlavourBinName(i) << " : ";
    _aStream->width(15);  *_aStream << number;
    _aStream->width(5);   *_aStream << " +/- ";
    _aStream->width(15);  *_aStream << error << endl;
  }

  *_aStream << "############################################################################" << endl;

  return true;
}

//==============================================================================
bool GJetsStudy::plotBase(const TString& histName, bool toLumi, Int_t rebin)
//==============================================================================
{
  *_aStream << "GJetsStudy::plotbase() : Histo Name = " << histName << endl;

  // Get the histos :
  //-----------------
  if ( !getHisto(histName) ) {
    *_aStream << "GJetsStudy::plot(): _isOK = false" << endl;
    return false;
  }

  // Scaling :
  //----------
  if ( toLumi ) {
    // scale to luminosity
    _hMC_TOT->Scale(_lumi);
    _hMC_G  ->Scale(_lumi);
    _hMC_QCD->Scale(_lumi);
  } else {
    // scale MC_TOT to data
    Double_t ndata = _hData->Integral();
    Double_t nmc   = _hMC_TOT->Integral();
    _hMC_TOT->Scale(ndata/nmc);
    _hMC_G  ->Scale(ndata/nmc);
    _hMC_QCD->Scale(ndata/nmc);
  }
  
  // Rebinning :
  //------------
  if ( rebin != 0 ) {
    _hData->Rebin(rebin);
    _hMC_TOT->Rebin(rebin);
    _hMC_G  ->Rebin(rebin);
    _hMC_QCD->Rebin(rebin);
  }
  
  // print statistic: MUST BE DONE BEFORE PLOTTING
  //-----------------
  printInfo();

  return true;
}

//==============================================================================
void GJetsStudy::setXaxisRange(TH1F* h, const double& xmin, const double& xmax, bool lowpad)
//==============================================================================
{
  h->GetXaxis()->SetRangeUser(xmin,xmax-h->GetBinWidth(1)/2.);
  gPad->RedrawAxis();

  if (lowpad) {
    _hDiv->GetXaxis()->SetRangeUser(xmin,xmax-h->GetBinWidth(1)/2.);
    _pad_lo->RedrawAxis();
    _pad_hi->cd();
  }
  
}

//==============================================================================
void GJetsStudy::setYaxisRange(TH1F* h, const double& ymin, const double& ymax, bool lowpad)
//==============================================================================
{
  h->SetMaximum(ymax);
  h->SetMinimum(ymin);
  
  if (!lowpad) gPad->RedrawAxis();
  else _pad_lo->RedrawAxis();
}

//==============================================================================
void setLogy(int log)
//==============================================================================
{
  gPad->SetLogy(log);
  gPad->RedrawAxis();
}

//==============================================================================
void setGrid(int grid)
//==============================================================================
{
  gPad->SetGridx(grid);
  gPad->SetGridy(grid);
  gPad->RedrawAxis();
}

//==============================================================================
void GJetsStudy::plotAll(const TString& histName, bool toLumi, Int_t rebin, TString xtitle, TString legpos)
//==============================================================================
{
  *_aStream << "GJetsStudy::plotAll() : Histo Name = " << histName << endl;

  // Get the histos :
  //-----------------
  if ( !plotBase(histName,toLumi,rebin) ) {
    *_aStream << "GJetsStudy::plotBase(): _isOK = false" << endl;
    return;
  }
  
  // Just add QCD to MC_G (no THStack)
  //---------------------
  _hMC_G->Add(_hMC_QCD);

  // cosmectic:
  //-----------
  myh1_style(_hData  ,1,1,1,-1111, 50,510,510,20,1,1,0);
  myh1_style(_hMC_TOT,1,1,1,-1111, 50,510,510,20,1,1,0);
  myh1_style(_hMC_G,  1,1,1, 1001,807,510,510,20,1,1,0);
  myh1_style(_hMC_QCD,1,1,1, 1001,842,510,510,20,1,1,0);

  // Finally the plot:
  //------------------
  TCanvas* c1 = new TCanvas("c1","c1");

  _hMC_TOT->Draw("hist");
  _hMC_G->Draw("histsame");
  _hMC_QCD->Draw("histsame");
  _hData->Draw("pe1same");

  // Axis title:
  //------------
  Float_t inter = _hData->GetXaxis()->GetXmax() - _hData->GetXaxis()->GetXmin();
  Float_t num   = (Float_t)(inter/(Float_t)_hData->GetXaxis()->GetNbins());
  TString ytitle;
  if      ( _hData->GetBinWidth(1) < 1e-1 ) ytitle = TString::Format("Events / %0.2f", num);
  else if ( _hData->GetBinWidth(1) < 1e-2 ) ytitle = TString::Format("Events / %0.3f", num);
  else if ( _hData->GetBinWidth(1) < 1e-3 ) ytitle = TString::Format("Events / %0.4f", num);
  else                                      ytitle = TString::Format("Events / %0.1f", num);

  _hMC_TOT->GetYaxis()->SetTitle(ytitle.Data());

  if ( !xtitle.Contains("dummy") ) {
    _hMC_TOT->GetXaxis()->SetTitle(xtitle.Data());
  } else {
    _hMC_TOT->GetXaxis()->SetTitle(_hName.Data());
  }
  
  // Maximum:
  //---------
  
  Double_t ymax = 1.1*TMath::Max(_hMC_TOT->GetMaximum(),_hData->GetMaximum());
  _hMC_TOT->SetMaximum(ymax*1.1);

  gPad->RedrawAxis();

  // CMS information:
  //-----------------
  CMS_lumi(c1,2,33);

  // Legend :
  //---------
  
  TLegend *myLegend;
  if(legpos == "l") {
    myLegend = new TLegend(0.22,0.74,0.36,0.88);
  }
  else {
    myLegend = new TLegend(0.78,0.74,0.92,0.88);
  }
  //myLegend->SetBorderSize(0);
  myLegend->AddEntry(_hMC_TOT,"MC Tot.","l");
  myLegend->AddEntry(_hMC_QCD,"MC QCD","f");
  myLegend->AddEntry(_hMC_G,"MC #gamma + jet","f");
  myLegend->AddEntry(_hData,"Data","p");
  myLegend->SetFillColor(0);
  myLegend->Draw("SAME");

}

//==============================================================================
void GJetsStudy::plotTot(const TString& histName, bool toLumi, Int_t rebin, TString xtitle, TString legpos)
//==============================================================================
{
  *_aStream << "GJetsStudy::plotTot() : Histo Name = " << histName << endl;

  // Get the histos :
  //-----------------
  if ( !plotBase(histName,toLumi,rebin) ) {
    *_aStream << "GJetsStudy::plotTot(): _isOK = false" << endl;
    return;
  }
  
  // cosmectic:
  //-----------

  myh1_style(_hData  ,1,1,1,-1111, 50,510,510,20,1,1,0);
  myh1_style(_hMC_TOT,1,1,1, 1001,625,510,510,20,1,1,0);

  // Finally the plot:
  //------------------
  TCanvas* c1 = new TCanvas("c1","c1",600,720);
  c1->cd();
  
  _pad_hi = new TPad("pad_hi", "", 0., 0.20, 0.99, 0.99);
  _pad_hi->Draw();
  _pad_lo = new TPad("pad_lo", "", 0., 0., 0.99, 0.20);
  _pad_lo->SetBottomMargin(0.3);

  _pad_lo->Draw();
  
  _pad_hi->cd();
  _hMC_TOT->Draw("hist");
  _hData->Draw("pe1same");

  _pad_lo->cd();  
  _hDiv = (TH1F*)_hData->Clone("HDIV");
  _hDiv->Divide(_hMC_TOT);
  myh1_style(_hDiv  ,1,1,1,-1111, 50,510,510,20,1,1,0);
  _hDiv->SetMaximum(1.5);
  _hDiv->SetMinimum(0.5);
  _hDiv->Draw("pe1");
  _hDiv->SetYTitle("Data/MC ratio");
  _hDiv->GetXaxis()->SetLabelSize(0.14);
  _hDiv->GetYaxis()->SetLabelSize(0.08);
  _hDiv->GetYaxis()->SetTitleSize(0.1);
  _hDiv->GetYaxis()->SetTitleOffset(0.4);
  setGrid(1);
  _pad_hi->cd();

  // Axis title:
  //------------
  Float_t inter = _hData->GetXaxis()->GetXmax() - _hData->GetXaxis()->GetXmin();
  Float_t num   = (Float_t)(inter/(Float_t)_hData->GetXaxis()->GetNbins());
  TString ytitle;
  if      ( _hData->GetBinWidth(1) < 1e-1 ) ytitle = TString::Format("Events / %0.2f", num);
  else if ( _hData->GetBinWidth(1) < 1e-2 ) ytitle = TString::Format("Events / %0.3f", num);
  else if ( _hData->GetBinWidth(1) < 1e-3 ) ytitle = TString::Format("Events / %0.4f", num);
  else                                      ytitle = TString::Format("Events / %0.1f", num);

  _hMC_TOT->GetYaxis()->SetTitle(ytitle.Data());

  if ( !xtitle.Contains("dummy") ) {
    _hMC_TOT->GetXaxis()->SetTitle(xtitle.Data());
  } else {
    _hMC_TOT->GetXaxis()->SetTitle(_hName.Data());
  }
  
  // Maximum:
  //---------
  
  Double_t ymax = 1.1*TMath::Max(_hMC_TOT->GetMaximum(),_hData->GetMaximum());
  _hMC_TOT->SetMaximum(ymax*1.1);

  _pad_hi->RedrawAxis();

  // CMS information:
  //-----------------
  CMS_lumi(_pad_hi,2,33);

  // Legend :
  //---------
  
  TLegend *myLegend;
  if(legpos == "l") {
    myLegend = new TLegend(0.22,0.74,0.36,0.88);
  }
  else {
    myLegend = new TLegend(0.78,0.74,0.92,0.88);
  }
  //myLegend->SetBorderSize(0);
  myLegend->AddEntry(_hMC_TOT,"MC Tot.","f");
  myLegend->AddEntry(_hData,"Data","p");
  myLegend->SetFillColor(0);
  myLegend->Draw("SAME");

}

//==============================================================================
void GJetsStudy::plotInd(const TString& histName, bool toLumi, Int_t rebin, TString xtitle, TString legpos)
//==============================================================================
{
  *_aStream << "GJetsStudy::plotInd() : Histo Name = " << histName << endl;

  // Get the histos :
  //-----------------
  if ( !plotBase(histName,toLumi,rebin) ) {
    *_aStream << "GJetsStudy::plotTot(): _isOK = false" << endl;
    return;
  }

  // Just add QCD to MC_G (no THStack)
  //---------------------
  _hMC_G->Add(_hMC_QCD);
  
  // cosmectic:
  //-----------
  myh1_style(_hData  ,1,1,1,-1111, 50,510,510,20,1,1,0);
  myh1_style(_hMC_TOT,1,1,1,-1111, 50,510,510,20,1,1,0);
  myh1_style(_hMC_G,  1,1,1, 1001,807,510,510,20,1,1,0);
  myh1_style(_hMC_QCD,1,1,1, 1001,842,510,510,20,1,1,0);

  // Finally the plot:
  //------------------
  TCanvas* c1 = new TCanvas("c1","c1",600,720);
  c1->cd();
  
  _pad_hi = new TPad("pad_hi", "", 0., 0.20, 0.99, 0.99);
  _pad_hi->Draw();
  _pad_lo = new TPad("pad_lo", "", 0., 0., 0.99, 0.20);
  _pad_lo->SetBottomMargin(0.3);
  _pad_lo->Draw();
  
  _pad_hi->cd();
  _hMC_G->Draw("hist");
  _hMC_QCD->Draw("histsame");
  _hData->Draw("pe1same");

  _pad_lo->cd();  
  _hDiv = (TH1F*)_hData->Clone("HDIV");
  _hDiv->Divide(_hMC_G);
  myh1_style(_hDiv  ,1,1,1,-1111, 50,510,510,20,1,1,0);
  _hDiv->SetMaximum(1.5);
  _hDiv->SetMinimum(0.5);
  _hDiv->Draw("pe1");
  _hDiv->SetYTitle("Data/MC ratio");
  _hDiv->GetXaxis()->SetLabelSize(0.14);
  _hDiv->GetYaxis()->SetLabelSize(0.08);
  _hDiv->GetYaxis()->SetTitleSize(0.1);
  _hDiv->GetYaxis()->SetTitleOffset(0.4);
  setGrid(1);
  _pad_hi->cd();

  // Axis title:
  //------------
  Float_t inter = _hData->GetXaxis()->GetXmax() - _hData->GetXaxis()->GetXmin();
  Float_t num   = (Float_t)(inter/(Float_t)_hData->GetXaxis()->GetNbins());
  TString ytitle;
  if      ( _hData->GetBinWidth(1) < 1e-1 ) ytitle = TString::Format("Events / %0.2f", num);
  else if ( _hData->GetBinWidth(1) < 1e-2 ) ytitle = TString::Format("Events / %0.3f", num);
  else if ( _hData->GetBinWidth(1) < 1e-3 ) ytitle = TString::Format("Events / %0.4f", num);
  else                                      ytitle = TString::Format("Events / %0.1f", num);

  _hMC_G->GetYaxis()->SetTitle(ytitle.Data());

  if ( !xtitle.Contains("dummy") ) {
    _hMC_G->GetXaxis()->SetTitle(xtitle.Data());
  } else {
    _hMC_G->GetXaxis()->SetTitle(_hName.Data());
  }
  
  // Maximum:
  //---------
  
  Double_t ymax = 1.1*TMath::Max(_hMC_G->GetMaximum(),_hData->GetMaximum());
  _hMC_G->SetMaximum(ymax*1.1);

  _pad_hi->RedrawAxis();

  // CMS information:
  //-----------------
  CMS_lumi(_pad_hi,2,33);

  // Legend :
  //---------
  
  TLegend *myLegend;
  if(legpos == "l") {
    myLegend = new TLegend(0.22,0.74,0.36,0.88);
  }
  else {
    myLegend = new TLegend(0.78,0.74,0.92,0.88);
  }
  //myLegend->SetBorderSize(0);
  myLegend->AddEntry(_hMC_TOT,"MC Tot.","l");
  myLegend->AddEntry(_hMC_QCD,"MC QCD","f");
  myLegend->AddEntry(_hMC_G,"MC #gamma + jet","f");
  myLegend->AddEntry(_hData,"Data","p");
  myLegend->SetFillColor(0);
  myLegend->Draw("SAME");

}

//====================================================================================
void GJetsStudy::drawFlavour(const std::string& title, bool toLumi)
//====================================================================================
{
  // Get Total histo : put it in _hMC_TOT whatever the mode was
  //------------------

  _hMC_TOT = (TH1F*)_vHisto[getFlavourNumber()-1]->Clone();

  Double_t normFac = (Double_t)_hData->Integral()/(Double_t)_hMC_TOT->Integral();
  if ( toLumi ) normFac = _lumi;

  _hMC_TOT->Scale(normFac);

  // Create the THStack :
  //---------------------
  // do it like this in order to have the correct number of events in printInfoFlavour()

  _hStack = new THStack();
  for (int i = 0; i<_nflavours; ++i) {
    // Normalization:
    _vHisto[i]->Scale(normFac);

    // Cosmetics: 
    h1_style(_vHisto[i]);
    _vHisto[i]->SetLineColor(1);
    _vHisto[i]->SetFillColor(_histoColor[i]);

    // Create the THStack
    if ( i < _nflavours-1 ) _hStack->Add(_vHisto[i]);
  }

  // cosmectic:
  //-----------

  myh1_style(_hData  ,1,1,1,-1111, 50,510,510,20,1,1,0);
  myh1_style(_hMC_TOT,1,1,1, 3001,  1,510,510,0,1,1,0);
  
  // Finally the plot:
  //------------------
  TCanvas* c1 = new TCanvas("c1","c1",600,720);
  c1->cd();
  
  _pad_hi = new TPad("pad_hi", "", 0., 0.20, 0.99, 0.99);
  _pad_hi->Draw();
  _pad_lo = new TPad("pad_lo", "", 0., 0., 0.99, 0.20);
  _pad_lo->SetBottomMargin(0.3);
  _pad_lo->Draw();
  
  _pad_hi->cd();
  _hMC_TOT->Draw("hist");
  _hStack->Draw("histsame");
  _hMC_TOT->Draw("e2same");
  _hData->Draw("pe1same");
  
  _pad_lo->cd();  
  _hDiv = (TH1F*)_hData->Clone("HDIV");
  _hDiv->Divide(_hMC_TOT);
  myh1_style(_hDiv  ,1,1,1,-1111, 50,510,510,20,1,1,0);
  _hDiv->SetMaximum(1.5);
  _hDiv->SetMinimum(0.5);
  _hDiv->Draw("pe1");
  _hDiv->SetYTitle("Data/MC ratio");
  _hDiv->GetXaxis()->SetLabelSize(0.14);
  _hDiv->GetYaxis()->SetLabelSize(0.08);
  _hDiv->GetYaxis()->SetTitleSize(0.1);
  _hDiv->GetYaxis()->SetTitleOffset(0.4);
  setGrid(1);
  _pad_hi->cd();

  // Axis title:
  //------------
  Float_t inter = _hData->GetXaxis()->GetXmax() - _hData->GetXaxis()->GetXmin();
  Float_t num   = (Float_t)(inter/(Float_t)_hData->GetXaxis()->GetNbins());
  TString ytitle;
  if      ( _hData->GetBinWidth(1) < 1e-1 ) ytitle = TString::Format("Events / %0.3f", num);
  else if ( _hData->GetBinWidth(1) < 1e-2 ) ytitle = TString::Format("Events / %0.4f", num);
  else if ( _hData->GetBinWidth(1) < 1e-3 ) ytitle = TString::Format("Events / %0.f", num);
  else                                      ytitle = TString::Format("Events / %0.1f", num);

  _hMC_TOT->GetYaxis()->SetTitle(ytitle.Data());
  _hMC_TOT->GetXaxis()->SetTitle(title.c_str());
  
  // Maximum:
  //---------
  
  Double_t ymax = 1.1*TMath::Max(_hMC_TOT->GetMaximum(),_hData->GetMaximum());
  _hMC_TOT->SetMaximum(ymax*1.1);

  _pad_hi->RedrawAxis();

  // CMS information:
  //-----------------
  CMS_lumi(_pad_hi,2,33);

  TLegend *aLegend = new TLegend(0.69,0.65,0.89,0.89);
  aLegend->SetBorderSize(0);
  for(int i=0; i<getFlavourNumber()-1; ++i) {
    aLegend->AddEntry(_vHisto[i],getFlavourBinName(i).c_str(),"f");
  }

  aLegend->AddEntry(_hData,"Data","p");
  aLegend->SetFillStyle(0);
  aLegend->Draw("SAME");

}

//====================================================================================
void GJetsStudy::drawFlavourNoRatio(const std::string& title, bool toLumi)
//====================================================================================
{
  // Get Total histo : put it in _hMC_TOT whatever the mode was
  //------------------

  _hMC_TOT = (TH1F*)_vHisto[getFlavourNumber()-1]->Clone();

  Double_t normFac = (Double_t)_hData->Integral()/(Double_t)_hMC_TOT->Integral();
  if ( toLumi ) normFac = _lumi;

  _hMC_TOT->Scale(normFac);

  // Create the THStack :
  //---------------------
  // do it like this in order to have the correct number of events in printInfoFlavour()

  _hStack = new THStack();
  for (int i = 0; i<_nflavours; ++i) {
    // Normalization:
    _vHisto[i]->Scale(normFac);

    // Cosmetics: 
    h1_style(_vHisto[i]);
    _vHisto[i]->SetLineColor(1);
    _vHisto[i]->SetFillColor(_histoColor[i]);

    // Create the THStack
    if ( i < _nflavours-1 ) _hStack->Add(_vHisto[i]);
  }

  // cosmectic:
  //-----------

  myh1_style(_hData  ,1,1,1,-1111, 50,510,510,20,1,1,0);
  myh1_style(_hMC_TOT,1,1,1, 3001,  1,510,510,0,1,1,0);
  
  // Finally the plot:
  //------------------
  TCanvas* c1 = new TCanvas("c1","c1");

  _hMC_TOT->Draw("hist");
  _hStack->Draw("histsame");
  _hMC_TOT->Draw("e2same");
  _hData->Draw("pe1same");
  
  // Axis title:
  //------------
  Float_t inter = _hData->GetXaxis()->GetXmax() - _hData->GetXaxis()->GetXmin();
  Float_t num   = (Float_t)(inter/(Float_t)_hData->GetXaxis()->GetNbins());
  TString ytitle;
  if      ( _hData->GetBinWidth(1) < 1e-1 ) ytitle = TString::Format("Events / %0.3f", num);
  else if ( _hData->GetBinWidth(1) < 1e-2 ) ytitle = TString::Format("Events / %0.4f", num);
  else if ( _hData->GetBinWidth(1) < 1e-3 ) ytitle = TString::Format("Events / %0.f", num);
  else                                      ytitle = TString::Format("Events / %0.1f", num);

  _hMC_TOT->GetYaxis()->SetTitle(ytitle.Data());
  _hMC_TOT->GetXaxis()->SetTitle(title.c_str());
  
  // Maximum:
  //---------
  
  Double_t ymax = 1.1*TMath::Max(_hMC_TOT->GetMaximum(),_hData->GetMaximum());
  _hMC_TOT->SetMaximum(ymax*1.1);

  gPad->RedrawAxis();

  // CMS information:
  //-----------------
  CMS_lumi(c1,2,33);

  TLegend *aLegend = new TLegend(0.69,0.65,0.89,0.89);
  aLegend->SetBorderSize(0);
  for(int i=0; i<getFlavourNumber()-1; ++i) {
    aLegend->AddEntry(_vHisto[i],getFlavourBinName(i).c_str(),"f");
  }

  aLegend->AddEntry(_hData,"Data","p");
  aLegend->SetFillStyle(0);
  aLegend->Draw("SAME");

}

//====================================================================================
void GJetsStudy::drawTagger(const string& aTaggerName, const int& ptBin, int toLumi, int mode) 
//====================================================================================
{
//*************************************************************************************************
// description: function which draws the flavour tagger (CSV or QGL) distributions for data and MC
// arguments: - aTaggerName: name of the tagger (CSV or QGL) you want to study
//            - aPtName: name of the 2DTagging pt bin
//            - ptBin: number of the corresponding 2DTagging pt bin
//            - toLumi: 1 if you want MC distributions normalized to the luminosity, 
//                      0 if you want MC distributions normalized to the number of data events (shape)
//            - mode: 0 fMC_TOT file, 1 fMC_G+fMC_QCD, 2 FMC_G only,
//*************************************************************************************************

  // Delete previous histos
  delHistos();
  
  // Parameter Initialization :
  //---------------------------

  string aPtName = _ptBinning->getName(ptBin);

  std::stringstream PtBinningName ;
  PtBinningName << (int) _ptBinning->getBinValueInf(ptBin) << " GeV < p_{t}^{#gamma} < " << (int) _ptBinning->getBinValueSup(ptBin) << " GeV";

  string aTitle;
  aTitle = aTaggerName + " distribution for " + PtBinningName.str();

  // Get the Histograms for data :
  //------------------------------

  string histoNameData;
  histoNameData = "tagger_Pt/" + aTaggerName + "/" + aTaggerName + "_" + aPtName;
  
  if (_debug) *_aStream << histoNameData << endl;

  _hData = (TH1F*)_fData->Get(histoNameData.c_str());
  
  // Get Histograms for MC :
  //------------------------

  for (int i=0; i<_nflavours; ++i) {

    string histoNameMC;
    histoNameMC = "tagger_Flavour_Pt/" + aTaggerName + "/" + aTaggerName + "_" + getFlavourBinName(i) + "_" + aPtName;

    if (_debug) *_aStream << histoNameMC << endl;

    if ( mode == 0 ) {
      _vHisto.push_back((TH1F*)((TH1F*)_fMC_TOT->Get(histoNameMC.c_str()))->Clone());
    } else if ( mode == 1 ) {
      TH1F* hTMP = (TH1F*)((TH1F*)_fMC_G->Get(histoNameMC.c_str()))->Clone();
      hTMP->Add((TH1F*)_fMC_QCD->Get(histoNameMC.c_str()));
      _vHisto.push_back(hTMP);
    } else if ( mode ==2 ) {
      _vHisto.push_back((TH1F*)((TH1F*)_fMC_G->Get(histoNameMC.c_str()))->Clone());
    }
  }
  
  // Finally call the function doing the plot :
  //-------------------------------------------
  
  drawFlavour(aTitle.c_str(),toLumi);
  
  // Print number of events per flavour
  printInfoFlavour();

}

//====================================================================================
void GJetsStudy::drawRmpfPerZone(const string& zoneName, const int& ptBin, int toLumi, int mode) 
//====================================================================================
{
//*************************************************************************************************
// description: function which draws the flavour tagger (CSV or QGL) distributions for data and MC
// arguments: - aTaggerName: name of the tagger (CSV or QGL) you want to study
//            - aPtName: name of the 2DTagging pt bin
//            - ptBin: number of the corresponding 2DTagging pt bin
//            - toLumi: 1 if you want MC distributions normalized to the luminosity, 
//                      0 if you want MC distributions normalized to the number of data events (shape)
//            - mode: 0 fMC_TOT file, 1 fMC_G+fMC_QCD, 2 FMC_G only,
//*************************************************************************************************

  // Delete previous histos
  delHistos();
  
  // Parameter Initialization :
  //---------------------------

  string aPtName = _ptBinning->getName(ptBin);

  std::stringstream PtBinningName ;
  PtBinningName << (int) _ptBinning->getBinValueInf(ptBin) << " GeV < p_{t}^{#gamma} < " << (int) _ptBinning->getBinValueSup(ptBin) << " GeV";

  string aTitle;
  aTitle = "R_{" + zoneName + "}^{MPF} for " + PtBinningName.str();

  // Get the Histograms for data :
  //------------------------------

  string histoNameData;
  histoNameData = "response_Zone_Pt/Rmpf/Rmpf_" + zoneName + "_" + aPtName;

  if (_debug) *_aStream << histoNameData << endl;

  _hData = (TH1F*)_fData->Get(histoNameData.c_str());
  
  // Get Histograms for MC :
  //------------------------

  for (int i=0; i<_nflavours; ++i) {

    string histoNameMC;
    histoNameMC = "response_Zone_Flavour_Pt/Rmpf/Rmpf_" + zoneName + "_" + getFlavourBinName(i) + "_" + aPtName;

    if (_debug) *_aStream << histoNameMC << endl;

    if ( mode == 0 ) {
      _vHisto.push_back((TH1F*)((TH1F*)_fMC_TOT->Get(histoNameMC.c_str()))->Clone());
    } else if ( mode == 1 ) {
      TH1F* hTMP = (TH1F*)((TH1F*)_fMC_G->Get(histoNameMC.c_str()))->Clone();
      hTMP->Add((TH1F*)_fMC_QCD->Get(histoNameMC.c_str()));
      _vHisto.push_back(hTMP);
    } else if ( mode ==2 ) {
      _vHisto.push_back((TH1F*)((TH1F*)_fMC_G->Get(histoNameMC.c_str()))->Clone());
    }
  }
  
  // Finally call the function doing the plot :
  //-------------------------------------------
  
  drawFlavour(aTitle.c_str(),toLumi);
  
  // Print number of events per flavour
  printInfoFlavour();

}

//====================================================================================
void GJetsStudy::drawVarFlavour(const string& varName, int zoneBin, string xtitle,  int toLumi, int mode)
//====================================================================================
{
//*************************************************************************************************
// description: function which draws the flavour tagger (CSV or QGL) distributions for data and MC
// arguments: - aTaggerName: name of the tagger (CSV or QGL) you want to study
//            - aPtName: name of the 2DTagging pt bin
//            - ptBin: number of the corresponding 2DTagging pt bin
//            - toLumi: 1 if you want MC distributions normalized to the luminosity, 
//                      0 if you want MC distributions normalized to the number of data events (shape)
//            - mode: 0 fMC_TOT file, 1 fMC_G+fMC_QCD, 2 FMC_G only,
//*************************************************************************************************
  
  // Delete previous histos
  delHistos();
  
  // Parameter Initialization :
  //---------------------------
  
  string zoneName = getZoneBinName(zoneBin);

  string aTitle;
  if ( xtitle.size() == 0 ) aTitle = varName;
  else                      aTitle = xtitle;

  if ( zoneName.size() == 0 ) aTitle = aTitle + " (inclusive)";
  else                        aTitle = aTitle + " for " + zoneName;

  // Get Histograms for MC :
  //------------------------

  string dirName;
  if      ( varName == "Gammapt"    && zoneName.size() == 0 ) dirName = "gammapt_Flavour/";
  else if ( varName == "Gammapt"    && zoneName.size() >  0 ) dirName = "gammaPtPerZonePerFlavour/";
  else if ( varName == "FirstJetPt" && zoneName.size() >  0 ) dirName = "firstJetPtPerZonePerFlavour/";
  else {
    *_aStream << "GJetsStudy::drawVarFlavour: histo not known" << endl;
    return;
  }

  for (int i=0; i<_nflavours; ++i) {

    string histoNameMC;
    histoNameMC = dirName + varName + "_";
    if ( zoneName.size() > 0 ) histoNameMC = histoNameMC + zoneName + "_" + getFlavourBinName(i);
    else                       histoNameMC = histoNameMC + getFlavourBinName(i);

    *_aStream << histoNameMC << endl;

    if ( mode == 0 ) {
      _vHisto.push_back((TH1F*)((TH1F*)_fMC_TOT->Get(histoNameMC.c_str()))->Clone());
    } else if ( mode == 1 ) {
      TH1F* hTMP = (TH1F*)((TH1F*)_fMC_G->Get(histoNameMC.c_str()))->Clone();
      hTMP->Add((TH1F*)_fMC_QCD->Get(histoNameMC.c_str()));
      _vHisto.push_back(hTMP);
    } else if ( mode ==2 ) {
      _vHisto.push_back((TH1F*)((TH1F*)_fMC_G->Get(histoNameMC.c_str()))->Clone());
    }
  }

  // Create a dummy histo for data :
  //--------------------------------

  _hData = (TH1F*)_vHisto[0]->Clone("HDATA");
  _hData->Reset();

  // Finally call the function doing the plot :
  //-------------------------------------------
  
  drawFlavourNoRatio(aTitle.c_str(),toLumi);
  
  // Print number of events per flavour
  printInfoFlavour();

}

//====================================================================================
void GJetsStudy::doFlavourRatio(std::string xtitle)
//====================================================================================
{
//*************************************************************************************************
// description: must be called after drawVarFlavour()
//*************************************************************************************************
  
  // define histos conatining the result
  vector<TH1*> vHistoFrac;
  vHistoFrac.resize(_nflavours-1);
  
  // Define the binning
  double binning[] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 800};
  
  // Get all histos
  TH1* all_rebin = _vHisto[_nflavours-1]->Rebin(15, "tot", binning);
  for(int i=0; i<_nflavours-1; ++i) {
    std::string newName = getFlavourBinName(i);
    vHistoFrac[i] = _vHisto[i]->Rebin(15, newName.c_str(), binning);
    vHistoFrac[i]->Divide(all_rebin);
    vHistoFrac[i]->SetMarkerStyle(20);
    vHistoFrac[i]->SetMarkerColor(_histoColor[i]);
    vHistoFrac[i]->SetLineColor(_histoColor[i]);
  }

  // Make the plot
  TCanvas* c1 = new TCanvas("c1","c1");
  c1->cd();
  vHistoFrac[0]->Draw("PE1");
  for(int i=1; i<_nflavours-1; ++i) {
    vHistoFrac[i]->Draw("PE1same");
  }

  // Cosmectics
  vHistoFrac[0]->SetTitle(0);
  vHistoFrac[0]->GetXaxis()->SetRangeUser(30.,800.);
  vHistoFrac[0]->GetXaxis()->SetTitle(xtitle.c_str());
  vHistoFrac[0]->GetYaxis()->SetTitle("Flavour fraction");
  vHistoFrac[0]->GetYaxis()->SetTitleOffset(1.7);
  vHistoFrac[0]->GetXaxis()->SetTitleOffset(1.5);
  vHistoFrac[0]->GetYaxis()->SetRangeUser(0,1);

  // Legend
  TLegend *aLegend = new TLegend(0.20,0.75,0.42,0.90);
  aLegend->SetBorderSize(0);
  aLegend->SetFillColor(0);
  aLegend->SetFillStyle(0);
  aLegend->SetTextFont(42);
  aLegend->SetTextSizePixels(24);
  for(int i=0; i<_nflavours-1; i++) {
    aLegend->AddEntry(vHistoFrac[i],getFlavourBinName(i).c_str(),"p");
  }
  aLegend->Draw("same");

}
