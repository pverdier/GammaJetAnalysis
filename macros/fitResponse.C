#include <string>
#include <vector>
#include <iostream>

#include "../ptBinning.cpp"
#include "../binning.h"

#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"

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

  h->Fit("gaus","QR0","",xmin,xmax);
  TF1 *fres = h->GetFunction("gaus");

  fitResult aFitRes;

  aFitRes.chi2 = fres->GetChisquare();
  aFitRes.ndf  = fres->GetNDF();
  
  aFitRes.mean  = fres->GetParameter(1);
  aFitRes.emean = fres->GetParError(1);
  
  aFitRes.sigma  = fres->GetParameter(2);
  aFitRes.esigma = fres->GetParError(2);

  return aFitRes;
}


void fitResponse()
{ 

  // Prepare the result:
  
  vector<TGraphErrors*> vGraph_Mean_MC_TOT;
  vector<TGraphErrors*> vGraph_Sigma_MC_TOT;

  // data/MC Rmpf : response_Zone_Pt/Rmpf/Rmpf_zone1Q_ptPhot_40_100
  // MC Rtrue: response_Zone_Pt/Rtrue/Rtrue_zone1Q_ptPhot_40_100

  TFile* f = new TFile("PhotonJet_MC_TOT_PFlowAK5chs.root");
  //TFile* f = new TFile("PhotonJet_MC_G_PFlowAK5chs.root");
  //TFile* f = new TFile("PhotonJet_MC_QCD_PFlowAK5chs.root");
  //TFile* f = new TFile("PhotonJet_Photon_Run2012.root");
 
  ptBinning aPtBinning(1);

  const int nZone = getZoneNumber();
  double x[nZone];
  double ex[nZone];
  double y1[nZone];
  double ey1[nZone];
  double y2[nZone];
  double ey2[nZone];

  // Loop on the pT Bins
  for (int j=0; j<aPtBinning.getSize(); ++j) {
    
    
    // Loop on the flavour zones
    for (int i=0; i<nZone; ++i) {
      
      //string histoName = "response_Zone_Pt/Rmpf/Rmpf_"+getZoneBinName(i)+"_"+aPtBinning.getName(j);
      string histoName = "response_Zone_Pt/Rtrue/Rtrue_"+getZoneBinName(i)+"_"+aPtBinning.getName(j);
      cout << " => processing histoName = " << histoName << endl;
      
      TH1F* h = (TH1F*)f->Get(histoName.c_str());  
      fitResult res = fitGaus(h,0.,2.);
      
      res.print();
      
      x[i]  = (double)i+1; ex[i] = 0.;
      y1[i]  = res.mean;  ey1[i] = res.emean;
      y2[i]  = res.sigma; ey2[i] = res.esigma;
      
    } // end loop flavour zone

    vGraph_Mean_MC_TOT.push_back(new TGraphErrors(nZone,x,y1,ex,ey1));
    vGraph_Sigma_MC_TOT.push_back(new TGraphErrors(nZone,x,y2,ex,ey2));
    
    vGraph_Mean_MC_TOT[j]->SetMarkerStyle(20);
    vGraph_Mean_MC_TOT[j]->SetMarkerColor(2);
    vGraph_Mean_MC_TOT[j]->SetMarkerSize(1.5);
    vGraph_Mean_MC_TOT[j]->SetLineColor(4);
    string title = "R_{MPF} for pt bin = " + aPtBinning.getName(j);
    vGraph_Mean_MC_TOT[j]->SetTitle(title.c_str());

    vGraph_Mean_MC_TOT[j]->SetMaximum(1.05);
    vGraph_Mean_MC_TOT[j]->SetMinimum(0.95);
 
    vGraph_Sigma_MC_TOT[j]->SetMarkerStyle(20);
    vGraph_Sigma_MC_TOT[j]->SetMarkerColor(2);
    vGraph_Sigma_MC_TOT[j]->SetMarkerSize(1.5);
    vGraph_Sigma_MC_TOT[j]->SetLineColor(4);
    
  } // end loop pT Bins
  

  // Do the plots :
  //---------------

   for (int j=0; j<aPtBinning.getSize(); ++j) {
     
     TCanvas* c1 = new TCanvas();
     vGraph_Mean_MC_TOT[j]->Draw("APE");

   }

}
