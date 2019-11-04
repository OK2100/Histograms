// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
// file DataBlockFileReaderFT0.h read data blocks from file
// Artur.Furs afurs@cern.ch

#ifndef ALICEO2_FIT_DataBlockFileReaderFT0_cxx
#define ALICEO2_FIT_DataBlockFileReaderFT0_cxx
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include "../sources/DataBlockReaderFT0.h"
using namespace std;
namespace o2
{
namespace ft0
{
class DataBlockFileReaderFT0:public DataBlockReaderFT0
{
public:

	DataBlockFileReaderFT0()  = default;
	DataBlockFileReaderFT0(const char *srcFilepath, const char *destFilepath, DataBlockFT0 *dataBlockFT0);

	virtual ~DataBlockFileReaderFT0();
	void InitFile();
	void SetFilepathSrc(const char * filepath)	{mFilepathSrc = filepath;}
	void SetFilepathDest(const char * filepath)	{mFilepathDest = filepath;}
	bool ReadNextEvent();
	void WriteNextEvent();
protected:
	const char *mFilepathSrc;
	const char *mFilepathDest;
	bool mIsFileSrcReady;
	bool mIsFileDestReady;
	int mSeekResult;
	size_t mSizeObjectsData,mSizeObjectsHeader;
	FILE *mFileSrc;
	FILE *mFileDest;
    const static size_t sHeaderSize = sizeof(struct EventHeader);
    const static size_t sDataSize = sizeof(struct EventData);
    const static int sHeaderDescriptor = 0x0000000f;

private:

	/////////////////////////////////////////////////
//	ClassDef(DataBlockFileReaderFT0,1);
};
}	// namespace ft0
}	// namespace o2
#endif
