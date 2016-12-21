//Headers{{{
#include <TROOT.h>
#include <TTree.h>
#include <TString.h>
//}}}

//Basic info{{{
//Double_t rapBinsArr[] = {-2.4, 0.0, 2.4};
//Double_t CentBinsArr[] = {20, 40, 60};
//Double_t ptBinsArr[] = {0, 6, 30};
Double_t rapBinsArr[] = {-2.4, 2.4};
Double_t CentBinsArr[] = {40, 120};
Double_t ptBinsArr[] = {0, 30};
const Int_t narr = sizeof(rapBinsArr)/sizeof(double);

TString VarName[3] = {"y", "Centrality", "pt"};

const Double_t U1S_mass = 9.460;
const Double_t U2S_mass = 10.023;
const Double_t U3S_mass = 10.355;
//}}}

//MC file info{{{
const Int_t nMCfile = 6;
Double_t Wbin[nMCfile+1] = {0, 3, 6, 9, 12, 15, 9999};
const Int_t nMC3Sfile = 4;
Double_t Wbin3S[nMC3Sfile+1] = {0, 3, 6, 9, 9999};
//}}}

class DiMuon
{
//{{{
	public:

	UInt_t eventNb, LS;
	Int_t Centrality;
	Double_t zVtx, dphi;
	Double_t mass, y, pt, phi;
	Double_t mupl_eta, mupl_pt, mupl_phi;
	Double_t mumi_eta, mumi_pt, mumi_phi;
	Double_t weight;

	void clear()
	{
		eventNb = -99; LS = -99; Centrality = -99; zVtx = -99; dphi = -99;
		mass = -99; y = -99; pt = -99; phi = -99;
		mupl_eta = -99; mupl_pt = -99; mupl_phi = -99;
		mumi_eta = -99; mumi_pt = -99; mumi_phi = -99;
		weight = -99;
	}

	void BuildBranch(TTree* tout)
	{
		tout->Branch("eventNb", &eventNb);
		tout->Branch("LS", &LS);
		tout->Branch("Centrality", &Centrality);
		tout->Branch("zVtx", &zVtx);
		tout->Branch("dphi", &dphi);
		tout->Branch("mass", &mass);
		tout->Branch("y", &y);
		tout->Branch("pt", &pt);
		tout->Branch("phi", &phi);
		tout->Branch("mupl_eta", &mupl_eta);
		tout->Branch("mupl_pt", &mupl_pt);
		tout->Branch("mupl_phi", &mupl_phi);
		tout->Branch("mumi_eta", &mumi_eta);
		tout->Branch("mumi_pt", &mumi_pt);
		tout->Branch("mumi_phi", &mumi_phi);
		tout->Branch("weight", &weight);
	}
//}}}
};
