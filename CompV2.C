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

void CompV2(Int_t iVar = 0, const Int_t narr = 3, TString version1S = "v4", TString version2S = "v4")
{
	TFile* fin1S = new TFile(Form("GetV2/V2_distribution_%s_%s.root", VarName[iVar].Data(), version1S.Data()), "READ");
	TFile* fin2S = new TFile(Form("GetV2/V2_distribution_%s_%s.root", VarName[iVar].Data(), version2S.Data()), "READ");
	TCanvas* c1 = new TCanvas("c1", "", 0, 0, 600, 600);
	TH1D* h1S = (TH1D*) fin1S->Get("hV2_0");
	TH1D* h2S = (TH1D*) fin2S->Get("hV2_1");

	TGraphErrors* g1S = new TGraphErrors();
	TGraphErrors* g2S = new TGraphErrors();

	for(Int_t i1 = 0; i1 < h1S->GetNbinsX(); i1++)
	{
		Double_t x, xE, y, yE;
		x = h1S->GetXaxis()->GetBinCenter(i1+1);
		xE = h1S->GetBinWidth(i1+1)/2;
		y = h1S->GetBinContent(i1+1);
		yE = h1S->GetBinError(i1+1);
		g1S->SetPoint(i1, x, y);
		g1S->SetPointError(i1, xE, yE);
cout << "1S x: " << x << ", y: " << y << endl;
	}
	for(Int_t i2 = 0; i2 < h2S->GetNbinsX(); i2++)
	{
		Double_t x, xE, y, yE;
		x = h2S->GetBinCenter(i2+1);
		xE = h2S->GetBinWidth(i2+1)/2;
		y = h2S->GetBinContent(i2+1);
		yE = h2S->GetBinError(i2+1);
		g2S->SetPoint(i2, x, y);
		g2S->SetPointError(i2, xE, yE);
	}

	gStyle->SetOptStat(0000);
	c1->cd();
	g1S->GetXaxis()->SetTitle(Form("%s", VarName[iVar].Data()));
	g1S->GetYaxis()->SetTitle("v2");
	g1S->SetMaximum(0.25);
	g1S->SetMinimum(-0.1);
	if(iVar == 0) g1S->GetXaxis()->SetRangeUser(rapBinsArr[0], rapBinsArr[narr-1]);
	else if(iVar == 1) g1S->GetXaxis()->SetRangeUser(CentBinsArr[0], CentBinsArr[narr-1]);
	else g1S->GetXaxis()->SetRangeUser(ptBinsArr[0], ptBinsArr[narr-1]);
	g1S->SetMarkerStyle(21);
	g1S->SetMarkerSize(1);
	g1S->SetMarkerColor(1);
	g1S->SetLineColor(1);
	g2S->SetMarkerStyle(25);
	g2S->SetMarkerSize(1);
	g2S->SetMarkerColor(2);
	g2S->SetLineColor(2);
	g1S->Draw("ap");
	g2S->Draw("samep");
	TLegend* leg = new TLegend(0.7, 0.8, 0.9, 0.9);
	leg->AddEntry(g1S, "1S", "pe");
	leg->AddEntry(g2S, "2S", "pe");
	leg->Draw();
	c1->SaveAs(Form("v2Comp_%s_%s_%s.pdf", VarName[iVar].Data(), version1S.Data(), version2S.Data()));
}
