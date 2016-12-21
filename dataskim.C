//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TBranch.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TString.h>
#include <iostream>
#include <fstream>
#include <TH1.h>
#include <TF1.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooArgSet.h>
#include "Style_Kisoo.h"
#include "Upsilon.h"

using namespace std;
using namespace RooFit;
//}}}

bool InAcc(Double_t muPt, Double_t muEta);

void dataskim(const Int_t PD = 1)
{
	Bool_t isMC = false;
	if(PD == 2 || PD == 3 || PD == 4) isMC = true;

	TChain* tin = new TChain("hionia/myTree");
	TString fname1, fname2, fname3, fname4, fname5, fname6; 
	FILE* ftxt;
	ftxt = fopen("Dphi_squeeze_check.txt", "w");

//Weighting factors{{{
	TH1D* hWeight;
	if(PD == 4) hWeight = new TH1D("hWeight", "", nMC3Sfile, Wbin3S);
	else hWeight = new TH1D("hWeight", "", nMCfile, Wbin);

	TF1* funcWeight[2];
	funcWeight[0] = new TF1("Weighting_mid", "([0]+[1]*x+[2]*x*x+[3]*x*x*x)*(1-TMath::Erf((x-7.5)/2))+[4]+[5]*x", 0, 30);
	funcWeight[1] = (TF1*) funcWeight[0]->Clone("Weighting_fwd");
	funcWeight[0]->SetParameters(0.182832, -0.186675, 0.047973, -0.00392975, 1.20421, -0.024113);
	funcWeight[1]->SetParameters(-0.213717, 0.0908163, -0.0316685, 0.0032112, 1.42968, -0.0404998);

//Get files{{{
	if(PD == 1)
	{
//Data{{{
		fname1 = "OniaTree_DoubleMu0ABCD_EvtPlane.root";
		tin->Add(fname1.Data());
//}}}
	}
	else if(PD == 2)
	{
//Upsilon 1S{{{
		fname1 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups1SMM_ptUps_00_03_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname2 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups1SMM_ptUps_03_06_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname3 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups1SMM_ptUps_06_09_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname4 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups1SMM_ptUps_09_12_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname5 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups1SMM_ptUps_12_15_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname6 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups1SMM_ptUps_15_30_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		tin->Add(fname1.Data());
		tin->Add(fname2.Data());
		tin->Add(fname3.Data());
		tin->Add(fname4.Data());
		tin->Add(fname5.Data());
		tin->Add(fname6.Data());
		hWeight->SetBinContent(1, 3.10497);
		hWeight->SetBinContent(2, 4.11498);
		hWeight->SetBinContent(3, 2.2579);
		hWeight->SetBinContent(4, 1.2591);
		hWeight->SetBinContent(5, 0.567094);
		hWeight->SetBinContent(6, 0.783399);
//}}}
	}
	else if(PD == 3)
	{
//Upsilon 2S{{{
		fname1 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups2SMM_ptUps2S_00_03_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname2 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups2SMM_ptUps2S_03_06_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname3 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups2SMM_ptUps2S_06_09_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname4 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups2SMM_ptUps2S_09_12_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname5 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups2SMM_ptUps2S_12_15_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname6 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups2SMM_ptUps2S_15_inf_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		tin->Add(fname1.Data());
		tin->Add(fname2.Data());
		tin->Add(fname3.Data());
		tin->Add(fname4.Data());
		tin->Add(fname5.Data());
		tin->Add(fname6.Data());
		hWeight->SetBinContent(1, 5.89168);
		hWeight->SetBinContent(2, 9.08207);
		hWeight->SetBinContent(3, 3.106);
		hWeight->SetBinContent(4, 1.10018);
		hWeight->SetBinContent(5, 0.534916);
		hWeight->SetBinContent(6, 0.776183);
//}}}
	}
	else if(PD == 4)
	{
//Upsilon 3S{{{
		fname1 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups3SMM_ptUps3S_00_03_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname2 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups3SMM_ptUps3S_03_06_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname3 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups3SMM_ptUps3S_06_09_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		fname4 = "/home/samba/OniaTree/Onia5TeV/PbPbOfficialMC/OniaTree_Pythia8_Ups3SMM_ptUps3S_09_inf_Hydjet_MB_HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v13-v1.root";
		tin->Add(fname1.Data());
		tin->Add(fname2.Data());
		tin->Add(fname3.Data());
		tin->Add(fname4.Data());
		hWeight->SetBinContent(1, 6.86815);
		hWeight->SetBinContent(2, 8.29618);
		hWeight->SetBinContent(3, 6.75153);
		hWeight->SetBinContent(4, 5.48684);
//}}}
	}
//}}}

//}}}

//Output files{{{
	TFile* fout;
	if(PD == 1)
	{
		fout = new TFile(Form("SkimmedFiles/Skim_OniaTree_PbPb_DoubleMu0ABCD_EvtPlane.root"), "RECREATE");
	}
	else if(PD == 2)
	{
		fout = new TFile(Form("SkimmedFiles/Skim_OniaTree_PbPb_MC_1S.root"), "RECREATE");
	}
	else if(PD == 3)
	{
		fout = new TFile(Form("SkimmedFiles/Skim_OniaTree_PbPb_MC_2S.root"), "RECREATE");
	}
	else if(PD == 4)
	{
		fout = new TFile(Form("SkimmedFiles/Skim_OniaTree_PbPb_MC_3S.root"), "RECREATE");
	}
//}}}

//tree variables{{{
	UInt_t eventNb;
	Int_t Centrality;
	ULong64_t HLTriggers;
	Float_t zVtx;
	Float_t rpAng[29];
	Float_t rpAngf[29];
	Int_t Reco_QQ_size;
	Int_t Reco_QQ_type[200];
	Int_t Reco_QQ_sign[200];
	Float_t Reco_QQ_VtxProb[200];
	ULong64_t Reco_QQ_trig[200];
	Bool_t Reco_QQ_mupl_highPurity[200];
	Bool_t Reco_QQ_mumi_highPurity[200];
	Float_t Reco_QQ_mupl_dxy[200];
	Float_t Reco_QQ_mumi_dxy[200];
	Float_t Reco_QQ_mupl_dz[200];
	Float_t Reco_QQ_mumi_dz[200];
	Int_t Reco_QQ_mupl_nTrkWMea[200];
	Int_t Reco_QQ_mumi_nTrkWMea[200];
	Bool_t Reco_QQ_mupl_TMOneStaTight[200];
	Bool_t Reco_QQ_mumi_TMOneStaTight[200];
	Int_t Reco_QQ_mupl_nPixWMea[200];
	Int_t Reco_QQ_mumi_nPixWMea[200];
	TClonesArray* Reco_QQ_4mom;
	TClonesArray* Reco_QQ_mupl_4mom;
	TClonesArray* Reco_QQ_mumi_4mom;
	Reco_QQ_4mom = 0;
	Reco_QQ_mupl_4mom = 0;
	Reco_QQ_mumi_4mom = 0;
//}}}

//Branches{{{
	TBranch* b_eventNb;
	TBranch* b_Centrality;
	TBranch* b_HLTriggers;
	TBranch* b_zVtx;
	TBranch* b_rpAng;
	TBranch* b_rpAngf;
	TBranch* b_Reco_QQ_size;
	TBranch* b_Reco_QQ_type;
	TBranch* b_Reco_QQ_sign;
	TBranch* b_Reco_QQ_VtxProb;
	TBranch* b_Reco_QQ_trig;
	TBranch* b_Reco_QQ_mupl_highPurity;
	TBranch* b_Reco_QQ_mumi_highPurity;
	TBranch* b_Reco_QQ_mupl_dxy;
	TBranch* b_Reco_QQ_mumi_dxy;
	TBranch* b_Reco_QQ_mupl_dz;
	TBranch* b_Reco_QQ_mumi_dz;
	TBranch* b_Reco_QQ_mupl_nTrkWMea;
	TBranch* b_Reco_QQ_mumi_nTrkWMea;
	TBranch* b_Reco_QQ_mupl_TMOneStaTight;
	TBranch* b_Reco_QQ_mumi_TMOneStaTight;
	TBranch* b_Reco_QQ_mupl_nPixWMea;
	TBranch* b_Reco_QQ_mumi_nPixWMea;
	TBranch* b_Reco_QQ_4mom;
	TBranch* b_Reco_QQ_mupl_4mom;
	TBranch* b_Reco_QQ_mumi_4mom;
//}}}

//Branch address{{{
	tin->SetBranchAddress("eventNb", &eventNb, &b_eventNb);
	tin->SetBranchAddress("Centrality", &Centrality, &b_Centrality);
	tin->SetBranchAddress("HLTriggers", &HLTriggers, &b_HLTriggers);
	tin->SetBranchAddress("zVtx", &zVtx, &b_zVtx);
	tin->SetBranchAddress("rpAng", &rpAng, &b_rpAng);
	if(!isMC) tin->SetBranchAddress("rpAngf", &rpAngf, &b_rpAngf);
	tin->SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);
	tin->SetBranchAddress("Reco_QQ_type", Reco_QQ_type, &b_Reco_QQ_type);
	tin->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign, &b_Reco_QQ_sign);
	tin->SetBranchAddress("Reco_QQ_VtxProb", Reco_QQ_VtxProb, &b_Reco_QQ_VtxProb);
	tin->SetBranchAddress("Reco_QQ_trig", Reco_QQ_trig, &b_Reco_QQ_trig);
	tin->SetBranchAddress("Reco_QQ_mupl_highPurity", Reco_QQ_mupl_highPurity, &b_Reco_QQ_mupl_highPurity);
	tin->SetBranchAddress("Reco_QQ_mumi_highPurity", Reco_QQ_mumi_highPurity, &b_Reco_QQ_mumi_highPurity);
	tin->SetBranchAddress("Reco_QQ_mupl_dxy", Reco_QQ_mupl_dxy, &b_Reco_QQ_mupl_dxy);
	tin->SetBranchAddress("Reco_QQ_mumi_dxy", Reco_QQ_mumi_dxy, &b_Reco_QQ_mumi_dxy);
	tin->SetBranchAddress("Reco_QQ_mupl_dz", Reco_QQ_mupl_dz, &b_Reco_QQ_mupl_dz);
	tin->SetBranchAddress("Reco_QQ_mumi_dz", Reco_QQ_mumi_dz, &b_Reco_QQ_mumi_dz);
	tin->SetBranchAddress("Reco_QQ_mupl_nTrkWMea", Reco_QQ_mupl_nTrkWMea, &b_Reco_QQ_mupl_nTrkWMea);
	tin->SetBranchAddress("Reco_QQ_mumi_nTrkWMea", Reco_QQ_mumi_nTrkWMea, &b_Reco_QQ_mumi_nTrkWMea);
	tin->SetBranchAddress("Reco_QQ_mupl_TMOneStaTight", Reco_QQ_mupl_TMOneStaTight, &b_Reco_QQ_mupl_TMOneStaTight);
	tin->SetBranchAddress("Reco_QQ_mumi_TMOneStaTight", Reco_QQ_mumi_TMOneStaTight, &b_Reco_QQ_mumi_TMOneStaTight);
	tin->SetBranchAddress("Reco_QQ_mupl_nPixWMea", Reco_QQ_mupl_nPixWMea, &b_Reco_QQ_mupl_nPixWMea);
	tin->SetBranchAddress("Reco_QQ_mumi_nPixWMea", Reco_QQ_mumi_nPixWMea, &b_Reco_QQ_mumi_nPixWMea);
	tin->SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
	tin->SetBranchAddress("Reco_QQ_mupl_4mom", &Reco_QQ_mupl_4mom, &b_Reco_QQ_mupl_4mom);
	tin->SetBranchAddress("Reco_QQ_mumi_4mom", &Reco_QQ_mumi_4mom, &b_Reco_QQ_mumi_4mom);
