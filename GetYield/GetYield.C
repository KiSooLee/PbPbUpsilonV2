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

void GetYield(const Int_t iVar = 1, const Int_t narr = 3, Int_t ndphi = 3, TString version = "v1")
{
	const Int_t Nmassbins = 120;

	TFile* fout = new TFile(Form("Yield_%s_%dbin_%dndphi_%s.root", VarName[iVar].Data(), narr-1, ndphi, version.Data()), "RECREATE");

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

//Define parameters{{{
		Int_t ivar_par, narr_par, iarr_par;
		Double_t sig11, sig12, sig21, sig22, sig31, sig32;
		Double_t Frac, alp, N;
		Double_t erfm, erfsig, erfp0;
//}}}

//Get parameter{{{
		ifstream in;
		in.open(Form("Parameters_%s_%s.txt", VarName[iVar].Data(), version.Data()));
		if(in.is_open())
		{
			while(!in.eof())
			{

				if(!in.good())
				{
					cout << "Parameter File is wrong!!! Please Check!!!" << endl;
					return;
				}
				else
				{

					in >> ivar_par >> narr_par >> iarr_par >> sig11 >> sig12 >> sig21 >> sig22 >> sig31 >> sig32 >> Frac >> alp >> N >> erfm >> erfsig >> erfp0;

					if(ivar_par == iVar && narr_par == narr && iarr_par == iarr)

					{
						cout << " " << endl;
						cout << "********** matched parameter found!!! **********" << endl;
						cout << " " << endl;
						break;
					}
				}
			}
		}
		in.close();
//}}}

		for(Int_t idphi = 0; idphi < ndphi; idphi++)
		{
//Get data{{{
			TFile* fin = new TFile(Form("../SkimmedFiles/Skim_OniaTree_PbPb_DoubleMu0ABCD_EvtPlane.root"), "READ");
			RooDataSet* dataset = (RooDataSet*) fin->Get("dataset");
			RooWorkspace* ws = new RooWorkspace(Form("workspace"));
			ws->import(*dataset);
			ws->data("dataset")->Print();

			RooDataSet* initialDS = (RooDataSet*) dataset->reduce(RooArgSet(*(ws->var("mass")), *(ws->var("pt")), *(ws->var("y")), *(ws->var("dphi")), *(ws->var("Centrality")), *(ws->var("weight")), *(ws->var("mupl_pt")), *(ws->var("mumi_pt"))));
			initialDS->SetName("initialDS");
			RooDataSet* weightedDS = new RooDataSet("weightedDS", "weight dataset", *initialDS->get(), Import(*initialDS), WeightVar(*ws->var("weight")));
//}}}

//Reduce dataset{{{
			RooDataSet* reducedDS = (RooDataSet*) weightedDS->reduce(RooArgSet(*(ws->var("mass"))), Form("(mupl_pt>4&&mumi_pt>4)&&(fabs(%s)>=%f&&fabs(%s)<%f)&&(%s>=%f&&%s<%f)&&(%s>=%f&&%s<%f)&&(fabs(%s)>=%f&&fabs(%s)<%f)&&(%s>=%f&&%s<%f)", VarName[0].Data(), rapBinsArr[0], VarName[0].Data(), rapBinsArr[rap_narr-1], VarName[1].Data(), CentBinsArr[0], VarName[1].Data(), CentBinsArr[Cent_narr-1], VarName[2].Data(), ptBinsArr[0], VarName[2].Data(), ptBinsArr[pt_narr-1], VarName[iVar].Data(), BinsArr[0], VarName[iVar].Data(), BinsArr[1], "dphi", TMath::Pi()*(double)idphi/(2*(double)ndphi), "dphi", TMath::Pi()*((double)idphi+1)/(2*(double)ndphi)));
			reducedDS->SetName("reducedDS");
			ws->import(*reducedDS);
			ws->var("mass")->setRange(8, 14);
			ws->var("mass")->Print();
//}}}

//Set Canvas and Pad{{{
			TCanvas* c1 = new TCanvas("c1", "", 0, 0, 600, 600);
			c1->cd();
			TPad* pad_mass = new TPad("pad_mass", "pad_mass", 0, 0.25, 0.98, 1.0);
			pad_mass->Draw();
			TPad* pad_pull = new TPad("pad_pull", "pad_pull", 0, 0.05, 0.98, 0.25);
			pad_pull->SetBottomMargin(0);
			pad_pull->Draw();
			TPad* pad_leg = new TPad("pad_leg", "pad_leg", 0.65, 0.35, 0.85, 0.92);
			pad_leg->SetBottomMargin(0);
			pad_leg->Draw();

			TH1D* hYield = new TH1D(Form("hYield_%s_%.1f_%.1f_%d", VarName[iVar].Data(), BinsArr[0], BinsArr[1], idphi), "", 3, 0, 3);
//}}}

			pad_mass->cd();
			RooPlot* massPlot = ws->var("mass")->frame(Nmassbins);
			ws->data("reducedDS")->plotOn(massPlot, Name("massPlot"));

//Signal function{{{

//mean{{{
			RooRealVar mean1S("mean1S", "mean of Upsilon 1S", U1S_mass, U1S_mass-0.01, U1S_mass+0.01);
			RooRealVar mratio2("mratio2", "mratio2", U2S_mass/U1S_mass);
			RooRealVar mratio3("mratio3", "mratio3", U3S_mass/U1S_mass);
			RooFormulaVar mean2S("mean2S", "mean1S*mratio2", RooArgSet(mean1S, mratio2));
			RooFormulaVar mean3S("mean3S", "mean1S*mratio3", RooArgSet(mean1S, mratio3));
//}}}

//sigma{{{
			RooRealVar sigma1S_1("sigma1S_1", "sigma1 of 1S", 0.05, 0.03, 0.18);
			RooRealVar sigma2S_1("sigma2S_1", "sigma1 of 2S", 0.05, 0.03, 0.18);
			RooRealVar sigma3S_1("sigma3S_1", "sigma1 of 3S", 0.05, 0.02, 0.15);
			RooRealVar sigma1S_2("sigma1S_2", "sigma2 of 1S", 0.05, 0.03, 0.18);
			RooRealVar sigma2S_2("sigma2S_2", "sigma2 of 2S", 0.05, 0.03, 0.18);
			RooRealVar sigma3S_2("sigma3S_2", "sigma2 of 3S", 0.05, 0.02, 0.15);
//}}}

			RooRealVar alpha("alpha", "alpha of Crystal ball", 2., 0.7, 15.0);
			RooRealVar n("n", "n of Crystal ball", 2.0, 0.7, 15.0);
			RooRealVar* frac = new RooRealVar("frac", "CB fraction", 0.5, 0, 1);

//twoCB function{{{
			RooCBShape* CB1S_1 = new RooCBShape("CB1S_1", "1S Crystal ball function1", *(ws->var("mass")), mean1S, sigma1S_1, alpha, n);
			RooCBShape* CB2S_1 = new RooCBShape("CB2S_1", "2S Crystal ball function1", *(ws->var("mass")), mean2S, sigma2S_1, alpha, n);
			RooCBShape* CB3S_1 = new RooCBShape("CB3S_1", "3S Crystal ball function1", *(ws->var("mass")), mean3S, sigma3S_1, alpha, n);
			RooCBShape* CB1S_2 = new RooCBShape("CB1S_2", "1S Crystal ball function2", *(ws->var("mass")), mean1S, sigma1S_2, alpha, n);
			RooCBShape* CB2S_2 = new RooCBShape("CB2S_2", "2S Crystal ball function2", *(ws->var("mass")), mean2S, sigma2S_2, alpha, n);
			RooCBShape* CB3S_2 = new RooCBShape("CB3S_2", "3S Crystal ball function2", *(ws->var("mass")), mean3S, sigma3S_2, alpha, n);

			RooAddPdf* twoCB1S = new RooAddPdf("twoCB1S", "Sum of 1S Crystal ball", RooArgList(*CB1S_1, *CB1S_2), RooArgList(*frac));
			RooAddPdf* twoCB2S = new RooAddPdf("twoCB2S", "Sum of 2S Crystal ball", RooArgList(*CB2S_1, *CB2S_2), RooArgList(*frac));
			RooAddPdf* twoCB3S = new RooAddPdf("twoCB3S", "Sum of 3S Crystal ball", RooArgList(*CB3S_1, *CB3S_2), RooArgList(*frac));
//}}}

//}}}

//Background function{{{
			RooRealVar Erfmean("Erfmean", "Mean of Errfunction", 5, 0, 9);
			RooRealVar Erfsigma("Erfsigma", "Sigma of Errfunction", 1, 0, 100);
			RooRealVar Erfp0("Erfp0", "1st parameter of Errfunction", 1, 0, 30);

			RooGenericPdf* bkgErf = new RooGenericPdf("bkgErr", "Error background", "TMath::Exp(-@0/@1)*(TMath::Erf((@0-@2)/(TMath::Sqrt(2)*@3))+1)*0.5", RooArgList(*(ws->var("mass")), Erfp0, Erfmean, Erfsigma));
//}}}

//Select Pdf{{{
			RooGenericPdf* Signal1S = (RooGenericPdf*) twoCB1S;
			RooGenericPdf* Signal2S = (RooGenericPdf*) twoCB2S;
			RooGenericPdf* Signal3S = (RooGenericPdf*) twoCB3S;

			RooGenericPdf* Background;
			Background = (RooGenericPdf*) bkgErf;
//}}}

//Set Parameters{{{
			sigma1S_1.setVal(sig11); sigma1S_1.setConstant();
			sigma1S_2.setVal(sig12); sigma1S_2.setConstant();
			sigma2S_1.setVal(sig21); sigma2S_1.setConstant();
			sigma2S_2.setVal(sig22); sigma2S_2.setConstant();
			sigma3S_1.setVal(sig31); sigma3S_1.setConstant();
			sigma3S_2.setVal(sig32); sigma3S_2.setConstant();
			alpha.setVal(alp); alpha.setConstant();
			n.setVal(N); n.setConstant();
			frac->setVal(Frac); frac->setConstant();
			Erfmean.setVal(erfm); Erfmean.setConstant(erfm);
			Erfsigma.setVal(erfsig); Erfsigma.setConstant(erfsig);
			Erfp0.setVal(erfp0); Erfp0.setConstant(erfp0);
//}}}

//Draw mass plot{{{
			RooRealVar* nSig1S = new RooRealVar("nSig1S", "# of 1S signal", 400, 0, 100000);
			RooRealVar* nSig2S = new RooRealVar("nSig2S", "# of 2S signal", 100, 0, 3000);
			RooRealVar* nSig3S = new RooRealVar("nSig3S", "# of 3S signal", 10, 0, 900);
			RooRealVar* nBkg = new RooRealVar("nBkg", "number of background", 300, 0, 1000000);
			RooAddPdf* model = new RooAddPdf("model", "1S+2S+3S+Bkg", RooArgList(*Signal1S, *Signal2S, *Signal3S, *Background), RooArgList(*nSig1S, *nSig2S, *nSig3S, *nBkg));
			ws->import(*model);

			RooFitResult* Result = ws->pdf("model")->fitTo(*reducedDS, Save(), Hesse(kTRUE), Range(8, 14), Minos(0), SumW2Error(kTRUE), Extended(kTRUE));
			ws->pdf("model")->plotOn(massPlot, Name("modelPlot"));
			ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*Signal1S)), LineColor(kRed), LineStyle(kDashed), MoveToBack());
			ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*Signal2S)), LineColor(kRed), LineStyle(kDashed), MoveToBack());
			ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*Signal3S)), LineColor(kRed), LineStyle(kDashed), MoveToBack());
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
			RooHist* hpull = massPlot->pullHist("massPlot", "modelPlot");
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

//Save Yield{{{
			Double_t Yield1S = ws->var("nSig1S")->getVal();
			Double_t Yield1SErr = ws->var("nSig1S")->getError();
			Double_t Yield2S = ws->var("nSig2S")->getVal();
			Double_t Yield2SErr = ws->var("nSig2S")->getError();
			Double_t Yield3S = ws->var("nSig3S")->getVal();
			Double_t Yield3SErr = ws->var("nSig3S")->getError();

			hYield->SetBinContent(1, Yield1S);
			hYield->SetBinError(1, Yield1SErr);
			hYield->SetBinContent(2, Yield2S);
			hYield->SetBinError(2, Yield2SErr);
			hYield->SetBinContent(3, Yield3S);
			hYield->SetBinError(3, Yield3SErr);
//}}}

			c1->SaveAs(Form("MassDistribution_%s_%dbin_%dth_%dndphi_%ddphi_%s.pdf", VarName[iVar].Data(), narr-1, iarr, ndphi, idphi, version.Data()));
			fout->cd();
			massPlot->Write();
			hYield->Write();
		}
	}
}
