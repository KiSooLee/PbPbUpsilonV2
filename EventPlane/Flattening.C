//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <iostream>
#include <TLegend.h>
#include <TLatex.h>
#include "../Style_Kisoo.h"
#include "../Upsilon.h"
//}}}

void Flattening()
{
	TFile* fin = new TFile("/home/kisoo/work/Upsilon/PbPb/OniaTree_DoubleMu0ABCD_EvtPlane.root", "READ");
	TTree* tin = (TTree*) fin->Get("hionia/myTree");
	TString plmi[3] = {"mi", "pl", "abs"};

//tree variables{{{
	Int_t Centrality;
	Float_t rpAng[29];
	Float_t rpAngf[29];
//}}}

//Branches{{{
	TBranch* b_Centrality;
	TBranch* b_rpAng;
	TBranch* b_rpAngf;
//}}}

//Branch address{{{
	tin->SetBranchAddress("Centrality", &Centrality, &b_Centrality);
	tin->SetBranchAddress("rpAng", &rpAng, &b_rpAng);
	tin->SetBranchAddress("rpAngf", &rpAngf, &b_rpAngf);
//}}}

//Define histrograms{{{
	TCanvas* c1[Cent_narr][3];
	TH1F* hAng[Cent_narr][3];
	TH1F* hAngf[Cent_narr][3];
	gStyle->SetOptStat(0000);
	for(Int_t icent = 0; icent < Cent_narr; icent++)
	{
		for(Int_t i = 0; i < 3; i++)
		{
			c1[icent][i] = new TCanvas(Form("c1_%d_%d", icent, i), "", 0, 0, 600, 600);
			hAng[icent][i] = new TH1F(Form("hAng_%d_%d", icent, i), "", 50, -1.6, 1.6);
			hAngf[icent][i] = new TH1F(Form("hAngf_%d_%d", icent, i), "", 50, -1.6, 1.6);
			FormTH1(hAng[icent][i], color[0]);
			FormTH1(hAngf[icent][i], color[1]);
		}
	}
//}}}

//Fill angle{{{
	Int_t Nevt = tin->GetEntries();
	for(Int_t ievt = 0; ievt < Nevt; ievt++)
	{
		if(ievt%100000==0) cout << ">>>>> EVENT " << ievt << " / " << Nevt << endl;
		tin->GetEntry(ievt);

		for(Int_t icent = 0; icent < Cent_narr; icent++)
		{
			if(icent < Cent_narr-1)
			{
				if(Centrality >= CentBinsArr[icent] && Centrality < CentBinsArr[icent+1] && rpAng[6] > -9)
				{
					hAng[icent][0]->Fill(rpAng[6]);
					hAng[icent][1]->Fill(rpAng[7]);
					hAng[icent][2]->Fill(rpAng[8]);
					hAngf[icent][0]->Fill(rpAngf[6]);
					hAngf[icent][1]->Fill(rpAngf[7]);
					hAngf[icent][2]->Fill(rpAngf[8]);
				}
			}
			else
			{
				if(Centrality >= 0 && Centrality <= 200 && rpAng[6] > -9)
				{
					hAng[icent][0]->Fill(rpAng[6]);
					hAng[icent][1]->Fill(rpAng[7]);
					hAng[icent][2]->Fill(rpAng[8]);
					hAngf[icent][0]->Fill(rpAngf[6]);
					hAngf[icent][1]->Fill(rpAngf[7]);
					hAngf[icent][2]->Fill(rpAngf[8]);
				}
			}
		}
	}
//}}}

//Draw angle distribution{{{
	for(Int_t icent = 0; icent < Cent_narr; icent++)
	{
		for(Int_t i = 0; i < 3; i++)
		{
			c1[icent][i]->cd();
			MaxRange(hAng[icent][i], hAngf[icent][i], 1.2, 0);
			hAng[icent][i]->SetXTitle("#Psi");
			hAng[icent][i]->SetYTitle("Counts");
			hAng[icent][i]->Draw();
			hAngf[icent][i]->Draw("same");
			TLegend* leg = new TLegend(0.3, 0.2, 0.8, 0.35);
			FormLegend(leg, 0.04);
			leg->AddEntry(hAng[icent][i], "non-Flatten #Psi", "pe");
			leg->AddEntry(hAngf[icent][i], "Flatten #Psi", "pe");
			leg->Draw();
			TLatex* latex = new TLatex();
			FormLatex(latex, 13, 0.06);
			if(icent < Cent_narr-1)
			{
				latex->DrawLatex(0.3, 0.4, Form("%d - %d %s", int(CentBinsArr[icent]/2), int(CentBinsArr[icent+1]/2), "%"));
				c1[icent][i]->SaveAs(Form("EventPlane_Flattening_%d-%d_%s_50bin.pdf", int(CentBinsArr[icent]), int(CentBinsArr[icent+1]), plmi[i].Data()));
			}
			else
			{
				latex->DrawLatex(0.3, 0.4, Form("0 - 100 %s", "%"));
				c1[icent][i]->SaveAs(Form("EventPlane_Flattening_0-200_%s_50bin.pdf", plmi[i].Data()));
			}
		}
	}
//}}}
}
