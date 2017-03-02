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
#include "../Upsilon.h"
#include "../Style_Kisoo.h"
//}}}

void Drawdphi(Int_t iVar = 3, const Int_t narr = 3, const Int_t ndphi = 3, TString version = "v1")
{
	TFile* fin[narr-1][ndphi];
	TCanvas* cdphiun[narr-1][3];
	TCanvas* cdphi[narr-1][3];
	SetStyle();
	TH1D* hdphiun[narr-1][3];
	TH1D* hdphi[narr-1][3];
	TH1D* hYield[narr-1][ndphi];
	TFile* fout = new TFile(Form("dphi_fit_%s_%s.root", VarName[iVar].Data(), version.Data()), "RECREATE");
	TLatex* lt = new TLatex();
	FormLatex(lt, 12, 0.05);

	for(Int_t iarr = 0; iarr < narr-1; iarr++)
	{
//Set target variable{{{
		Double_t BinsArr[2] = {0};
		if(iVar == 0)
		{
			BinsArr[0] = rapBinsArr[iarr];
			BinsArr[1] = rapBinsArr[iarr+1];
		}
		else if(iVar == 1)
		{
			BinsArr[0] = CentBinsArr[iarr];
			BinsArr[1] = CentBinsArr[iarr+1];
		}
		else if(iVar == 2)
		{
			BinsArr[0] = ptBinsArr[iarr];
			BinsArr[1] = ptBinsArr[iarr+1];
		}
//}}}

//Define canvas and histogram{{{
		for(Int_t iS = 0; iS < 3; iS++)
		{
			cdphiun[iarr][iS] = new TCanvas(Form("cdphiun_%d_%dS", iarr, iS+1), "", 0, 0, 600, 600);
			cdphi[iarr][iS] = new TCanvas(Form("cdphi_%d_%dS", iarr, iS+1), "", 0, 0, 600, 600);
			hdphiun[iarr][iS] = new TH1D(Form("hdphiun_%s_%.1f_%.1f_%dS", VarName[iVar].Data(), BinsArr[0], BinsArr[1], iS+1), "", ndphi, 0, TMath::Pi()/2);
			hdphi[iarr][iS] = new TH1D(Form("hdphi_%s_%.1f_%.1f_%dS", VarName[iVar].Data(), BinsArr[0], BinsArr[1], iS+1), "", ndphi, 0, TMath::Pi()/2);
			FormTH1Marker(hdphiun[iarr][iS], 0, 0, 1.5);
			FormTH1Marker(hdphi[iarr][iS], 0, 0, 1.5);
		}
//}}}

		for(Int_t idphi = 0; idphi < ndphi; idphi++)
		{
//Get Yield{{{
			fin[iarr][idphi] = new TFile(Form("../GetYield/Yield_%s_%dbin_%dndphi_%s.root", VarName[iVar].Data(), narr-1, ndphi, version.Data()), "READ");
			hYield[iarr][idphi] = (TH1D*) fin[iarr][idphi]->Get(Form("hYield_%s_%.1f_%.1f_%d", VarName[iVar].Data(), BinsArr[0], BinsArr[1], idphi));

			for(Int_t iS = 0; iS < 3; iS++)
			{
				hdphiun[iarr][iS]->SetBinContent(idphi+1, hYield[iarr][idphi]->GetBinContent(iS+1));
				hdphiun[iarr][iS]->SetBinError(idphi+1, hYield[iarr][idphi]->GetBinError(iS+1));
				hdphi[iarr][iS]->SetBinContent(idphi+1, hYield[iarr][idphi]->GetBinContent(iS+1));
				hdphi[iarr][iS]->SetBinError(idphi+1, hYield[iarr][idphi]->GetBinError(iS+1));
			}
//}}}
		}
	}

	gStyle->SetOptStat(0000);
	fout->cd();
	for(Int_t iarr = 0; iarr < narr-1; iarr++)
	{
		for(Int_t iS = 0; iS < 3; iS++)
		{
//Fit dphi Yield{{{

//Un normalized{{{
			cdphiun[iarr][iS]->cd();
			//hdphiun[iarr][iS]->SetTitle(Form("%dS", iS+1));
			hdphiun[iarr][iS]->SetXTitle("|#phi^{#Upsilon} - #Psi_{2}| (rad)");
			hdphiun[iarr][iS]->SetYTitle("#frac{dN}{d#phi}(rad^{-1})");
			hdphiun[iarr][iS]->SetMinimum(0);
			hdphiun[iarr][iS]->Draw();
			cdphiun[iarr][iS]->SaveAs(Form("dphi_unnormal_%s_%dbin_%dth_%dS_%s.pdf", VarName[iVar].Data(), narr-1, iarr, iS+1, version.Data()));
//}}}

//normalized{{{
			cdphi[iarr][iS]->cd();
			hdphi[iarr][iS]->Scale(1./hdphi[iarr][iS]->Integral());
			//hdphi[iarr][iS]->SetTitle(Form("%dS", iS+1));
			hdphi[iarr][iS]->SetXTitle("|#phi^{#Upsilon} - #Psi_{2}| (rad)");
			hdphi[iarr][iS]->SetYTitle("#frac{1}{N_{#Upsilon}} #frac{dN}{d#phi}(rad^{-1})");
			hdphi[iarr][iS]->SetMaximum(0.6);
			hdphi[iarr][iS]->SetMinimum(0.0);
			hdphi[iarr][iS]->Draw("E1");

			TF1* v2Fit = new TF1(Form("v2Fit_%d_%d", iarr, iS), "[1]*(1+2*[0]*TMath::Cos(2.0*x))", 0, TMath::PiOver2());
			//v2Fit->SetParLimits(0, 0., 1);
			//v2Fit->SetParameter(0, 40);
			//v2Fit->SetParameter(1, 20);
			v2Fit->SetLineColor(kViolet+2);
			v2Fit->SetLineStyle(2);
			v2Fit->SetLineWidth(2);
			hdphi[iarr][iS]->Fit(v2Fit, "rqm", "", 0, TMath::Pi()/2);
/*
			hdphi[iarr][iS]->Fit(v2Fit, "q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "L q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "L q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "L q", "", 0, TMath::Pi()/2);
			hdphi[iarr][iS]->Fit(v2Fit, "q m", "", 0, TMath::Pi()/2);
*/

			lt->DrawLatex(0.52, 0.20, Form("v_{2}^{obs}= %.3f #pm %.3f", v2Fit->GetParameter(0), v2Fit->GetParError(0)));
			lt->DrawLatex(0.18, 0.84, Form("#Upsilon (%dS)", iS+1));
			if(iVar == 0)
			{
				lt->DrawLatex(0.52, 0.78, Form("%.1f<|y|<%.1f", rapBinsArr[iarr], rapBinsArr[iarr+1]));
				lt->DrawLatex(0.52, 0.84, "Cent. 10-60 %");
			}
			else if(iVar == 1)
			{
				lt->DrawLatex(0.52, 0.84, Form("Cent. %d - %d", (int)(CentBinsArr[iarr]/2), (int)(CentBinsArr[iarr+1]/2)));
				lt->DrawLatex(0.52, 0.78, "|y|<2.4");
			}
			else
			{
				lt->DrawLatex(0.52, 0.72, Form("%.1f<p_{T}<%.1f", ptBinsArr[iarr], ptBinsArr[iarr+1]));
				lt->DrawLatex(0.52, 0.78, "|y|<2.4");
				lt->DrawLatex(0.52, 0.84, "Cent. 10-60 %");
			}
			lt->DrawLatex(0.15, 0.935, "CMS Preliminary");
			lt->DrawLatex(0.52, 0.935, "PbPb #sqrt{s_{NN}} = 5.02 TeV");

			cdphi[iarr][iS]->SaveAs(Form("dphi_fit_%s_%dbin_%dth_%dS_%s.pdf", VarName[iVar].Data(), narr-1, iarr, iS+1, version.Data()));
//}}}

			hdphi[iarr][iS]->Write();
			v2Fit->Write();
//}}}
		}
	}
}
