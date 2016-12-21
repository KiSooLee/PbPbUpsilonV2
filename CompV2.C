//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <iostream>
#include <TString.h>
#include "Upsilon.h"
#include "Style_Kisoo.h"
//}}}

void CompV2(Int_t iVar = 0)
{
	TFile* fin1S = new TFile(Form("V2_distribution_%s_%dbin_v2.root", VarName[iVar].Data(), narr-1), "READ");
	TFile* fin2S = new TFile(Form("V2_distribution_%s_%dbin_v2.root", VarName[iVar].Data(), narr-1), "READ");
	TCanvas* c1 = new TCanvas("c1", "", 0, 0, 600, 600);
	TH1D* h1S = (TH1D*) fin1S->Get("hV2_0");
	TH1D* h2S = (TH1D*) fin2S->Get("hV2_1");

	gStyle->SetOptStat(0000);
	c1->cd();
	TH1D* h1S_Draw = (TH1D*) h1S->Clone("h1S_Draw");
	TH1D* h2S_Draw = (TH1D*) h2S->Clone("h2S_Draw");
	h1S_Draw->SetAxisRange(-0.2, 0.8, "Y");
	h1S_Draw->SetMarkerStyle(21);
	h1S_Draw->SetMarkerSize(1);
	h1S_Draw->SetMarkerColor(1);
	h1S_Draw->SetLineColor(1);
	h2S_Draw->SetMarkerStyle(21);
	h2S_Draw->SetMarkerSize(1);
	h2S_Draw->SetMarkerColor(2);
	h2S_Draw->SetLineColor(2);
	h1S_Draw->Draw("pe");
	h2S_Draw->Draw("samepe");
	TLegend* leg = new TLegend(0.7, 0.8, 0.9, 0.9);
	leg->AddEntry(h1S_Draw, "1S", "pe");
	leg->AddEntry(h2S_Draw, "2S", "pe");
	leg->Draw();
	c1->SaveAs(Form("v2Comp_%s_v2_v2.pdf", VarName[iVar].Data()));
}
