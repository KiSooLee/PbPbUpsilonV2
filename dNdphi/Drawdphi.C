//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>
#include <TString.h>
#include <iostream>
#include <TMath.h>
#include "Upsilon.h"
#include "Style_Kisoo.h"
//}}}

void Drawdphi(Int_t iVar = 3, const Int_t ndphi = 3, TString version = "v1")
{
	TFile* fin[narr-1][ndphi];
	TCanvas* cdphi[narr-1][3];
	TH1D* hdphi[narr-1][3];
	TH1D* hYield[narr-1][ndphi];
	TFile* fout = new TFile(Form("dphi_fit_%s_%dbin_%s.root", VarName[iVar].Data(), narr-1, version.Data()), "RECREATE");
	for(Int_t iarr = 0; iarr < narr-1; iarr++)
	{
//Define canvas and histogram{{{
		for(Int_t iS = 0; iS < 3; iS++)
		{
			cdphi[iarr][iS] = new TCanvas(Form("cdphi_%d_%d", iarr, iS), "", 0, 0, 600, 600);
			hdphi[iarr][iS] = new TH1D(Form("hdphi_%d_%d", iarr, iS), "", ndphi, 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Sumw2();
		}
//}}}

		for(Int_t idphi = 0; idphi < ndphi; idphi++)
		{
//Get Yield{{{
			fin[iarr][idphi] = new TFile(Form("GetYield/Yield_%s_%dbin_%dth_%dndphi_%dth_dphi_%s.root", VarName[iVar].Data(), narr-1, iarr, ndphi, idphi, version.Data()), "READ");
			hYield[iarr][idphi] = (TH1D*) fin[iarr][idphi]->Get("hYield");
			for(Int_t iS = 0; iS < 3; iS++)
			{
				hdphi[iarr][iS]->SetBinContent(idphi+1, hYield[iarr][idphi]->GetBinContent(iS+1));
				hdphi[iarr][iS]->SetBinError(idphi+1, hYield[iarr][idphi]->GetBinError(iS+1));
			}
//}}}
		}
	}

	gStyle->SetOptStat(0000);
	fout->cd();
	for(Int_t iarr = 0; iarr < narr; iarr++)
	{
		for(Int_t iS = 0; iS < 3; iS++)
		{
//Fit dphi Yield{{{
			cdphi[iarr][iS]->cd();
			hdphi[iarr][iS]->Scale(hdphi[iarr][iS]->Integral());
			hdphi[iarr][iS]->Draw();
			TF1* v2Fit = new TF1(Form("v2Fit_%d_%d", iarr, iS), "[1]*(1+2*[0]*TMath::Cos(2.0*x))", 0, TMath::PiOver2());
			v2Fit->SetParLimits(0, 0., 1);
			v2Fit->SetParameter(0, 40);
			v2Fit->SetParameter(1, 20);
			v2Fit->SetLineColor(kMagenta+2);
			v2Fit->SetLineStyle(2);
			v2Fit->SetLineWidth(4);
			hdphi[iarr][iS]->Fit(v2Fit, "q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "L q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "L q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "L q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "q m", "", 0, TMath::Pi()/2);
			cdphi[iarr][iS]->SaveAs(Form("dphi_fit_%s_%dbin_%dth_%dS.pdf", VarName[iVar].Data(), narr-1, iarr, iS+1));
			hdphi[iarr][iS]->Write();
			v2Fit->Write();
//}}}
		}
	}
}
