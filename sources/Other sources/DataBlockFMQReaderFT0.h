#ifndef ALICEO2_FIT_DataBlockFMQReaderFT0_cxx
#define ALICEO2_FIT_DataBlockFMQReaderFT0_cxx
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "DataBlockReaderFT0.h"
using namespace std;
namespace o2
{
namespace ft0
{
class DataBlockFMQReaderFT0:public DataBlockReaderFT0
{
public:
//	DataBlockFMQReaderFT0();
	DataBlockFMQReaderFT0(DataBlockFT0 *dataBlockFT0);
	virtual ~DataBlockFMQReaderFT0();

protected:

private:

	/////////////////////////////////////////////////
	ClassDef(DataBlockFMQReaderFT0,1);
};
} //namespace ft0
} // namespace o2
#endif
