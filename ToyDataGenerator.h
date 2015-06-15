#ifndef TOYDATAGENERATOR_H
#define TOYDATAGENERATOR_H 1
//c++
#include <iostream>
#include <fstream>
#include <cmath>
#include <assert.h>
#include <string>
#include <stdexcept>
#include <map>

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


class I_ToyDataGenerator
{
public:

  typedef std::vector<int > SignalVector;
  typedef std::map<int, SignalVector> SignalMap;

  virtual ~I_ToyDataGenerator(){};
  virtual void clear_histograms()=0;
  virtual void generatePedestal()=0;
  virtual void generateEvent(int ev)=0;
  virtual void generateSignal()=0;
  virtual void combineEvent()=0;

  virtual SignalMap getSignalMap()=0;

};



class ToyDataGenerator : public I_ToyDataGenerator
{
public:

  class  NotSuchBeetleChip: public std::runtime_error
  {
  public:
    NotSuchBeetleChip(const std::string &msg) :
      std::runtime_error(msg)
    {
    }
  };

  class  WrongChannelNumber: public std::runtime_error
  {
  public:
    WrongChannelNumber(const std::string &msg) :
      std::runtime_error(msg)
    {
    }
  };


  ToyDataGenerator(int seed = 0, int nevt = 100, int nBeetle = 4);
  virtual ~ToyDataGenerator(){};
  void clear_histograms();
  void generatePedestal();
  void generateEvent(int ev);
  void generateSignal();
  void combineEvent();
  I_ToyDataGenerator::SignalMap getSignalMap();

private:

   int getSignal(int beetleNmuber, int channelNumber);

  TH1D* m_signals[4];
  TH1D* m_pedestals[4];
  TH1D* m_event[4];
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
