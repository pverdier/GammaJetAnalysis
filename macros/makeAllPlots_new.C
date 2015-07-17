#include <vector>

void makeAllPlots_new()
{

  // Initialization

  gROOT->ProcessLine(".L ../macros/GJetsStudy_cpp.so");
  GJetsStudy ana;

  gROOT->ProcessLine(".L ../macros/my2DTaggingMethod_4x4_new_C.so");
  
  // Parameters

  bool toLumi = true;
  TString format = ".pdf";
  TString out = "result/";
  gROOT->ProcessLine(TString(".! mkdir ")+out);
  
  bool doMCplots = true;
  bool doTagger  = true;
  bool doFfrac   = true;
  bool doRplots  = true;
  bool doMatrix  = true;

  bool doLatex = true;
  
  //--------------------------------------------------------------------------
  // Data-MC plots per zone
  //--------------------------------------------------------------------------

  vector<TString> allTypes;
  allTypes.push_back("Inclusive");
  allTypes.push_back("zone1Q");
  allTypes.push_back("zone2G");
  allTypes.push_back("zone3C");
  allTypes.push_back("zone4B");
  allTypes.push_back("zone5QG");
  allTypes.push_back("zone6LC");

  if ( doMCplots ) {

  for (int i=0; i<allTypes.size(); ++i ) {

  TString type = allTypes[i];

  TString prefixe = type + "/" + type + "_";

  TString hName = "P_PT";
  ana.plotInd(prefixe+hName,toLumi,4,"p_{t}^{#gamma} [GeV/c]","right");
  setLogy(1);
  ana.setXaxisRange(ana._hMC_G,0.,800.,true);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "P_ETA";
  ana.plotInd(prefixe+hName,toLumi,1,"#eta^{#gamma}","right");
  ana.setXaxisRange(ana._hMC_G,-2.,2.,true);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "J1_PT";
  ana.plotInd(prefixe+hName,toLumi,4,"p_{t}^{j1} [GeV/c]","right");
  setLogy(1);
  ana.setXaxisRange(ana._hMC_G,0.,800.,true);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "J1_ETA";
  ana.plotInd(prefixe+hName,toLumi,1,"#eta^{j1}","right");
  ana.setXaxisRange(ana._hMC_G,-2.,2.,true);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "J2_PT";
  ana.plotInd(prefixe+hName,toLumi,1,"p_{t}^{j2} [GeV/c]","right");
  setLogy(1);
  ana.setXaxisRange(ana._hMC_G,0.,250.,true);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "J2_ETA";
  ana.plotInd(prefixe+hName,toLumi,1,"#eta^{j2}","right");
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "M_PT";
  ana.plotInd(prefixe+hName,toLumi,1,"#slash{E}_{t} [GeV]","right");
  setLogy(1);
  ana.setXaxisRange(ana._hMC_G,0.,500.,true);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "Alpha";
  ana.plotInd(prefixe+hName,toLumi,1,"#alpha","right");
  ana.setXaxisRange(ana._hMC_G,0.,0.5,true);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "Rmpf";
  ana.plotInd(prefixe+hName,toLumi,1,"R_{MPF}","right");
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+hName+".C");

  TString hName = "DeltaPhi_j1gamma";
  ana.plotInd(prefixe+hName,toLumi,1,"#Delta#phi (j_{1},#gamma) [radians]","l");
  ana.setXaxisRange(ana._hMC_G,2.7,3.2,true);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "J1_QGTAGGER";
  ana.plotInd(prefixe+hName,toLumi,1,"QGL Tagger j_{1}","right");
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  TString hName = "J1_BTAGGER";
  ana.plotInd(prefixe+hName,toLumi,1,"CSV Tagger j_{1}","right");
  setLogy(1);
  c1->Update();
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+format.Data());
  c1->SaveAs(out+"datamc_Ind_"+type+"_"+hName+".C");

  } // end for
  } // endif

  //--------------------------------------------------------------------------
  // Tagger plots per ptBin
  //--------------------------------------------------------------------------

  if ( doTagger ) {
  
  ana.drawTagger("CSV",0,1,0);
  setLogy(1);
  ana._pad_hi->GetCanvas()->Update();
  ana._pad_hi->GetCanvas()->SaveAs(out+"CSV_MC_TOT_ptPhot_40_100"+format.Data());
  ana._pad_hi->GetCanvas()->SaveAs(out+"CSV_MC_TOT_ptPhot_40_100"+".C");

  ana.drawTagger("CSV",1,1,0);
  setLogy(1);
  ana._pad_hi->GetCanvas()->Update();
  ana._pad_hi->GetCanvas()->SaveAs(out+"CSV_MC_TOT_ptPhot_100_200"+format.Data());
  ana._pad_hi->GetCanvas()->SaveAs(out+"CSV_MC_TOT_ptPhot_100_200"+".C");

  ana.drawTagger("CSV",2,1,0);
  setLogy(1);
  ana._pad_hi->GetCanvas()->Update();
  ana._pad_hi->GetCanvas()->SaveAs(out+"CSV_MC_TOT_ptPhot_200_800"+format.Data());
  ana._pad_hi->GetCanvas()->SaveAs(out+"CSV_MC_TOT_ptPhot_200_800"+".C");

  ana.drawTagger("QGL",0,1,0);
  ana._hMC_TOT->SetMinimum(0.);
  ana._pad_hi->GetCanvas()->Update();
  ana._pad_hi->GetCanvas()->SaveAs(out+"QGL_MC_TOT_ptPhot_40_100"+format.Data());
  ana._pad_hi->GetCanvas()->SaveAs(out+"QGL_MC_TOT_ptPhot_40_100"+".C");

  ana.drawTagger("QGL",1,1,0);
  ana._hMC_TOT->SetMinimum(0.);
  ana._pad_hi->GetCanvas()->Update();
  ana._pad_hi->GetCanvas()->SaveAs(out+"QGL_MC_TOT_ptPhot_100_200"+format.Data());
  ana._pad_hi->GetCanvas()->SaveAs(out+"QGL_MC_TOT_ptPhot_100_200"+".C");

  ana.drawTagger("QGL",2,1,0);
  ana._hMC_TOT->SetMinimum(0.);
  ana._pad_hi->GetCanvas()->Update();
  ana._pad_hi->GetCanvas()->SaveAs(out+"QGL_MC_TOT_ptPhot_200_800"+format.Data());
  ana._pad_hi->GetCanvas()->SaveAs(out+"QGL_MC_TOT_ptPhot_200_800"+".C");
  
  } // endif

  //--------------------------------------------------------------------------
  // Flavour fraction
  //--------------------------------------------------------------------------

  if ( doFfrac ) {

  ana.drawVarFlavour("Gammapt",-1,"p_{t}^{#gamma} [GeV/c]",1,0);
  setLogy(1);
  gPad->Update();
  gPad->GetCanvas()->SaveAs(out+"FlavourDist_MC_TOT_P_PT_inclusive"+format.Data());
  gPad->GetCanvas()->SaveAs(out+"FlavourDist_MC_TOT_P_PT_inclusive"+".C");

  ana.doFlavourRatio("p_{t}^{#gamma} [GeV/c]");
  gPad->Update();
  gPad->GetCanvas()->SaveAs(out+"FlavourFrac_MC_TOT_P_PT_inclusive"+format.Data());
  gPad->GetCanvas()->SaveAs(out+"FlavourFrac_MC_TOT_P_PT_inclusive"+".C");

  for (int i=1; i<allTypes.size(); ++i ) {
  
  TString type = allTypes[i];

  ana.drawVarFlavour("Gammapt",i-1,"p_{t}^{#gamma} [GeV/c]",1,0);
  setLogy(1);
  gPad->Update();
  gPad->GetCanvas()->SaveAs(out+"FlavourDist_MC_TOT_P_PT_"+type+format.Data());
  gPad->GetCanvas()->SaveAs(out+"FlavourDist_MC_TOT_P_PT_"+type+".C");

  ana.doFlavourRatio("p_{t}^{#gamma} [GeV/c]");
  gPad->Update();
  gPad->GetCanvas()->SaveAs(out+"FlavourFrac_MC_TOT_P_PT_"+type+format.Data());
  gPad->GetCanvas()->SaveAs(out+"FlavourFrac_MC_TOT_P_PT_"+type+".C");

  }// end for
  }// endif
  
  //--------------------------------------------------------------------------
  // Response plots
  //--------------------------------------------------------------------------
  
  if ( doRplots ) {

  for (int i=1; i<allTypes.size(); ++i ) {
    
    TString type = allTypes[i];
    
    ana.drawRmpfPerZone(type.Data(),0,1,0);
    ana._pad_hi->GetCanvas()->Update();
    ana._pad_hi->GetCanvas()->SaveAs(out+"Rmpf_MC_TOT_"+type+"_ptPhot_40_100"+format.Data());
    ana._pad_hi->GetCanvas()->SaveAs(out+"Rmpf_MC_TOT_"+type+"_ptPhot_40_100"+".C");

    TString hName = "response_Zone_Pt/Rmpf/Rmpf_"+type+"_ptPhot_40_100";
    ana.plotInd(hName,toLumi,1,"R_{MPF} for "+type+" & ptPhot_40_100","right");
    ana._pad_hi->GetCanvas()->Update();
    ana._pad_hi->GetCanvas()->SaveAs(out+"datamc_Ind_Rmpf"+type+"_ptPhot_40_100"+format.Data());
    ana._pad_hi->GetCanvas()->SaveAs(out+"datamc_Ind_Rmpf"+type+"_ptPhot_40_100"+".C");

    ana.drawRmpfPerZone(type.Data(),1,1,0);
    ana._pad_hi->GetCanvas()->Update();
    ana._pad_hi->GetCanvas()->SaveAs(out+"Rmpf_MC_TOT_"+type+"_ptPhot_100_200"+format.Data());
    ana._pad_hi->GetCanvas()->SaveAs(out+"Rmpf_MC_TOT_"+type+"_ptPhot_100_200"+".C");

    TString hName = "response_Zone_Pt/Rmpf/Rmpf_"+type+"_ptPhot_100_200";
    ana.plotInd(hName,toLumi,1,"R_{MPF} for "+type+" & ptPhot_100_200","right");
    ana._pad_hi->GetCanvas()->Update();
    ana._pad_hi->GetCanvas()->SaveAs(out+"datamc_Ind_Rmpf"+type+"_ptPhot_100_200"+format.Data());
    ana._pad_hi->GetCanvas()->SaveAs(out+"datamc_Ind_Rmpf"+type+"_ptPhot_100_200"+".C");

    ana.drawRmpfPerZone(type.Data(),2,1,0);
    ana._pad_hi->GetCanvas()->Update();
    ana._pad_hi->GetCanvas()->SaveAs(out+"Rmpf_MC_TOT_"+type+"_ptPhot_200_800"+format.Data());
    ana._pad_hi->GetCanvas()->SaveAs(out+"Rmpf_MC_TOT_"+type+"_ptPhot_200_800"+".C");

    TString hName = "response_Zone_Pt/Rmpf/Rmpf_"+type+"_ptPhot_200_800";
    ana.plotInd(hName,toLumi,1,"R_{MPF} for "+type+" & ptPhot_200_800","right");
    ana._pad_hi->GetCanvas()->Update();
    ana._pad_hi->GetCanvas()->SaveAs(out+"datamc_Ind_Rmpf"+type+"_ptPhot_200_800"+format.Data());
    ana._pad_hi->GetCanvas()->SaveAs(out+"datamc_Ind_Rmpf"+type+"_ptPhot_200_800"+".C");

  } // end for
  } // endif

  //--------------------------------------------------------------------------
  // Matrix
  //--------------------------------------------------------------------------

  if ( doMatrix ) {
    
  // With MC_TOT
  my2DTaggingMethod_4x4_new(ana._fMC_TOT,ana._fData,"MC_TOT_");
  
  // With MC_G
  my2DTaggingMethod_4x4_new(ana._fMC_G,ana._fData,"MC_G_");
  
  // With MC_QCD
  my2DTaggingMethod_4x4_new(ana._fMC_QCD,ana._fData,"MC_QCD_");

  }

  //--------------------------------------------------------------------------
  // Do Latex plots
  //--------------------------------------------------------------------------

  if ( doLatex ) {

  gROOT->ProcessLine(".! cp ../macros/makeAllPlots.tex .");
  gROOT->ProcessLine(".! pdflatex makeAllPlots.tex");

  }

}
