/*
 * FileWriter.h
 *
 *  Created on: Jun 15, 2015
 *      Author: ADendek
 */

#pragma once

#include "ToyDataGenerator.h"
#include <string>
#include <fstream>
#include <array>

class I_FileWriter
{
public:

  class InputFileError: public std::runtime_error
  {
  public:
    InputFileError(const string& msg) :
      std::runtime_error(msg)
    {
    }
  };

  virtual ~I_FileWriter(){};
  virtual void setData(const I_ToyDataGenerator::SignalMap& signalMap )=0;
  virtual void writeEventToFile()=0;
};


typedef struct _fileHeader
{
  int64_t hederLine1; // that is this?
  int64_t hederLine2;
  int64_t packId;
  int64_t trackId;
  int64_t mambaTimestamp;
  int64_t telescopeTimestamp;
  int64_t coincidenceID;
  int64_t padding;
}FileHeader ;

typedef struct _fileFooter
{
  int64_t dataFooter1;// what is this?
  int64_t dataFooter2;
  int64_t tdcData1;
  int64_t tdcData2; // what is this?

}FileFooter;


class FileWriter : public I_FileWriter
{
public:
  FileWriter (const std::string& fileName);
  ~FileWriter();
  void setData(const I_ToyDataGenerator::SignalMap& signalMap){m_signalMap=signalMap;}
  void writeEventToFile( );

private:

  void prepareHeader();
  void prepareFooter();

  void saveHeader() ;
  void saveData() ;
  void saveFooter() ;

  FileHeader m_fileHeader;
  FileFooter m_fileFooter;
  I_ToyDataGenerator::SignalMap m_signalMap;

  std::ofstream m_file;

};

