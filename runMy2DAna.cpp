#include "My2DAna.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"

#include "StdArg.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

void Usage(const char* exename) {
  cout << "Usage : runMy2DAna -debug -filelist <data.list> -out <output.root> -num <Nevent> -skim -mc" << endl;
}

int main(int argc, char *argv[])
{

// parse executable options :
//___________________________

  StdArg arg(argc,argv);
 
  cout << "======================================================================" << endl;
  for (int i=0;i<argc;i++)
    cout << argv[i] << " ";
  cout << endl;
  cout << "======================================================================" << endl;
   
  // enter all possible flags:
  arg.flags << "-debug" << "-skim" << "-mc" << "-doraw" << "-domc";
 
  // enter all possible keys:
  arg.keys << "-filelist" << "-out" << "-num";
 
  bool debug  = false;
  bool skim   = false;
  bool ismc   = false;
  bool domc   = false;
  bool doraw  = false;

  string input  = "data.list";
  string output = "output_dummy.root";
  int nevent = -1;

  try {  // parse command line
    arg.Process();
    debug = arg.Flag("-debug") ? true: false;
    skim  = arg.Flag("-skim")  ? true: false;
    ismc  = arg.Flag("-mc")    ? true: false;
    domc  = arg.Flag("-domc")  ? true: false;
    doraw = arg.Flag("-doraw") ? true: false;
    if ( arg.Key("-filelist")    ) input  = arg.Get<string>("-filelist");
    if ( arg.Key("-out")         ) output = arg.Get<string>("-out");    
    if ( arg.Key("-num")         ) nevent = arg.Get<int>("-num");
 }
  catch (StdArg::BadInput) {
    if (argc > 1) cout<< "Input error" <<endl;
    // usage in case of error or no parameters
    Usage(argv[0]);
    return 1;
  }

//
// Load ntuples or TCHAIN :
//_________________________

  TChain *first_jetChain = new TChain("first_jet");
  TChain *second_jetChain = new TChain("second_jet");
  TChain *photonChain = new TChain("photon");
  TChain *metChain = new TChain("met");
  TChain *electronsChain = new TChain("electrons");
  TChain *muonsChain = new TChain("muons");
  TChain *miscChain = new TChain("misc");
  TChain *rhoChain = new TChain("rho");
  
  TChain *first_jet_rawChain = new TChain("first_jet_raw");
  TChain *second_jet_rawChain = new TChain("second_jet_raw");
  TChain *met_rawChain = new TChain("met_raw");
  
  TChain *first_jet_genChain = new TChain("first_jet_gen");
  TChain *second_jet_genChain = new TChain("second_jet_gen");
  TChain *met_genChain = new TChain("met_gen");
  TChain *photon_genChain = new TChain("photon_gen");

  TString fNameList = input.c_str();
  cout << "=> Load Chain from file: " << fNameList << endl;
  ifstream fList(fNameList.Data());
  if (!fList) {
    cout << "=> Can't open file " << fNameList << endl;
    return 1;
  }

  std::string lineFromFile;
  do {
    std::getline(fList, lineFromFile);
    if (lineFromFile=="") break;

    if(first_jetChain->Add(lineFromFile.c_str())) cout << "=>File '"
						       << lineFromFile
						       << "' has been loaded" << endl;
    else cout << ">>Can't load file '" << lineFromFile << "'" << endl;

    second_jetChain->Add(lineFromFile.c_str());
    photonChain->Add(lineFromFile.c_str());
    metChain->Add(lineFromFile.c_str());
    electronsChain->Add(lineFromFile.c_str());
    muonsChain->Add(lineFromFile.c_str());
    miscChain->Add(lineFromFile.c_str());
    rhoChain->Add(lineFromFile.c_str());
    if ( doraw ) {
      first_jet_rawChain->Add(lineFromFile.c_str());
      second_jet_rawChain->Add(lineFromFile.c_str());
      met_rawChain->Add(lineFromFile.c_str());
    }
    if ( domc && ismc ) {    
      first_jet_genChain->Add(lineFromFile.c_str());
      second_jet_genChain->Add(lineFromFile.c_str());
      met_genChain->Add(lineFromFile.c_str());
      photon_genChain->Add(lineFromFile.c_str());
    }
  } while (fList.good());
  fList.close();

  cout << "======================================================================" << endl;
  cout << "Total Number of events = "  << first_jetChain->GetEntries() << endl;
  cout << "======================================================================" << endl;

  My2DAna ana(domc,doraw);

  ana.Init(first_jetChain,second_jetChain,photonChain,metChain,electronsChain,muonsChain,miscChain,rhoChain,
	   first_jet_rawChain,second_jet_rawChain,met_rawChain,
	   first_jet_genChain,second_jet_genChain,met_genChain,photon_genChain);

  ana.SetNevent(nevent);// configure the object of type MyAna
  ana.SetRootName(output);
  ana.SetDebugMode(debug);
  ana.SetDoSkim(skim);
  ana.SetMCmode(ismc);
  cout << "======================================================================" << endl;
  
  cout << "Calling My2DAna::Loop()..." << endl;
  ana.Loop();

  return 0;
}
