//Headers{{{
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <TLorentzVector.h>
#include <TClonesArray.h>
//}}}
void rpAngCheck()
{
	TFile* fin = new TFile("/home/kisoo/work/Upsilon/PbPb/OniaTree_DoubleMu0ABCD_EvtPlane.root", "READ");
	TTree* tin = (TTree*) fin->Get("hionia/myTree");

//tree variables{{{
	UInt_t eventNb;
	Float_t rpAngf[29];
	Int_t Reco_QQ_size;
	Int_t Reco_QQ_type[20];
	Int_t Reco_QQ_sign[20];
	TClonesArray* Reco_QQ_4mom;
	Reco_QQ_4mom = 0;
//}}}

//Branches{{{
	TBranch* b_eventNb;
	TBranch* b_rpAngf;
	TBranch* b_Reco_QQ_size;
	TBranch* b_Reco_QQ_type;
	TBranch* b_Reco_QQ_sign;
	TBranch* b_Reco_QQ_4mom;
//}}}

//Branch address{{{
	tin->SetBranchAddress("eventNb", &eventNb, &b_eventNb);
	tin->SetBranchAddress("rpAngf", &rpAngf, &b_rpAngf);
	tin->SetBranchAddress("Reco_QQ_size", &Reco_QQ_size, &b_Reco_QQ_size);
	tin->SetBranchAddress("Reco_QQ_type", Reco_QQ_type, &b_Reco_QQ_type);
	tin->SetBranchAddress("Reco_QQ_sign", Reco_QQ_sign, &b_Reco_QQ_sign);
	tin->SetBranchAddress("Reco_QQ_4mom", &Reco_QQ_4mom, &b_Reco_QQ_4mom);
//}}}

	TLorentzVector* Up_Reco_4mom = new TLorentzVector;

	Int_t Nevt = tin->GetEntries();
	for(Int_t ievt = 0; ievt < Nevt; ievt++)
	{
		if(ievt%100000==0) cout << ">>>>> EVENT " << ievt << " / " << Nevt << endl;
		tin->GetEntry(ievt);

//Check y = 0 case{{{
		for(Int_t iqq = 0; iqq < Reco_QQ_size; iqq++)
		{
			Up_Reco_4mom = (TLorentzVector*) Reco_QQ_4mom->At(iqq);
			if(Up_Reco_4mom->Rapidity() == 0)
			{
				cout << "When Upsilon rapidity is 0, +rap: " << rpAngf[6] << ", -rap: " << rpAngf[7] << endl;
			}
		}
//}}}

		if(rpAngf[6] > -9 && rpAngf[7] > -9)
		{
			cout << "Both side +rap: " << rpAngf[6] << ", -rap: " << rpAngf[7] << endl;
			Double_t reverseAng = 0.;
			if(rpAngf[7] > 0) reverseAng = rpAngf[7]-TMath::Pi()/2;
			if(rpAngf[7] < 0) reverseAng = rpAngf[7]+TMath::Pi()/2;
			cout << "Both side +rap:: " << rpAngf[6] << ", reverse -rap: " << reverseAng << endl;
		}

//Check Only one side valid case{{{
		if(rpAngf[6] > -9 && rpAngf[7] < -8) cout << "only +rap" << endl;
		if(rpAngf[6] < -8 && rpAngf[7] > -9) cout << "only -rap" << endl;
//}}}
	}
}
