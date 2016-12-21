# PbPbUpsilonV2

1. v2 study uses flattened Event Plane.
	Validity of flattened Event Plane is checked by EventPlane/Flattening.C.

2. In order to use Roofit dataset is skimmed by doskim.sh.
	dphi value is also calculated and checked in this code.

3. Because MC is made based on some pt range, weighting is needed.
	The weighting factros are added as branch in the doskim.sh.
	To check the validity of weighted value SkimmedFiles/do_weight_check.sh is used.
	Both Tree and RooDataSet are compared with SkimmedFiles/MC_weight_check_Tree.C and SkimmedFiles/MC_weight_check_Roofit.C.

4. To determine fitting function skimmed MC is used.
	Fitting function study is done by SkimmedFiles/SignalFunction/doMCfit.sh
	We choose twoCB(Sum of two Crystal Ball function).

5. In the real event there are background. So background study is done with data sample.
	With twoCB signal function background functions study is done by SkimmedFiles/BackgroundFunction/doBkgfit.sh
	We choose Error function.

6. With twoCB and Erf mass of Upsilon 1S, 2S, 3S are fitted by GetYield/doGetYield.sh for each bin.

7. dN/dphi distribution is fitted to calculate v2 value by dNdphi/dodphi.sh

8. Final v2 values for each state are collected by GetV2/doV2.sh

9. Comparison between different state is done by doComp.sh

v1:
rapidity: {-2.4, 0.0, 2.4}
Centrality: {20, 40, 60}
pt: {0, 10, 30}
3dphi bin

v2:
rapidity: {-2.4, 2.4}
Centrality: {20, 60}
pt: {0, 30}
3dphi bin

v3:
rapidity: {-2.4, 2.4}
Centrality: {40, 120}
pt: {0, 30}
3dphi bin
