#ifndef DataBlockReaderFT0_cxx
#define DataBlockReaderFT0_cxx
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../sources/DataBlockFT0.h"
using namespace o2;
using namespace ft0;


class DataBlockReaderFT0
{
public:

    DataBlockReaderFT0(DataBlockFT0 *dataBlockFT0);
	virtual ~DataBlockReaderFT0();

protected:
    Int_t mNchannels;
    EventHeader &mEventHeader;
    EventData &fEventDataFT0;
    void *mPtrEventHeader;
    void *mPtrEventData;

private:

	/////////////////////////////////////////////////
//	ClassDef(DataBlockReaderFT0,1);
};
#endif
