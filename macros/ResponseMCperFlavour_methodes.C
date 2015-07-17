#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "../ptBinning.cpp"
#include "../binning.h"

#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "tdrstyle.C"
#include "CMS_lumi.C"

using namespace std;

class fitResult
{

public:
  fitResult() {};
  ~fitResult() {};

  void print() {
    cout << "Fit result : chi2/ndf = " << chi2  << "  ,  " << ndf    << endl
	 << "             mean     = " << mean  << " +/- " << emean  << endl
	 << "             sigma    = " << sigma << " +/- " << esigma << endl;
  }
  
  double chi2;
  int    ndf;
  double mean;
  double emean;
  double sigma;
  double esigma;
};


fitResult fitGaus(TH1F* h, const double& xmin, const double& xmax)
{

  TF1* fres = new TF1("fSignal","gaus",0.,2.);
  fres->SetParameter(1,h->GetMean());
  fres->SetParameter(2,h->GetRMS());
  
  h->Fit("fSignal","QR","",xmin,xmax);
  //h->Fit("gaus","QR0","",xmin,xmax);
  //TF1 *fres = h->GetFunction("gaus");
  gPad->Update();
  getchar();

  fitResult aFitRes;

  aFitRes.chi2 = fres->GetChisquare();
  aFitRes.ndf  = fres->GetNDF();
  
  aFitRes.mean  = fres->GetParameter(1);
  aFitRes.emean = fres->GetParError(1);
  
  aFitRes.sigma  = fres->GetParameter(2);
  aFitRes.esigma = fres->GetParError(2);

  return aFitRes;
}


vector<TGraphErrors*> fitResponse(TFile* f, bool doRtrue=false, bool doFit=false, bool doRange=false, double offset=0.)
{ 

  // Prepare the result:
  
  vector<TGraphErrors*> vGraph_Mean;

  // data/MC Rmpf : response_Zone_Pt/Rmpf/Rmpf_zone1Q_ptPhot_40_100
  // MC Rtrue: response_Zone_Pt/Rtrue/Rtrue_zone1Q_ptPhot_40_100

 
  ptBinning aPtBinning(1);

  const int nFlav = getFlavourNumber();
  double x[nFlav];
  double ex[nFlav];
  double y1[nFlav];
  double ey1[nFlav];
  double y2[nFlav];
  double ey2[nFlav];

  // Loop on the pT Bins
  for (unsigned int j=0; j<aPtBinning.getSize(); ++j) {
    
    
    // Loop on the flavour zones
    for (int i=0; i<nFlav; ++i) {
      
      string histoName;
      if ( !doRtrue ) {
	histoName = "response_Flavour_Pt/Rmpf/Rmpf_"+getFlavourBinName(i)+"_"+aPtBinning.getName(j);
      } else {
	histoName = "response_Flavour_Pt/Rtrue/Rtrue_"+getFlavourBinName(i)+"_"+aPtBinning.getName(j);
      }
      cout << " => processing histoName = " << histoName << endl;
      
      TH1F* h = (TH1F*)f->Get(histoName.c_str());  

      if ( !doFit ) {
	x[i]   = offset+(double)i+1; ex[i] = 0.;
	y1[i]  = h->GetMean(); ey1[i] = h->GetMeanError();
	y2[i]  = h->GetRMS();  ey2[i] = h->GetRMSError();
      } else {
      
	double xmin = 0.;
	double xmax = 2.;
	if ( doRange ) {
	  if ( !doRtrue ) {
	    if      ( j==0 ) { xmin = 0.6; xmax = 1.4; }
	    else if ( j==1 ) { xmin = 0.7; xmax = 1.3; }
	    else if ( j==2 ) { xmin = 0.8; xmax = 1.2; }
	  } else {
	    xmin = 0.8; xmax = 1.2;
	  }
	}
	
	fitResult res = fitGaus(h,xmin,xmax);
	
	res.print();
	
	x[i]   = offset+(double)i+1; ex[i] = 0.;
	y1[i]  = res.mean;  ey1[i] = res.emean;
	y2[i]  = res.sigma; ey2[i] = res.esigma;

      }
      
    } // end loop flavour zone

    vGraph_Mean.push_back(new TGraphErrors(nFlav,x,y1,ex,ey1));
    
    vGraph_Mean[j]->SetMarkerStyle(20);
    vGraph_Mean[j]->SetMarkerColor(2);
    vGraph_Mean[j]->SetMarkerSize(1.);
    vGraph_Mean[j]->SetLineColor(1);
    string title;
    if ( !doRtrue ) {
      title = "R_{MPF} for pt bin = "+aPtBinning.getName(j);
    } else {
      title = "R_{true} for pt bin = "+aPtBinning.getName(j);
    }
    vGraph_Mean[j]->SetTitle(title.c_str());

    vGraph_Mean[j]->SetMaximum(1.06);
    vGraph_Mean[j]->SetMinimum(0.94);
 
  } // end loop pT Bins
  

  return vGraph_Mean;
}

