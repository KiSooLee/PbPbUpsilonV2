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
#include "../../Style_Kisoo.h"
#include "../../Upsilon.h"
using namespace std;
using namespace RooFit;
//}}}

void MC_fit_function(const Int_t iS = 1, TString SigName = "CB", TString BkgName = "Erf", TString version = "v1")
{
	const Int_t Nmassbins = 120;

//Upsilon mass{{{
	Double_t Up_mass;
	if(iS == 1) Up_mass = U1S_mass;
	else if(iS == 2) Up_mass = U2S_mass;
	else if(iS == 3) Up_mass = U3S_mass;
	else
	{
		cout << "Out of variable range!!!" << endl;
		return;
	}
//}}}

//Get data{{{
	TFile* fin = new TFile(Form("/home/kisoo/work/Upsilon/PbPb/SkimmedFiles/Skim_OniaTree_PbPb_MC_%dS.root", iS), "READ");
	RooDataSet* dataset = (RooDataSet*) fin->Get("dataset");
	RooWorkspace* ws = new RooWorkspace(Form("workspace_%dS", iS));
	ws->import(*dataset);
	ws->data("dataset")->Print();

	RooDataSet* initialDS = (RooDataSet*) dataset->reduce(RooArgSet(*(ws->var("mass")), *(ws->var("pt")), *(ws->var("y")), *(ws->var("dphi")), *(ws->var("Centrality")), *(ws->var("weight")), *(ws->var("mupl_pt")), *(ws->var("mumi_pt"))));
	initialDS->SetName("initialDS");
	RooDataSet* weightedDS = new RooDataSet("weightedDS", "weight dataset", *initialDS->get(), Import(*initialDS), WeightVar(*ws->var("weight")));
//}}}

	TFile* fout = new TFile(Form("Fit_Function_%s_%s_%dS_%s.root", SigName.Data(), BkgName.Data(), iS, version.Data()), "RECREATE");
	fout->cd();

	RooDataSet* reducedDS = (RooDataSet*) weightedDS->reduce(RooArgSet(*(ws->var("mass"))), Form("(mupl_pt>4&&mumi_pt>4)&&(fabs(%s)>=%f&&fabs(%s)<%f)&&(%s>=%f&&%s<%f)&&(%s>=%f&&%s<%f)", VarName[0].Data(), rapBinsArr[0], VarName[0].Data(), rapBinsArr[rap_narr-1], VarName[1].Data(), CentBinsArr[0], VarName[1].Data(), CentBinsArr[Cent_narr-1], VarName[2].Data(), ptBinsArr[0], VarName[2].Data(), ptBinsArr[pt_narr-1]));
	reducedDS->SetName("reducedDS");
	ws->import(*reducedDS);
	ws->var("mass")->setRange(8, 14);
	ws->var("mass")->Print();

//Set Canvas and Pad{{{
	TCanvas* c1 = new TCanvas("c1", "", 0, 0, 600, 600);
	c1->cd();
	TPad* pad_mass = new TPad("pad_mass", "pad_mass", 0, 0.25, 0.98, 1.0);
	pad_mass->Draw();
	TPad* pad_pull = new TPad("pad_pull", "pad_pull", 0, 0.05, 0.98, 0.25);
	pad_pull->SetBottomMargin(0);
	pad_pull->Draw();
	TPad* pad_leg = new TPad("pad_leg", "pad_leg", 0.65, 0.55, 0.85, 0.92);
	pad_leg->SetBottomMargin(0);
	pad_leg->Draw();
//}}}

	pad_mass->cd();
	RooPlot* massPlot = ws->var("mass")->frame(Nmassbins);
	ws->data("reducedDS")->plotOn(massPlot, Name("massPlot"));

//Signal function{{{

	RooRealVar mean("mean", "mean of dimuon", Up_mass, Up_mass-0.01, Up_mass+0.01);
	RooRealVar sigma1("sigma1", "sigma of signal function", 0.06, 0.001, 0.5);

//Gaussian{{{
	RooGaussian* Gaus1 = new RooGaussian("Gaus1", "Gaussian function", *(ws->var("mass")), mean, sigma1);
//}}}

//two Gaussian{{{
	RooRealVar sigmafrac("sigmafrac", "sigma fraction of 2 Crystal ball function", 0.55, 0., 2.);
	RooFormulaVar sigma2("sigma2", "@0*@1", RooArgSet(sigma1, sigmafrac));
	RooGaussian* Gaus2 = new RooGaussian("Gaus2", "Gaussian function", *(ws->var("mass")), mean, sigma2);
	RooRealVar *frac = new RooRealVar("frac", "fraction of 2 Crystal ball function", 0.5, 0., 1.);
	RooAddPdf* twoGaus = new RooAddPdf("twoGaus", "Sum of Gaussian", RooArgList(*Gaus1, *Gaus2), RooArgList(*frac));
//}}}

//Crystal ball{{{
	RooRealVar alpha("alpha", "alpha of Crystal ball", 1.18, 1.18, 2.65);
	RooRealVar n("n", "n of Crystal ball", 1.0, 1.0, 1.78);
	RooCBShape* CB1 = new RooCBShape("CB1", "Crystal ball function", *(ws->var("mass")), mean, sigma1, alpha, n);
//}}}

//two Crystal ball{{{
	RooCBShape* CB2 = new RooCBShape("CB2", "Crystal ball function", *(ws->var("mass")), mean, sigma2, alpha, n);
	RooAddPdf* twoCB = new RooAddPdf("twoCB", "Sum of Crystal ball", RooArgList(*CB1, *CB2), RooArgList(*frac));
//}}}

//Crystal ball + Gaussian{{{
	RooAddPdf* CBGaus = new RooAddPdf("CBGaus", "Sum of Crystal ball and Gaussian", RooArgList(*CB1, *Gaus1), RooArgList(*frac));
//}}}

//}}}

//Background function{{{

//Chebychev{{{
	RooRealVar Chebp0("Chebp0", "1st parameter of background", 0.1, -3, 3);
	RooRealVar Chebp1("Chebp1", "2nd parameter of background", 0.1, -3, 3);
	RooRealVar Chebp2("Chebp2", "3rd parameter of background", 0.1, -3, 3);

	RooChebychev* bkgCheb1 = new RooChebychev("bkgCheb1", "1st order Chebychev background", *(ws->var("mass")), RooArgList(Chebp0, Chebp1));
	RooChebychev* bkgCheb2 = new RooChebychev("bkgCheb2", "2nd order Chebychev background", *(ws->var("mass")), RooArgList(Chebp0, Chebp1, Chebp2));
//}}}

//Polynomial{{{
	RooRealVar Polp0("Polp0", "1st parameter of background", 0.1, -100, 100);
	RooRealVar Polp1("Polp1", "2nd parameter of background", 0.1, -100, 100);
	RooRealVar Polp2("Polp2", "3rd parameter of background", 0.1, -100, 100);
	RooPolynomial* bkgPol1 = new RooPolynomial("bkgPol1", "1st order polynomial background", *(ws->var("mass")), RooArgList(Polp0, Polp1));
//}}}

//Exponential{{{
	RooRealVar Expp0("Expp0", "1st parameter of exponential", 100, 0, 10000);
	RooRealVar Expp1("Expp1", "2nd parameter of exponential", 100, 0, 10000);

	RooExponential* bkgExp = new RooExponential("bkgExp", "Exponential background", Expp0, Expp1);
//}}}

//Error function{{{
	RooRealVar Erfmean("Erfmean", "Mean of Errfunction", 9, 0, 30);
	RooRealVar Erfsigma("Erfsigma", "Sigma of Errfunction", 1, 0, 30);
	RooRealVar Erfp0("Erfp0", "1st parameter of Errfunction", 1, 0, 30);

	RooGenericPdf* bkgErf = new RooGenericPdf("bkgErr", "Error background", "TMath::Exp(-@0/@1)*(TMath::Erf((@0-@2)/(TMath::Sqrt(2)*@3))+1)*0.5", RooArgList(*(ws->var("mass")), Erfp0, Erfmean, Erfsigma));
//}}}

//}}}

//Select Pdf{{{
	RooGenericPdf* Signal;
	if(SigName == "Gaus") Signal = (RooGenericPdf*) Gaus1;
	else if(SigName == "twoGaus") Signal = (RooGenericPdf*) twoGaus;
	else if(SigName == "CBGaus") Signal = (RooGenericPdf*) CBGaus;
	else if(SigName == "CB") Signal = (RooGenericPdf*) CB1;
	else if(SigName == "twoCB") Signal = (RooGenericPdf*) twoCB;
	else
	{
		cout << "No such signal function!!! Please check!!!" << endl;
		return;
	}

	RooGenericPdf* Background;
	if(BkgName == "Exp") Background = (RooGenericPdf*) bkgExp;
	else if(BkgName == "Pol1") Background = (RooGenericPdf*) bkgPol1;
	else if(BkgName == "Cheb1") Background = (RooGenericPdf*) bkgCheb1;
	else if(BkgName == "Cheb2") Background = (RooGenericPdf*) bkgCheb2;
	else if(BkgName == "Erf") Background = (RooGenericPdf*) bkgErf;
	else
	{
		cout << "No such background function!!! Please check!!!" << endl;
		return;
	}
//}}}

//Draw mass plot{{{
	RooRealVar* nSig = new RooRealVar("nSig", "number of signal", 20000, 0, 10000000);
	RooRealVar* nBkg = new RooRealVar("nBkg", "number of background", 300, 0, 100000);
	RooAddPdf* model = new RooAddPdf("model", "Dimuon fit", RooArgList(*Signal, *Background), RooArgList(*nSig, *nBkg));
	ws->import(*model);
	//ws->data("reducedDS")->plotOn(massPlot);

	RooFitResult* Result = ws->pdf("model")->fitTo(*reducedDS, Save(), Hesse(kTRUE), Range(8, 14), Minos(0), SumW2Error(kTRUE), Extended(kTRUE));
	ws->pdf("model")->plotOn(massPlot, Name("modelPlot"));
	ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*Signal)), LineColor(kRed), DrawOption("F"), FillColor(kRed), MoveToBack());
	ws->pdf("model")->plotOn(massPlot, Components(RooArgSet(*Background)), LineColor(kBlue), LineStyle(kDashed));
	massPlot->SetTitle(Form("%s_%s", SigName.Data(), BkgName.Data()));
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

	c1->SaveAs(Form("Fit_function_Sig%s_Bkg%s_%dS_%s.pdf", SigName.Data(), BkgName.Data(), iS, version.Data()));
}
