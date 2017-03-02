//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
#include <TParameter.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataSet.h>
#include <RooWorkspace.h>
#include <RooPlot.h>
#include <RooHist.h>
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooChebychev.h>
#include <RooPolynomial.h>
#include <RooExponential.h>
#include <RooGenericPdf.h>
#include <RooAddPdf.h>
#include <RooFitResult.h>
#include <RooFormulaVar.h>
#include "../Style_Kisoo.h"
#include "../Upsilon.h"
using namespace std;
using namespace RooFit;
//}}}

void GetMCParameters(const Int_t iVar = 1, const Int_t narr = 3, const Int_t iS = 1, TString version = "v1")
{
	const Int_t Nmassbins = 120;
	Double_t Up_mass;
	if(iS == 1) Up_mass = U1S_mass;
	else if(iS == 2) Up_mass = U2S_mass;
	else Up_mass = U3S_mass;

//Files{{{
	TFile* fin = new TFile(Form("/home/kisoo/work/Upsilon/PbPb/SkimmedFiles/Skim_OniaTree_PbPb_MC_%dS.root", iS), "READ");

	TFile* fout = new TFile(Form("MCParameterPlot_%s_%dS_%s.root", VarName[iVar].Data(), iS, version.Data()), "RECREATE");

	FILE* ftxt;
	ftxt = fopen(Form("MCParameters_%s_%dS_%s.txt", VarName[iVar].Data(), iS, version.Data()), "w");
//}}}

	if(ftxt != NULL)
	{
		for(Int_t iarr = 0; iarr < narr-1; iarr++)
		{
//Get data{{{
			RooDataSet* dataset = (RooDataSet*) fin->Get("dataset");
			RooWorkspace* ws = new RooWorkspace(Form("workspace_%dS", iS));
			ws->import(*dataset);
			ws->data("dataset")->Print();

			RooDataSet* initialDS = (RooDataSet*) dataset->reduce(RooArgSet(*(ws->var("mass")), *(ws->var("pt")), *(ws->var("y")), *(ws->var("dphi")), *(ws->var("Centrality")), *(ws->var("weight")), *(ws->var("mupl_pt")), *(ws->var("mumi_pt"))));
			initialDS->SetName("initialDS");
			RooDataSet* weightedDS = new RooDataSet("weightedDS", "weight dataset", *initialDS->get(), Import(*initialDS), WeightVar(*ws->var("weight")));
//}}}

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

			RooDataSet* reducedDS = (RooDataSet*) weightedDS->reduce(RooArgSet(*(ws->var("mass"))), Form("(mupl_pt>4&&mumi_pt>4)&&(fabs(%s)>=%f&&fabs(%s)<%f)&&(%s>=%f&&%s<%f)&&(%s>=%f&&%s<%f)&&(fabs(%s)>=%f&&fabs(%s)<%f)", VarName[0].Data(), rapBinsArr[0], VarName[0].Data(), rapBinsArr[rap_narr-1], VarName[1].Data(), CentBinsArr[0], VarName[1].Data(), CentBinsArr[Cent_narr-1], VarName[2].Data(), ptBinsArr[0], VarName[2].Data(), ptBinsArr[pt_narr-1], VarName[iVar].Data(), BinsArr[0], VarName[iVar].Data(), BinsArr[1]));
			reducedDS->SetName("reducedDS");
			ws->import(*reducedDS);
			ws->var("mass")->setRange(8, 14);
			ws->var("mass")->Print();

//Set Canvas and Pad{{{
			TCanvas* c1 = new TCanvas(Form("c1_%s_%.1f_%.1f_%dS", VarName[iVar].Data(), BinsArr[0], BinsArr[1], iS), "", 0, 0, 600, 600);
			c1->cd();
			TPad* pad_mass = new TPad("pad_mass", "pad_mass", 0, 0.25, 0.98, 1.0);
			pad_mass->Draw();
			TPad* pad_pull = new TPad("pad_pull", "pad_pull", 0, 0.05, 0.98, 0.25);
			pad_pull->SetBottomMargin(0);
			pad_pull->Draw();
			TPad* pad_leg = new TPad("pad_leg", "pad_leg", 0.65, 0.35, 0.85, 0.92);
			pad_leg->SetBottomMargin(0);
			pad_leg->Draw();
//}}}

			pad_mass->cd();
			RooPlot* massPlot = ws->var("mass")->frame(Nmassbins);
			ws->data("reducedDS")->plotOn(massPlot, Name(Form("massPlot_%s_%.1f_%.1f_%dS", VarName[iVar].Data(), BinsArr[0], BinsArr[1], iS)));

//Fitting function{{{

//Signal function{{{
			RooRealVar mean("mean", "mean of dimuon", Up_mass, Up_mass-0.01, Up_mass+0.01);
			RooRealVar sigma1("sigma1", "sigma1 of dimuon", 0.08, 0.01, 0.3);
			RooRealVar sf("sf", "sigma fraction", 0.5, 0, 10);
			RooFormulaVar sigma2("sigma2", "@0*@1", RooArgSet(sigma1, sf));
			RooRealVar alpha("alpha", "alpha of Crystal ball", 1.0, 0.1, 10.0);
			RooRealVar n("n", "n of Crystal ball", 1.0, 0.1, 10.0);
			RooRealVar* frac = new RooRealVar("frac", "CB fraction", 0.5, 0, 1);

			RooCBShape* CB1 = new RooCBShape("CB1", "Crystal ball function1", *(ws->var("mass")), mean, sigma1, alpha, n);
			RooCBShape* CB2 = new RooCBShape("CB2", "Crystal ball function2", *(ws->var("mass")), mean, sigma2, alpha, n);
			RooAddPdf* twoCB = new RooAddPdf("twoCB", "Sum of Crystal ball", RooArgList(*CB1, *CB2), RooArgList(*frac));
//}}}

//Background function{{{
			RooRealVar Polp0("Polp0", "1st parameter of background", 0.1, -5, 5);
			RooRealVar Polp1("Polp1", "2nd parameter of background", 0.1, -5, 5);
			RooRealVar Polp2("Polp2", "3rd parameter of background", 0.1, -100, 100);
			RooPolynomial* bkgPol1 = new RooPolynomial("bkgPol1", "1st order polynomial background", *(ws->var("mass")), RooArgList(Polp0, Polp1));
			//RooPolynomial* bkgPol1 = new RooPolynomial("bkgPol1", "1st order polynomial background", *(ws->var("mass")), RooArgList(Polp0));
//}}}

//Select Pdf{{{
			RooGenericPdf* Signal = (RooGenericPdf*) twoCB;
			RooGenericPdf* Background = (RooGenericPdf*) bkgPol1;
//}}}

//}}}

//Draw mass plot{{{
			RooRealVar* nSig = new RooRealVar("nSig", "# of signal", 400, 0, 100000000);
			RooRealVar* nBkg = new RooRealVar("nBkg", "number of background", 300, 0, 1000000000);
			RooAddPdf* model = new RooAddPdf("model", "Sig+Bkg", RooArgList(*Signal, *Background), RooArgList(*nSig, *nBkg));
			ws->import(*model);

			RooFitResult* Result = ws->pdf("model")->fitTo(*reducedDS, Save(), Hesse(kTRUE), Range(8, 14), Minos(0), SumW2Error(kTRUE), Extended(kTRUE));
			ws->pdf("model")->plotOn(massPlot, Name("modelPlot"));
			ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*Signal)), LineColor(kRed), LineStyle(kDashed), MoveToBack());
			ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*CB1)), LineColor(kRed), LineStyle(kLine), MoveToBack());
			ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*CB2)), LineColor(kGreen), LineStyle(kDashed), MoveToBack());
			ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*Background)), LineColor(kBlue), LineStyle(kDashed));
			massPlot->SetTitle(Form("twoCB"));
			massPlot->SetXTitle("M (GeV/c^2)");
			massPlot->SetYTitle("Counts");
			massPlot->Draw();
			Result->Print("v");
			WriteMessage("Fitting is Done !!!");
