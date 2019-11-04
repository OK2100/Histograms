// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#include "../sources/DataBlockFileReaderFT0.h"
#include "../sources/DataBlockFT0.h"


//ClassImp(DataBlockFileReaderFT0)
using namespace o2;
using namespace ft0;
/*******************************************************************************************************************/
DataBlockFileReaderFT0::DataBlockFileReaderFT0(const char *srcFilepath, const char *destFilepath
										   , DataBlockFT0 *dataBlockFT0):
	DataBlockReaderFT0(dataBlockFT0),
	mIsFileSrcReady(false),mIsFileDestReady(false),mFileSrc(NULL),mFileDest(NULL),
	mFilepathSrc(srcFilepath),mFilepathDest(destFilepath)
{
    //cout<<"\n////////////////////////////////////////////////////////////////";
    //cout<<"\n/Initializating object DataBlockFileReaderFT0...";
    //cout<<"\n////////////////////////////////////////////////////////////////";
	InitFile();
    //cout<<"\n/Initialization complete!";
    //cout<<"\n////////////////////////////////////////////////////////////////\n";

}
/*******************************************************************************************************************/
void DataBlockFileReaderFT0::InitFile()	{
	if(mFileSrc) fclose(mFileSrc);
	mFileSrc = fopen(mFilepathSrc,"rb");
	if(!mFileSrc)	{
        //cout<<endl<<"Warning! Cannot open source file!"<<endl;
	}
	if(mFileDest)	fclose(mFileDest);
	mFileDest = fopen(mFilepathDest,"wb+");
	if(!mFileDest)	{
        //cout<<endl<<"Warning! Cannot create destination file!"<<endl;
	}
	if(mFileSrc)	mIsFileSrcReady = true;
	if(mFileDest)	mIsFileDestReady = true;
}
/*******************************************************************************************************************/
bool DataBlockFileReaderFT0::ReadNextEvent()	{
    assert(mFileSrc);

    mSizeObjectsHeader = fread(mPtrEventHeader,sHeaderSize,1,mFileSrc);
	///Looking for header,by shifting bytes,
	///  if descriptor isn't correct
	while(mEventHeader.startDescriptor-sHeaderDescriptor)	{
		mSeekResult = fseek(mFileSrc,1-sHeaderSize,SEEK_CUR);
		//if(mSeekResult) return false;
		mSizeObjectsHeader = fread(mPtrEventHeader,sHeaderSize,1,mFileSrc);
		if(feof(mFileSrc)) return false;
	}
	if(!mSizeObjectsHeader)		return false;
	///
	mNchannels = 2*mEventHeader.Nwords;
    if(mNchannels)	mSizeObjectsData = fread(mPtrEventData,sDataSize,mNchannels,mFileSrc);
	if(!mSizeObjectsData) return false;
	return true;
}
/*******************************************************************************************************************/
void DataBlockFileReaderFT0::WriteNextEvent()	{
	assert(mFileDest);
	mSizeObjectsHeader = fwrite(mPtrEventHeader,sHeaderSize,1,mFileDest);
	mSizeObjectsData = fwrite(mPtrEventData,sDataSize,2*mEventHeader.Nwords,mFileDest);
}
/*******************************************************************************************************************/
DataBlockFileReaderFT0::~DataBlockFileReaderFT0()	{
    //cout<<"\n////////////////////////////////////////////////////////////////";
    //cout<<"\n/Deleting object DataBlockFileReaderFT0...";
    //cout<<"\n////////////////////////////////////////////////////////////////";
	if(mFileSrc)	fclose(mFileSrc);
	if(mFileDest)	fclose(mFileDest);
	mFileSrc = NULL;
	mFileDest = NULL;
    //cout<<"\n/Deleting completed!";
    //cout<<"\n////////////////////////////////////////////////////////////////\n";
}

/*******************************************************************************************************************/
