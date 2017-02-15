
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

6. The initial parameters for the fitting is obtained from two process. First, we fit MC mass distribution of each binning of variable to get signal fitting parameters by GetYield/GetMCParameters.C.
	Second, fit data of each variable bin by setting the initial parameters and range of +- 20 % from the initial parameters with recorded values by GetYield/GetMCParameters.C. This process is execuded by GetYield/GetParameters.C.
	Both macros are executed by GetYield/doGetYield.sh

7. With twoCB and Erf mass of Upsilon 1S, 2S, 3S are fitted by GetYield/doGetYield.sh for each bin.
	doGetYield.sh runs the GetYield.C macro.
	All the parameters are fixed. Parameter values are obtained from GetYield/GetParameters.C
	One should adjust these values for detail fitting of each bins.

8. dN/dphi distribution is fitted to calculate v2 value by dNdphi/dodphi.sh

9. Final v2 values for each state are collected by GetV2/doV2.sh

10. Comparison between different state is done by doComp.sh

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

v4:
rapidity: {0, 1.2, 2.4};
Centrality: {20, 30, 50, 80, 120};
pt: {0, 3, 6, 10, 30};
4dphi bin

v5:
rapidity: {0, 1.2, 2.4};
Centrality: {20, 50, 120};
pt: {0, 6, 30};
4dphi bin

v6:
rapidity: {0, 2.4}
Centrality: {20, 120}
pt: {0, 30}
4dphi bin

v7:
rapidity: {0, 2.4}
Centrality: {0, 20}
pt: {0, 30}
4dphi bin

v8:
rapidity: {0, 1.2, 2.4};
Centrality: {20, 40, 60, 120};
pt: {0, 6, 15, 30};
4dphi bin

