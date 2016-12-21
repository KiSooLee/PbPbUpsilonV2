//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TLatex.h>
#include <iostream>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataSet.h>
#include <RooWorkspace.h>
#include <RooPlot.h>
#include "../Style_Kisoo.h"
#include "../Upsilon.h"
using namespace std;
using namespace RooFit;
//}}}

void MC_weight_check_Roofit(const Int_t iS = 1)
{
	TString Var[4] = {"pt", "y", "phi", "mass"};
	Double_t Varnbin[4] = {100, 100, 150, 300};
	Double_t VarMin[4] = {0, -2.5, -1.5, 8};
	Double_t VarMax[4] = {50, 2.5, 1.5, 14};

//Get Dataset{{{
	TFile* fin = new TFile(Form("/home/kisoo/work/Upsilon/PbPb/SkimmedFiles/Skim_OniaTree_PbPb_MC_%dS.root", iS), "READ");
	//TFile* fin = new TFile(Form("/home/kisoo/work/Upsilon/PbPb/SkimmedFiles/yskimAA_MC_Ups1S_Trig-L1DoubleMu0_OpSign_EP-OppositeHF_20168142122_3c54df0419c4813e2d7256dc8952ac699405d027.root"), "READ");
	RooDataSet* dataset = (RooDataSet*) fin->Get("dataset");
	RooWorkspace* ws = new RooWorkspace(Form("workspace_%dS", iS));
	ws->import(*dataset);
	ws->data("dataset")->Print();

	RooDataSet* initialDS = (RooDataSet*) dataset->reduce(RooArgSet(*(ws->var("mass")), *(ws->var("pt")), *(ws->var("y")), *(ws->var("phi")), *(ws->var("dphi")), *(ws->var("Centrality")), *(ws->var("weight"))));
	//RooDataSet* reducedDS = (RooDataSet*) dataset->reduce(RooArgSet(*(ws->var("mass")), *(ws->var("pt")), *(ws->var("y")), *(ws->var("weight"))));
	initialDS->SetName("initialDS");
	RooDataSet* weightDS = new RooDataSet("weightedDS", "weight dataset", *initialDS->get(), Import(*initialDS), WeightVar(*ws->var("weight")));
	RooDataSet* unweightDS = new RooDataSet("weightedDS", "weight dataset", *initialDS->get(), Import(*initialDS));
	RooDataSet* weightedDS = (RooDataSet*) weightDS->reduce(RooArgSet(*(ws->var("mass")), *(ws->var("pt")), *(ws->var("y")), *(ws->var("phi")), *(ws->var("Centrality")), *(ws->var("dphi"))));
	weightedDS->SetName("weightedDS");
	RooDataSet* unweightedDS = (RooDataSet*) unweightDS->reduce(RooArgSet(*(ws->var("mass")), *(ws->var("pt")), *(ws->var("y")), *(ws->var("phi")), *(ws->var("Centrality")), *(ws->var("dphi"))));
	//RooDataSet* finalDS = (RooDataSet*) weightedDS->reduce(RooArgSet(*(ws->var("mass")), *(ws->var("pt")), *(ws->var("y"))));
	unweightedDS->SetName("unweightedDS");
	ws->import(*weightedDS);
	ws->import(*unweightedDS);
//}}}

	gStyle->SetOptStat(0000);
	TCanvas* c1[4];

	TFile* fout = new TFile(Form("RoofitHist/WeightingCheck_Roofit_%d.root", iS), "RECREATE");
	fout->cd();

	for(Int_t ivar = 0; ivar < 4; ivar++)
	{
		c1[ivar] = new TCanvas(Form("c1_%d", ivar), "", 0, 0, 600, 600);
		ws->var(Form("%s", Var[ivar].Data()))->setRange(VarMin[ivar], VarMax[ivar]);
		ws->var(Form("%s", Var[ivar].Data()))->Print();

		RooPlot* weightPlot = ws->var(Form("%s", Var[ivar].Data()))->frame(Varnbin[ivar]);
		RooPlot* unweightPlot = ws->var(Form("%s", Var[ivar].Data()))->frame(Varnbin[ivar]);
		ws->data("weightedDS")->plotOn(weightPlot, Name("weightPlot"), LineColor(1), MarkerColor(1));
		ws->data("unweightedDS")->plotOn(unweightPlot, Name("unweightPlot"), LineColor(2), MarkerColor(2));
		c1[ivar]->cd();
		RooMaxRange(weightPlot, unweightPlot, 1.3, 0);
		if(ivar == 0) weightPlot->SetXTitle("p_{T} (GeV/c)");
		else if(ivar == 1) weightPlot->SetXTitle("Rapidity");
		else if(ivar == 2) weightPlot->SetXTitle("#phi");
		else weightPlot->SetXTitle("M (GeV/c^2)");
		weightPlot->SetYTitle("Counts");
		weightPlot->Draw();
		unweightPlot->Draw("same");
		TH1D* hweight = new TH1D();
		TH1D* hunweight = new TH1D();
		FormTH1Marker(hweight, color[0], marker[0], 1.4);
		FormTH1Marker(hunweight, color[1], marker[0], 1.4);
		TLegend* leg = new TLegend(0.6, 0.7, 0.9, 0.8);
		FormLegend(leg, 0.04);
		leg->AddEntry(hweight, "Weighted", "pe");
		leg->AddEntry(hunweight, "Unweighted", "pe");
		leg->Draw();
		TLatex* latex = new TLatex();
		FormLatex(latex, 13, 0.06);
		latex->DrawLatex(0.65, 0.85, Form("#Upsilon %dS", iS));
		c1[ivar]->SaveAs(Form("RoofitHist/WeightingCheck_Roofit_%s_%dS.pdf", Var[ivar].Data(), iS));
		weightPlot->Write();;
		unweightPlot->Write();;
	}
	fout->Close();
}
