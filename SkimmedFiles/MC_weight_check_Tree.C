//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TString.h>
#include <iostream>
#include <../Upsilon.h>
#include <../Style_Kisoo.h>
//}}}

void MC_weight_check_Tree(Int_t iS = 1)
{
	TChain* TReco = new TChain("UpsilonTree");
	TReco->Add(Form("Skim_OniaTree_PbPb_MC_%dS.root", iS));
	//TChain* TReco = new TChain("mm");
//	TReco->Add("yskimAA_MC_Ups1S_Trig-L1DoubleMu0_OpSign_EP-OppositeHF_20168142122_3c54df0419c4813e2d7256dc8952ac699405d027.root");

	TString Var[4] = {"pt", "y", "phi", "mass"};
	Double_t Varnbin[4] = {100, 100, 150, 300};
	Double_t VarMin[4] = {0, -2.5, -1.5, 8};
	Double_t VarMax[4] = {50, 2.5, 1.5, 14};

	TCanvas* c1[4];
	TH1F* hweight[4];
	TH1F* hunweight[4];
	gStyle->SetOptStat(0000);

	TFile* fout = new TFile(Form("TreeHist/WeightingCheck_Tree_%d.root", iS), "RECREATE");
	fout->cd();

	for(Int_t i = 0; i < 4; i++)
	{
		c1[i] = new TCanvas("c1", "", 0, 0, 600, 600);
		hweight[i] = new TH1F(Form("hweight_%s", Var[i].Data()), "", Varnbin[i], VarMin[i], VarMax[i]);
		hunweight[i] = new TH1F(Form("hunweight_%s", Var[i].Data()), "", Varnbin[i], VarMin[i], VarMax[i]);
		TReco->Draw(Form("%s>>hweight_%s", Var[i].Data(), Var[i].Data()), "weight");
		TReco->Draw(Form("%s>>hunweight_%s", Var[i].Data(), Var[i].Data()), "");

		FormTH1(hweight[i], color[0]);
		FormTH1(hunweight[i], color[1]);

		c1[i]->cd();
		MaxRange(hweight[i], hunweight[i], 1.3, 0);
		if(i == 0) hweight[i]->SetXTitle("p_{T} (GeV/c)");
		else if(i == 1) hweight[i]->SetXTitle("Rapidity");
		else if(i == 2) hweight[i]->SetXTitle("#phi");
		else hweight[i]->SetXTitle("M (GeV/c^2)");
		hweight[i]->SetYTitle("Counts");
		hweight[i]->Draw("hist");
		hunweight[i]->Draw("hist same");
		TLegend* leg = new TLegend(0.6, 0.7, 0.9, 0.8);
		FormLegend(leg, 0.04);
		leg->AddEntry(hweight[i], "Weighted", "l");
		leg->AddEntry(hunweight[i], "Unweighted", "l");
		leg->Draw();
		TLatex* latex = new TLatex();
		FormLatex(latex, 13, 0.06);
		latex->DrawLatex(0.65, 0.85, Form("#Upsilon %dS", iS));
		c1[i]->SaveAs(Form("TreeHist/WeightingCheck_Tree_%s_%dS.pdf", Var[i].Data(), iS));
		hweight[i]->Write();
		hunweight[i]->Write();
	}
	fout->Close();
}
