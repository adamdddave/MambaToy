/*
 * FileWriter.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: ADendek
 */

#include "FileWriter.h"
#include <sys/time.h>
#include <boost/foreach.hpp>

FileWriter::FileWriter (const std::string& fileName):
  m_fileHeader(),
  m_fileFooter(),
  m_signalMap()
{
  m_file.open(fileName.c_str(),std::ios_base::binary| ios::out);
  m_file.imbue(std::locale::classic());
  if (!m_file.is_open()) throw I_FileWriter::InputFileError("cannot open output file: " +fileName );
}


FileWriter::~FileWriter()
{
  m_file.close();
}

void FileWriter::writeEventToFile( )
{
  prepareHeader();
  prepareFooter();

  saveHeader();
  saveData();
  saveFooter();
}

/**
 * this are a dummy functions for filling the data header and footer
 * but for now I cannot find any better idea how to do it
 */

void FileWriter::prepareHeader()
{
  m_fileHeader.hederLine1=	0x00000000f1ca600D;
  m_fileHeader.hederLine2=	0xabbaffffffffffff;
  m_fileHeader.packId=   	0x000064fc00000000;
  m_fileHeader.trackId=   	0x00023f5500000000;

  struct timeval tp;
  gettimeofday(&tp,NULL);
  int64_t timeStamp= tp.tv_sec * 1000 + tp.tv_usec / 1000;
  m_fileHeader.telescopeTimestamp=timeStamp;
  int offset=100;
  int64_t mambaTimestamp=timeStamp+offset;
  m_fileHeader.mambaTimestamp=mambaTimestamp;

  m_fileHeader.coincidenceID=0;
  m_fileHeader.padding=0;
}


void FileWriter::prepareFooter()
{
  m_fileFooter.dataFooter1=	0xffffffffffffffff;
  m_fileFooter.dataFooter2=	0xffffffffffffffff;
  m_fileFooter.tdcData1 =  	0x0000000000000000;
  m_fileFooter.tdcData2 =  	0x1234567800000000;

}

void FileWriter::saveHeader()
{
  m_file.write((char*)(&m_fileHeader.hederLine1), sizeof(m_fileHeader.hederLine1));
  m_file.write((char*)(&m_fileHeader.hederLine2), sizeof(m_fileHeader.hederLine2));
  m_file.write((char*)(&m_fileHeader.packId), sizeof(m_fileHeader.packId));
  m_file.write((char*)(&m_fileHeader.trackId), sizeof(m_fileHeader.trackId));
  m_file.write((char*)(&m_fileHeader.mambaTimestamp), sizeof(m_fileHeader.mambaTimestamp));
  m_file.write((char*)(&m_fileHeader.telescopeTimestamp), sizeof(m_fileHeader.telescopeTimestamp));
  m_file.write((char*)(&m_fileHeader.coincidenceID), sizeof(m_fileHeader.coincidenceID));
  m_file.write((char*)(&m_fileHeader.padding), sizeof(m_fileHeader.padding));
}

void FileWriter::saveData()
{
  BOOST_FOREACH(const auto& beetleIt,m_signalMap){
    BOOST_FOREACH(const auto& adcData,beetleIt.second){
      m_file.write((char*)(&adcData), sizeof(adcData));
    }
  }
}

void FileWriter::saveFooter()
{
  m_file.write((char*)(&m_fileFooter.dataFooter1), sizeof(m_fileFooter.dataFooter1));
  m_file.write((char*)(&m_fileFooter.dataFooter2), sizeof(m_fileFooter.dataFooter2));
  m_file.write((char*)(&m_fileFooter.tdcData1), sizeof(m_fileFooter.tdcData1));
  m_file.write((char*)(&m_fileFooter.tdcData2), sizeof(m_fileFooter.tdcData2));

}