//}}}

//RooFit variable{{{
	RooRealVar* RooCentrality = new RooRealVar("Centrality", "Centrality bin", 0, 200, "");
	RooRealVar* Roomass = new RooRealVar("mass", "dimuon mass", 0, 100, "GeV/c^{2}");
	RooRealVar* Roopt = new RooRealVar("pt", "p_{T} of dimuon", 0, 100, "GeV/c");
	RooRealVar* Rooy = new RooRealVar("y", "Rapidity of dimuon", -3, 3, "");
	RooRealVar* Roophi = new RooRealVar("phi", "phi of dimuon", -4, 4, "");
	RooRealVar* Roodphi = new RooRealVar("dphi", "dphi of dimuon", -4, 4, "");
	RooRealVar* RooWeight = new RooRealVar("weight", "p_{T} weight", 0, 10000, "");
	RooRealVar* Roomupl_pt = new RooRealVar("mupl_pt", "p_{T} of + charged muon", 0, 10000, "");
	RooRealVar* Roomumi_pt = new RooRealVar("mumi_pt", "p_{T} of - charged muon", 0, 10000, "");
	RooArgSet* argset = new RooArgSet(*RooCentrality, *Roomass, *Roopt, *Rooy, *Roophi, *Roodphi, *RooWeight, *Roomupl_pt, *Roomumi_pt);
	RooDataSet* dataset = new RooDataSet("dataset", "dataset", *argset);
