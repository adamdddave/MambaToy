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
#include <RooRandom.h>
#include "ToyDataGenerator.h"
using namespace RooFit ;
using namespace std;

ToyDataGenerator::ToyDataGenerator(int seed, int nevt, int nBeetle){
  m_nevt = nevt;
  m_seed = seed;
  m_nBeetle = nBeetle;
  rand = new TRandom3(seed);
  prob1strip = 0.88;
  prob2strip = 0.11;
  t = new RooRealVar("t","t",0.,200.);
  mg = new RooRealVar("mg","mg",0.);
  wg = new RooRealVar("wg","wg",6.81);
  lmu = new RooRealVar("lmu","lmu",70.);
  lsig= new RooRealVar("lsig","lsig",5.65);
  landau = new RooLandau("landau","landau",*t,*lmu,*lsig);
  gauss = new RooGaussian("gauss","gauss",*t,*mg,*wg);
  lxg = new RooFFTConvPdf("lxg","lxg",*t,*landau,*gauss);
  
  for(int i=0; i<4;++i){
    m_signals[i] = new TH1D(Form("beetle%i",i),Form("beetle%i",i),128,0,127);
    m_pedestals[i] = new TH1D(Form("ped%i",i),Form("ped%i",i),128,0,127);
    m_event[i] = new TH1D(Form("data%i",i),Form("data%i",i),128,0,127);
  }
}
void ToyDataGenerator::clear_histograms(){
  for(int i=0; i<4; ++i){
    for(int bin=0; bin<128;++bin){
      m_signals[i]->SetBinContent(bin,0.);
      m_pedestals[i]->SetBinContent(bin,0.);
      m_event[i]->SetBinContent(bin,0.);
    }
  }
}

void ToyDataGenerator::generatePedestal(){
  for(int i=0; i<4; ++i){//beetles
    for(int j=0;j<(25*128);++j){//baseline pedestals
      m_pedestals[i]->Fill(floor(rand->Uniform(128)));
    }
    for(int bin=0; bin<128;++bin){
      for(int j=0; j<500;++j){
	m_pedestals[i]->Fill(bin);
      }
    }
  }
}

void ToyDataGenerator::generateSignal(){
  RooRandom::randomGenerator()->SetSeed(m_seed);
  dataset = lxg->generate(RooArgSet(*t),m_nevt);
  //this should be about right. Numerical convolution with a gaussian with noise width of 6.81, from TB note
  //and the landau with the correct width, also from TB note. All for p-in-n fits.
}

void ToyDataGenerator::generateEvent(int ev){
  std::cout<<"generating event"<<std::endl;
  //random number to see if we want a 1 2 or 3 strip cluster.
  //just test one beetle for now.
  //for(int i=0; i<m_nBeetle;++i){
    double evSeed = rand->Uniform(128);//channel which the seed will be deposited in. Use the decimal for 1,2 or 3 strip clusters
    RooArgSet *set = lxg->getObservables(dataset);
    //dataset->get(ev)->find("t");
    //dataset->get(ev);
    *set = *dataset->get(ev);
    //std::cout<<"test after get, t->getVal() = "<<t->getVal()<<std::endl;
    int adcSeed = (int)(t->getVal());//number of ADC counts in the signal
    double genVal = rand->Uniform(1);//probability for cluster size
    int clusSize = 2;
    if(genVal< prob1strip){
      clusSize=0;
    }
    else if(genVal < (prob1strip+prob2strip)){
      clusSize = 1;
    }
    std::cout<<"evSeed = "<<evSeed<<" adcSeed = "<<adcSeed<<" clusSize = "<<clusSize<<std::endl;
    
    if(clusSize==0){      
      std::cout<<"filling with 1 strip cluster"<<std::endl;
      for(int j = 0; j<adcSeed;++j){
	m_signals[/*i*/0]->Fill(floor(evSeed));
      }
    }
    else if(clusSize==1){
      double seedbias = evSeed-floor(evSeed);
      std::cout<<"filling 2 strip cluster with "<<seedbias<<" %% in Channel "<<floor(evSeed)
	       <<" and "<<1-seedbias<<" %% in Channel "<<ceil(evSeed)<<std::endl;
      int nclus1 = (int) floor(adcSeed*seedbias);
      int nclus2 = (int) floor(adcSeed*(1-seedbias));
      for(int j=0; j<nclus1;++j){
	m_signals[/*i*/0]->Fill(floor(evSeed));
      }
      for(int j=0; j<nclus2;++j){
	m_signals[/*i*/0]->Fill(ceil(evSeed));
      }
    }
    else{
      std::cout<<"filling 3 strip cluster"<<endl;
      for(int j = 0; j<adcSeed;++j){
	m_signals[/*i*/0]->Fill(rand->Gaus((float)evSeed,1.5/2.));//fill as a gaussian around the seed.
      }
    }
    
    //}//loop over beetles
}//end of the generation


void ToyDataGenerator::combineEvent(){
  for(int i=0; i<4;++i){
    m_event[i]->Add(m_pedestals[i]);
    m_event[i]->Add(m_signals[i]);
  }
}
