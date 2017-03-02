//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>
#include <TString.h>
#include <iostream>
#include "../Upsilon.h"
#include "../Style_Kisoo.h"
//}}}

void DrawV2(Int_t iVar = 0, const Int_t narr = 3, TString version = "v1")
{
	TFile* fin = new TFile(Form("../dNdphi/dphi_fit_%s_%s.root", VarName[iVar].Data(), version.Data()), "READ");
	TFile* fout = new TFile(Form("V2_distribution_%s_%s.root", VarName[iVar].Data(), version.Data()), "RECREATE");

	SetStyle();

//Define canvas and histogram{{{
	TCanvas* c1[3];
	TH1D* hV2[3];
	TLatex* lt = new TLatex();
	FormLatex(lt, 12, 0.050);
	for(Int_t iS = 0; iS < 3; iS++)
	{
		c1[iS] = new TCanvas(Form("c1_%d", iS), "", 0, 0, 600, 600);
		if(iVar == 0) hV2[iS] = new TH1D(Form("hV2_%d", iS), ";|y|;v_{2}", narr-1, rapBinsArr);
		else if(iVar == 1) hV2[iS] = new TH1D(Form("hV2_%d", iS), ";Centrality;v_{2}", narr-1, CentBinsArr2);
		else hV2[iS] = new TH1D(Form("hV2_%d", iS), ";p_{T} (GeV/c);v_{2}", narr-1, ptBinsArr);
		FormTH1Marker(hV2[iS], 0, 0, 1.5);
	}
//}}}

	for(Int_t iarr = 0; iarr < narr-1; iarr++)
	{
		for(Int_t iS = 0; iS < 3; iS++)
		{
			TF1* v2Fit;
			v2Fit = (TF1*) fin->Get(Form("v2Fit_%d_%d", iarr, iS));
			Double_t v2 = v2Fit->GetParameter(0);
			Double_t v2Err = v2Fit->GetParError(0);
			hV2[iS]->SetBinContent(iarr+1, v2);
			hV2[iS]->SetBinError(iarr+1, v2Err);
		}
	}

	gStyle->SetOptStat(0000);
	fout->cd();
	for(Int_t iS = 0; iS < 3; iS++)
	{
		c1[iS]->cd();
		hV2[iS]->GetYaxis()->SetRangeUser(-0.2, 0.4);
		hV2[iS]->Draw();
		lt->DrawLatex(0.15, 0.935, "CMS Preliminary");
		lt->DrawLatex(0.52, 0.935, "PbPb #sqrt{s_{NN}} = 5.02 TeV");
		if(iVar == 0) lt->DrawLatex(0.56, 0.82,"Cent. 0-60 %");
		else if(iVar == 1) lt->DrawLatex(0.56, 0.82,"|y|<2.4");
		else
		{
			lt->DrawLatex(0.56, 0.82,"Cent. 10-60 %");
			lt->DrawLatex(0.56, 0.76,"|y|<2.4");
		}
		lt->DrawLatex(0.18, 0.82, Form("#Upsilon (%dS)", iS+1));
		c1[iS]->SaveAs(Form("V2_distribution_%s_%dS_%s.pdf", VarName[iVar].Data(), iS+1, version.Data()));
		hV2[iS]->Write();
	}
}