void ResponseMCperFlavour_methodes()
{
  setTDRStyle();
  gStyle->SetCanvasDefH(500); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas

  ptBinning aPtBinning(1);

  //TFile* f1 = new TFile("PhotonJet_MC_TOT_PFlowAK5chs.root");
  //TFile* f1 = new TFile("PhotonJet_MC_G_PFlowAK5chs.root");
  TFile* f1 = new TFile("PhotonJet_MC_QCD_PFlowAK5chs.root");

  // Compare 3 methods:
  // Mean/RMS
  // gaussian fit
  // gaussian fit truncated

  vector<TGraphErrors*> gr_Rmpf_TOT = fitResponse(f1,false,false,false,0.);
  vector<TGraphErrors*> gr_Rmpf_G   = fitResponse(f1,false,true,false,0.05);
  vector<TGraphErrors*> gr_Rmpf_QCD = fitResponse(f1,false,true,true,0.1);

  vector<TGraphErrors*> gr_Rtrue_TOT = fitResponse(f1,true,false,false,0.);
  vector<TGraphErrors*> gr_Rtrue_G   = fitResponse(f1,true,true,false,0.05);
  vector<TGraphErrors*> gr_Rtrue_QCD = fitResponse(f1,true,true,true,0.1);

  for (int i=0; i<gr_Rmpf_TOT.size(); ++i) {

    // Rmpf :
    //-------
    
    TCanvas* c1 = new TCanvas();
    c1->SetBottomMargin(0.2);
    
    gr_Rmpf_TOT[i]->Draw("APE");
    gr_Rmpf_G[i]  ->Draw("PE");
    gr_Rmpf_QCD[i]->Draw("PE");

    gr_Rmpf_TOT[i]->SetMaximum(1.2);
    gr_Rmpf_TOT[i]->SetMinimum(0.8);

    //Cosmetic
    gr_Rmpf_TOT[i]->SetLineColor(1); gr_Rmpf_TOT[i]->SetMarkerColor(1);
    gr_Rmpf_G[i]  ->SetLineColor(1); gr_Rmpf_G[i]  ->SetMarkerColor(2);
    gr_Rmpf_QCD[i]->SetLineColor(1); gr_Rmpf_TOT[i]->SetMarkerColor(4);
    
    gr_Rmpf_TOT[i]->GetXaxis()->SetNdivisions(6,kTRUE);
    for(int j=0; j<getFlavourNumber(); ++j)
      gr_Rmpf_TOT[i]->GetXaxis()->SetBinLabel(gr_Rmpf_TOT[i]->GetXaxis()->FindBin(j+1),getFlavourBinName(j).c_str());
    
    gr_Rmpf_TOT[i]->GetYaxis()->SetTitle("R_{mpf}");
    gr_Rmpf_TOT[i]->GetYaxis()->SetLabelSize(0.04);
    gr_Rmpf_TOT[i]->GetYaxis()->SetTitleOffset(1.05);

    TLegend *myLegend = new TLegend(0.18,0.28,0.40,0.42);
    myLegend->AddEntry(gr_Rmpf_TOT[i],"Mean/RMS","p");
    myLegend->AddEntry(gr_Rmpf_G[i],  "Gaussian","p");
    myLegend->AddEntry(gr_Rmpf_QCD[i],"Trunc. Gaussian","p");
    myLegend->SetFillColor(0);
    myLegend->Draw("same");

    TPaveText* tt = new TPaveText(0.18,0.88, 0.58,0.93,"brNDC");
    tt->SetTextSize(0.04);
    tt->SetFillColor(0);
    tt->SetTextFont(42);  
    std::stringstream PtBinningName ;
    PtBinningName << (int) aPtBinning.getBinValueInf(i) << " GeV < p_{t}^{#gamma} < " << (int) aPtBinning.getBinValueSup(i) << " GeV";
    tt->AddText(PtBinningName.str().c_str());
    tt->Draw("same");

    CMS_lumi(c1,2,33);
    
    c1->SetGridy(1);
    c1->RedrawAxis();
    c1->Update();

    // Rtrue :
    //--------

    TCanvas* c2 = new TCanvas();
    c2->SetBottomMargin(0.2);

    gr_Rtrue_TOT[i]->Draw("APE");
    gr_Rtrue_G[i]  ->Draw("PE");
    gr_Rtrue_QCD[i]->Draw("PE");

    gr_Rtrue_TOT[i]->SetMaximum(1.2);
    gr_Rtrue_TOT[i]->SetMinimum(0.8);

    //Cosmetic
    gr_Rtrue_TOT[i]->SetLineColor(1); gr_Rtrue_TOT[i]->SetMarkerColor(1);
    gr_Rtrue_G[i]  ->SetLineColor(1); gr_Rtrue_G[i]  ->SetMarkerColor(2);
    gr_Rtrue_QCD[i]->SetLineColor(1); gr_Rtrue_QCD[i]->SetMarkerColor(4);
    
    gr_Rtrue_TOT[i]->GetXaxis()->SetNdivisions(6,kTRUE);
    for(int j=0; j<getFlavourNumber(); ++j)
      gr_Rtrue_TOT[i]->GetXaxis()->SetBinLabel(gr_Rtrue_TOT[i]->GetXaxis()->FindBin(j+1),getFlavourBinName(j).c_str());
    
    gr_Rtrue_TOT[i]->GetYaxis()->SetTitle("R_{true}");
    gr_Rtrue_TOT[i]->GetYaxis()->SetLabelSize(0.04);
    gr_Rtrue_TOT[i]->GetYaxis()->SetTitleOffset(1.05);

    TLegend *myLegend2 = new TLegend(0.18,0.28,0.40,0.42);
    myLegend2->AddEntry(gr_Rtrue_TOT[i],"Mean/RMS","p");
    myLegend2->AddEntry(gr_Rtrue_G[i],  "Gaussian","p");
    myLegend2->AddEntry(gr_Rtrue_QCD[i],"Trunc. Gaussian","p");
    myLegend2->SetFillColor(0);
    myLegend2->Draw("same");

    TPaveText* tt2 = new TPaveText(0.18,0.88, 0.58,0.93,"brNDC");
    tt2->SetTextSize(0.04);
    tt2->SetFillColor(0);
    tt2->SetTextFont(42);  
    std::stringstream PtBinningName2 ;
    PtBinningName2 << (int) aPtBinning.getBinValueInf(i) << " GeV < p_{t}^{#gamma} < " << (int) aPtBinning.getBinValueSup(i) << " GeV";
    tt2->AddText(PtBinningName2.str().c_str());
    tt2->Draw("same");

    CMS_lumi(c2,2,33);
    
    c2->SetGridy(1);
    c2->RedrawAxis();
    c2->Update();

  }

}
