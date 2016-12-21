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

void GetYield(const Int_t iVar = 1, Int_t iarr = 0, Int_t ndphi = 3, Int_t idphi = 0, TString version = "v1")
{
	const Int_t Nmassbins = 120;

//Set target variable{{{
	Double_t BinsArr[narr] = {0};
	if(iVar == 0)
	{
		for(Int_t iarr = 0; iarr < narr; iarr++)
		{
			BinsArr[iarr] = rapBinsArr[iarr];
		}
	}
	else if(iVar == 1)
	{
		for(Int_t iarr = 0; iarr < narr; iarr++)
		{
			BinsArr[iarr] = CentBinsArr[iarr];
		}
	}
	else if(iVar == 2)
	{
		for(Int_t iarr = 0; iarr < narr; iarr++)
		{
			BinsArr[iarr] = ptBinsArr[iarr];
		}
	}
	else
	{
		cout << "wrong variable number" << endl;
		return;
	}
//}}}

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

	TFile* fout = new TFile(Form("Yield_%s_%dbin_%dth_%dndphi_%dth_dphi_%s.root", VarName[iVar].Data(), narr-1, iarr, ndphi, idphi, version.Data()), "RECREATE");
	fout->cd();

	TH1D* hYield = new TH1D("hYield", "", 3, 0, 3);

	RooDataSet* reducedDS = (RooDataSet*) weightedDS->reduce(RooArgSet(*(ws->var("mass"))), Form("(mupl_pt>4&&mumi_pt>4)&&(%s>=%f&&%s<%f)&&(%s>=%f&&%s<%f)&&(%s>=%f&&%s<%f)&&(%s>=%f&&%s<%f)&&(%s>=%f&&%s<%f)", VarName[1].Data(), CentBinsArr[0], VarName[1].Data(), CentBinsArr[narr-1], VarName[0].Data(), rapBinsArr[0], VarName[0].Data(), rapBinsArr[narr-1], VarName[2].Data(), ptBinsArr[0], VarName[2].Data(), ptBinsArr[narr-1], VarName[iVar].Data(), BinsArr[iarr], VarName[iVar].Data(), BinsArr[iarr+1], "dphi", TMath::Pi()*(double)idphi/(2*(double)ndphi), "dphi", TMath::Pi()*((double)idphi+1)/(2*(double)ndphi)));
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
	TPad* pad_leg = new TPad("pad_leg", "pad_leg", 0.65, 0.35, 0.85, 0.92);
	pad_leg->SetBottomMargin(0);
	pad_leg->Draw();
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
	RooRealVar sigma1S_1("sigma1S_1", "sigma1 of 1S", 0.04, 0.03, 0.18);
	RooRealVar sigma2S_1("sigma2S_1", "sigma1 of 2S", 0.04, 0.03, 0.18);
	RooRealVar sigma3S_1("sigma3S_1", "sigma1 of 3S", 0.04, 0.02, 0.18);
	RooRealVar sigma1S_2("sigma1S_2", "sigma2 of 1S", 0.04, 0.03, 0.18);
	RooRealVar sigma2S_2("sigma2S_2", "sigma2 of 2S", 0.04, 0.03, 0.18);
	RooRealVar sigma3S_2("sigma3S_2", "sigma2 of 3S", 0.04, 0.02, 0.18);
//}}}

//alpha{{{
	RooRealVar alpha1S_1("alpha1S_1", "alpha1 of 1S Crystal ball", 2., 1.1, 20.0);
	RooRealVar alpha2S_1("alpha2S_1", "alpha1 of 2S Crystal ball", 2., 1.1, 20.0);
	RooRealVar alpha3S_1("alpha3S_1", "alpha1 of 3S Crystal ball", 2., 1.1, 20.0);
	RooRealVar alpha1S_2("alpha1S_2", "alpha2 of 1S Crystal ball", 2., 1.1, 20.0);
	RooRealVar alpha2S_2("alpha2S_2", "alpha2 of 2S Crystal ball", 2., 1.1, 20.0);
	RooRealVar alpha3S_2("alpha3S_2", "alpha2 of 3S Crystal ball", 2., 1.1, 20.0);
//}}}

//n{{{
	RooRealVar n1S_1("n1S_1", "n1 of 1S Crystal ball", 2.0, 1.1, 20.0);
	RooRealVar n2S_1("n2S_1", "n1 of 2S Crystal ball", 2.0, 1.1, 20.0);
	RooRealVar n3S_1("n3S_1", "n1 of 3S Crystal ball", 2.0, 1.1, 20.0);
	RooRealVar n1S_2("n1S_2", "n2 of 1S Crystal ball", 2.0, 1.1, 20.0);
	RooRealVar n2S_2("n2S_2", "n2 of 2S Crystal ball", 2.0, 1.1, 20.0);
	RooRealVar n3S_2("n3S_2", "n2 of 3S Crystal ball", 2.0, 1.1, 20.0);
//}}}

//fraction{{{
	RooRealVar* frac1S = new RooRealVar("frac1S", "CB fraction of 1S", 0.5, 0, 1);
	RooRealVar* frac2S = new RooRealVar("frac2S", "CB fraction of 2S", 0.5, 0, 1);
	RooRealVar* frac3S = new RooRealVar("frac3S", "CB fraction of 3S", 0.5, 0, 1);
//}}}

//twoCB function{{{
	RooCBShape* CB1S_1 = new RooCBShape("CB1S_1", "1S Crystal ball function1", *(ws->var("mass")), mean1S, sigma1S_1, alpha1S_1, n1S_1);
	RooCBShape* CB2S_1 = new RooCBShape("CB2S_1", "2S Crystal ball function1", *(ws->var("mass")), mean2S, sigma2S_1, alpha2S_1, n2S_1);
	RooCBShape* CB3S_1 = new RooCBShape("CB3S_1", "3S Crystal ball function1", *(ws->var("mass")), mean3S, sigma3S_1, alpha3S_1, n3S_1);
	RooCBShape* CB1S_2 = new RooCBShape("CB1S_2", "1S Crystal ball function2", *(ws->var("mass")), mean1S, sigma1S_2, alpha1S_2, n1S_2);
	RooCBShape* CB2S_2 = new RooCBShape("CB2S_2", "2S Crystal ball function2", *(ws->var("mass")), mean2S, sigma2S_2, alpha2S_2, n2S_2);
	RooCBShape* CB3S_2 = new RooCBShape("CB3S_2", "3S Crystal ball function2", *(ws->var("mass")), mean3S, sigma3S_2, alpha3S_2, n3S_2);

	RooAddPdf* twoCB1S = new RooAddPdf("twoCB1S", "Sum of 1S Crystal ball", RooArgList(*CB1S_1, *CB1S_2), RooArgList(*frac1S));
	RooAddPdf* twoCB2S = new RooAddPdf("twoCB2S", "Sum of 2S Crystal ball", RooArgList(*CB2S_1, *CB2S_2), RooArgList(*frac2S));
	RooAddPdf* twoCB3S = new RooAddPdf("twoCB3S", "Sum of 3S Crystal ball", RooArgList(*CB3S_1, *CB3S_2), RooArgList(*frac3S));
//}}}

//}}}

//Background function{{{
	RooRealVar Erfmean("Erfmean", "Mean of Errfunction", 6, 0, 9);
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

//Draw mass plot{{{
	RooRealVar* nSig1S = new RooRealVar("nSig1S", "# of 1S signal", 400, 0, 1000);
	RooRealVar* nSig2S = new RooRealVar("nSig2S", "# of 2S signal", 100, 0, 300);
	RooRealVar* nSig3S = new RooRealVar("nSig3S", "# of 3S signal", 10, 0, 50);
	RooRealVar* nBkg = new RooRealVar("nBkg", "number of background", 300, 0, 100000);
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

	c1->SaveAs(Form("MassDistribution_%s_%dbin_%dth_%dndphi_%ddphi_%s.pdf", VarName[iVar].Data(), narr-1, iarr, ndphi, idphi, version.Data()));
	massPlot->Write();
	hYield->Write();
}
