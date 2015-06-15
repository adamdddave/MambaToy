//c++                                                                                                                                                                                                                                       
#include <iostream>
#include <cmath>
//root   

//external
#include "ToyDataGenerator.h"

int main(int argc, char* const argv[]){
  std::cout<<"Toy Data Generator for Milano DAQ testing"<<std::endl;
  int seed =0;
  if(argc>1){
    seed = atoi(argv[1]);
    std::cout<<"using seed"<<atoi(argv[1])<<std::endl;
  }
  int nev = 100;
  if(argc >2){
    nev = atoi(argv[2]);
    std::cout<<"generating "<<atoi(argv[2])<<" events"<<std::endl;
  }
  ToyDataGenerator gen(seed,nev);
  gen.generateSignal();//sample everything first.
  for(int i=0; i<nev;++i){
    gen.generatePedestal();
    gen.generateEvent(i);
    gen.combineEvent();
    //write the file
    int adcVals[4*128];
    //I don't know what to do here.
    gen.clear_histograms();//start again
  }
  return 0;
}