//}}}

//Gen variable{{{
	Int_t Gen_QQ_size;
	Int_t Gen_QQ_type[200];
	TClonesArray* Gen_QQ_4mom;
	TClonesArray* Gen_QQ_mupl_4mom;
	TClonesArray* Gen_QQ_mumi_4mom;
	Gen_QQ_4mom = 0;
	Gen_QQ_mupl_4mom = 0;
	Gen_QQ_mumi_4mom = 0;
	TBranch* b_Gen_QQ_size;
	TBranch* b_Gen_QQ_type;
	TBranch* b_Gen_QQ_4mom;
	TBranch* b_Gen_QQ_mupl_4mom;
	TBranch* b_Gen_QQ_mumi_4mom;
	if(isMC)
	{
		tin->SetBranchAddress("Gen_QQ_size", &Gen_QQ_size, &b_Gen_QQ_size);
		tin->SetBranchAddress("Gen_QQ_type", Gen_QQ_type, &b_Gen_QQ_type);
		tin->SetBranchAddress("Gen_QQ_4mom", &Gen_QQ_4mom, &b_Gen_QQ_4mom);
		tin->SetBranchAddress("Gen_QQ_mupl_4mom", &Gen_QQ_mupl_4mom, &b_Gen_QQ_mupl_4mom);
		tin->SetBranchAddress("Gen_QQ_mumi_4mom", &Gen_QQ_mumi_4mom, &b_Gen_QQ_mumi_4mom);
	}