//}}}

//Draw pull distribution{{{
			pad_pull->cd();
			RooHist* hpull = massPlot->pullHist(Form("massPlot_%s_%.1f_%.1f_%dS", VarName[iVar].Data(), BinsArr[0], BinsArr[1], iS), "modelPlot");
			RooPlot* pullPlot = ws->var("mass")->frame(Title("Pull Distribution"));
			pullPlot->addPlotable(hpull, "P");
			pullPlot->Draw();

			Double_t chi2 = 0.;
			Double_t *ypull = hpull->GetY();
			Double_t Npullbin = 0;
			for(Int_t ibin = 0; ibin < Nmassbins; ibin++)
			{
				if(ypull[ibin] == 0) continue;
				chi2 += TMath::Power(ypull[ibin], 2);
				Npullbin++;
			}
			Int_t Nfitpar = Result->floatParsFinal().getSize();
			Int_t ndf = Npullbin - Nfitpar;

			TLatex* NormChi2tex = new TLatex();
			FormLatex(NormChi2tex, 42, 0.13);
			NormChi2tex->DrawLatex(0.15, 0.95, Form("#chi^{2}/ndf: %3.f/%d", chi2, ndf));
			WriteMessage("Pull distribution is Done !!!");
//}}}

//Draw result values{{{
			pad_leg->cd();
			RooPlot* legPlot = ws->var("mass")->frame(Name("Fit Results"), Title("Fit Results"));
			RooArgList parList = Result->floatParsFinal();
			parList.Print("v");
			ws->pdf("model")->paramOn(legPlot, Layout(0, 0.93, 0.97), Parameters(parList));
			legPlot->getAttText()->SetTextAlign(11);
			legPlot->getAttText()->SetTextSize(0.09);

			TPaveText* Ptext = (TPaveText*) legPlot->findObject(Form("%s_paramBox", ws->pdf("model")->GetName()));
			Ptext->SetY1(0.01);
			Ptext->SetY2(0.95);
			Ptext->Draw();
			WriteMessage("Writing result is Done !!!");
//}}}

			Double_t Sigma1 = ws->var("sigma1")->getVal();
			Double_t Sf = ws->var("sf")->getVal();
			Double_t Alpha = ws->var("alpha")->getVal();
			Double_t N = ws->var("n")->getVal();
			Double_t Frac = ws->var("frac")->getVal();

			fprintf(ftxt, "%d %d %d %.3f %.3f %.3f %.3f %.1f %.1f %.1f %.1f %.1f %.1f %.2f %.2f %.2f \n", iVar, narr, iarr, Sigma1, 0.8*Sigma1, 1.2*Sigma1, Sf, Alpha, 0.8*Alpha, 1.2*Alpha, N, 0.8*N, 1.2*N, Frac, 0.8*Frac, 1.2*Frac);

			c1->SaveAs(Form("MCDistribution_%s_%dbin_%dth_%dS_%s.pdf", VarName[iVar].Data(), narr-1, iarr, iS, version.Data()));

			fout->cd();
			c1->Write();
		}
	}
	fclose(ftxt);
}
