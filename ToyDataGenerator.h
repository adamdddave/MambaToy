#ifndef TOYDATAGENERATOR_H
#define TOYDATAGENERATOR_H 1
//c++
#include <iostream>
#include <fstream>
#include <cmath>
#include <assert.h>
#include <string>
//root
#include <TH1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TString.h>
#include <TRandom3.h>
//roofit
#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include <RooFitResult.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <RooPlot.h>
#include <RooHist.h>
#include <RooLandau.h>
#include <RooFFTConvPdf.h>

using namespace RooFit ;
using namespace std;
class ToyDataGenerator {
public:
  ToyDataGenerator(int seed = 0, int nevt = 100, int nBeetle = 4);
  virtual ~ToyDataGenerator(){};
  void clear_histograms();
  void generatePedestal();
  void generateEvent(int ev);
  void generateSignal();
  void combineEvent();
  TH1D* m_signals[4];
  TH1D* m_pedestals[4];
  TH1D* m_event[4];
private:
  int m_nevt;
  int m_seed;
  int m_nBeetle;
  TRandom3* rand;
  double prob1strip;
  double prob2strip;
  RooRealVar* t;
  RooRealVar* mg;
  RooRealVar* wg;
  RooRealVar* lmu;
  RooRealVar* lsig;
  RooLandau* landau;
  RooGaussian* gauss;
  RooFFTConvPdf* lxg;
  RooDataSet* dataset;
};

#endif