//}}}

	TLorentzVector* Up_Reco_4mom = new TLorentzVector;
	TLorentzVector* mupl_Reco_4mom = new TLorentzVector;
	TLorentzVector* mumi_Reco_4mom = new TLorentzVector;
	Up_Reco_4mom = 0;
	mupl_Reco_4mom = 0;
	mumi_Reco_4mom = 0;

	TLorentzVector* Up_Gen_4mom = new TLorentzVector;
	TLorentzVector* mupl_Gen_4mom = new TLorentzVector;
	TLorentzVector* mumi_Gen_4mom = new TLorentzVector;
	Up_Gen_4mom = 0;
	mupl_Gen_4mom = 0;
	mumi_Gen_4mom = 0;

	DiMuon DMset;
	DiMuon DMGenset;

	TTree* tout = new TTree("UpsilonTree", "");
	tout->SetMaxTreeSize(10000000000);
	DMset.BuildBranch(tout);
	TTree* toutGen;
	if(isMC)
	{
		toutGen = new TTree("UpsilonGenTree", "");
		toutGen->SetMaxTreeSize(10000000000);
		DMset.BuildBranch(toutGen);
	}

	const Double_t Nevt = tin->GetEntries();
	for(Int_t ievt = 0; ievt < Nevt; ievt++)
	{
		if(ievt%100000 == 0) cout << "Events: " << ievt << " / " << Nevt << " [" << Form("%.1f", ((double)ievt/(double)Nevt)*100) << " %]" << endl;
		tin->GetEntry(ievt);

//Fill Gen{{{
		DMGenset.clear();
		if(isMC)
		{
			DMGenset.zVtx = zVtx;
			DMGenset.Centrality = Centrality;
			for(Int_t iqq = 0; iqq < Gen_QQ_size; iqq++)
			{
				Up_Gen_4mom = (TLorentzVector*) Gen_QQ_4mom->At(iqq);
				mupl_Gen_4mom = (TLorentzVector*) Gen_QQ_mupl_4mom->At(iqq);
				mumi_Gen_4mom = (TLorentzVector*) Gen_QQ_mumi_4mom->At(iqq);
				DMGenset.mass = Up_Gen_4mom->M();
				DMGenset.pt = Up_Gen_4mom->Pt();
				DMGenset.y = Up_Gen_4mom->Rapidity();
				DMGenset.phi = Up_Gen_4mom->Phi();
				DMGenset.mupl_pt = mupl_Gen_4mom->Pt();
				DMGenset.mupl_eta = mupl_Gen_4mom->Eta();
				DMGenset.mupl_phi = mupl_Gen_4mom->Phi();
				DMGenset.mumi_pt = mumi_Gen_4mom->Pt();
				DMGenset.mumi_eta = mumi_Gen_4mom->Eta();
				DMGenset.mumi_phi = mumi_Gen_4mom->Phi();
				DMGenset.weight = (float) hWeight->GetBinContent(hWeight->FindBin(DMGenset.pt));
//				if(fabs(DMGenset.y) < 1.2) DMGenset.weight = DMGenset.weight * funcWeight[0]->Eval(DMGenset.pt);
//				else DMGenset.weight = DMGenset.weight * funcWeight[1]->Eval(DMGenset.pt);
				toutGen->Fill();
			}
		}
//}}}

		DMset.clear();
		DMset.eventNb = eventNb;
		DMset.zVtx = zVtx;
		DMset.Centrality = Centrality;
		Double_t dphi = -999;

		if( (HLTriggers&1)!=1 ) continue;

		for(Int_t iqq = 0; iqq < Reco_QQ_size; iqq++)
		{
			Up_Reco_4mom = (TLorentzVector*) Reco_QQ_4mom->At(iqq);
			mupl_Reco_4mom = (TLorentzVector*) Reco_QQ_mupl_4mom->At(iqq);
			mumi_Reco_4mom = (TLorentzVector*) Reco_QQ_mumi_4mom->At(iqq);

//Cuts{{{
			if( Reco_QQ_sign[iqq] != 0 ) continue;
			if( !InAcc(mupl_Reco_4mom->Pt(), mupl_Reco_4mom->Eta()) ) continue;
			if( !InAcc(mumi_Reco_4mom->Pt(), mumi_Reco_4mom->Eta()) ) continue;
			if( Up_Reco_4mom->M() < 8 || Up_Reco_4mom->M() > 14) continue;
			if( (Reco_QQ_trig[iqq]&1)!=1 ) continue;
			bool muplSoft = ( (Reco_QQ_mupl_TMOneStaTight[iqq]==true) &&
									(Reco_QQ_mupl_nTrkWMea[iqq] > 5) &&
									(Reco_QQ_mupl_nPixWMea[iqq] > 0) &&
									(Reco_QQ_mupl_dxy[iqq]<0.3) &&
									(Reco_QQ_mupl_dz[iqq]<20.) );
			bool mumiSoft = ( (Reco_QQ_mumi_TMOneStaTight[iqq]==true) &&
									(Reco_QQ_mumi_nTrkWMea[iqq] > 5) &&
									(Reco_QQ_mumi_nPixWMea[iqq] > 0) &&
									(Reco_QQ_mumi_dxy[iqq]<0.3) &&
									(Reco_QQ_mumi_dz[iqq]<20.) );
			if( !(muplSoft && mumiSoft) ) continue;
			if( Reco_QQ_VtxProb[iqq] < 0.01 ) continue;
//}}}

//Get dphi{{{
			if(Up_Reco_4mom->Rapidity() > 0)
			{
				if(isMC)
				{
					if(rpAng[6] > -9)
					{
						dphi = Up_Reco_4mom->Phi()-rpAng[6];
						while(dphi > TMath::Pi()/2) dphi -= TMath::Pi();
						while(dphi <= -TMath::Pi()/2) dphi += TMath::Pi();
						dphi = TMath::Abs(dphi);
					}
				}
				else
				{
					if(rpAngf[6] > -9)
					{
						dphi = Up_Reco_4mom->Phi()-rpAngf[6];
						fprintf(ftxt, "+y: dphi = %.2f, ", dphi);
						while(dphi > TMath::Pi()/2) dphi -= TMath::Pi();
						while(dphi <= -TMath::Pi()/2) dphi += TMath::Pi();
						dphi = TMath::Abs(dphi);
						fprintf(ftxt, "squzeed dphi = %.2f \n", dphi);
					}
				}
			}
			if(Up_Reco_4mom->Rapidity() < 0)
			{
				if(isMC)
				{
					if(rpAng[7] > -9)
					{
						dphi = Up_Reco_4mom->Phi()-rpAng[7];
						while(dphi > TMath::Pi()/2) dphi -= TMath::Pi();
						while(dphi <= -TMath::Pi()/2) dphi += TMath::Pi();
						dphi = TMath::Abs(dphi);
					}
				}
				else
				{
					if(rpAngf[7] > -9)
					{
						dphi = Up_Reco_4mom->Phi()-rpAngf[7];
						fprintf(ftxt, "-y: dphi = %.2f, ", dphi);
						while(dphi > TMath::Pi()/2) dphi -= TMath::Pi();
						while(dphi <= -TMath::Pi()/2) dphi += TMath::Pi();
						dphi = TMath::Abs(dphi);
						fprintf(ftxt, "squzeed dphi = %.2f \n", dphi);
					}
				}
			}
//}}}

//Fill Tree{{{
			DMset.mass = Up_Reco_4mom->M();
			DMset.pt = Up_Reco_4mom->Pt();
			DMset.y = Up_Reco_4mom->Rapidity();
			DMset.phi = Up_Reco_4mom->Phi();
			DMset.mupl_pt = mupl_Reco_4mom->Pt();
			DMset.mupl_eta = mupl_Reco_4mom->Eta();
			DMset.mupl_phi = mupl_Reco_4mom->Phi();
			DMset.mumi_pt = mupl_Reco_4mom->Pt();
			DMset.mumi_eta = mupl_Reco_4mom->Eta();
			DMset.mumi_phi = mupl_Reco_4mom->Phi();
			DMset.dphi = dphi;
			if(isMC) DMset.weight = DMGenset.weight;
			else DMset.weight = 1.;
			tout->Fill();
//}}}

//Fill RooDataSet{{{
			RooCentrality->setVal((double)DMset.Centrality);
			Roomass->setVal((double)DMset.mass);
			Roopt->setVal((double)DMset.pt);
			Rooy->setVal((double)DMset.y);
			Roophi->setVal((double)DMset.phi);
			Roodphi->setVal((double)DMset.dphi);
			RooWeight->setVal((double)DMset.weight);
			Roomupl_pt->setVal((double)DMset.mupl_pt);
			Roomumi_pt->setVal((double)DMset.mumi_pt);
			dataset->add(*argset);
//}}}
		}
	}
	dataset->Write();
	tout->Write();
	if(isMC) toutGen->Write();
	fout->Close();
}

//External function{{{
//Old Acceptance{{{
bool InAcc(Double_t muPt, Double_t muEta)
{
	return ( ((TMath::Abs(muEta) <= 1.0) && (muPt >=3.4)) ||
				((TMath::Abs(muEta) > 1.0) && (TMath::Abs(muEta) <= 1.5) && (muPt >= 5.8-2.4*(TMath::Abs(muEta)))) ||
				((TMath::Abs(muEta) > 1.5) && (TMath::Abs(muEta) <= 2.4) && (muPt >= 3.3667-7.0/9.0*(TMath::Abs(muEta)))) );
}
//}}}
/*
//New Acceptance{{{
bool InAcc(Double_t muPt, Double_t muEta)
{
	return ( TMath::Abs(muEta) < 2.4 && muPt >= 4);
}
//}}}
*/
//}}}
