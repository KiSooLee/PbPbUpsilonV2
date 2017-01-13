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
	TCanvas* c1[3];
	TH1D* hV2[3];
	for(Int_t iS = 0; iS < 3; iS++)
	{
		c1[iS] = new TCanvas(Form("c1_%d", iS), "", 0, 0, 600, 600);
		if(iVar == 0) hV2[iS] = new TH1D(Form("hV2_%d", iS), "", narr-1, rapBinsArr);
		else if(iVar == 1) hV2[iS] = new TH1D(Form("hV2_%d", iS), "", narr-1, CentBinsArr);
		else hV2[iS] = new TH1D(Form("hV2_%d", iS), "", narr-1, ptBinsArr);
	}

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
		hV2[iS]->Draw();
		c1[iS]->SaveAs(Form("V2_distribution_%s_%dS_%s.pdf", VarName[iVar].Data(), iS+1, version.Data()));
		hV2[iS]->Write();
	}
}
