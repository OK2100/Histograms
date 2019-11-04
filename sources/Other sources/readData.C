#include "DataBlockFileReaderFT0.cxx"
#include "DataBlockFT0.cxx"
#include "DataBlockReaderFT0.cxx"
using namespace o2::ft0;

int main(){
//void readData()	{
	DataBlockFT0 *dataBlock = new DataBlockFT0;
	DataBlockFileReaderFT0 *rawReader = new DataBlockFileReaderFT0("srcData.bin","",dataBlock);
	rawReader->SetFilepathSrc("srcData.bin");
	rawReader->InitFile();
	while(rawReader->ReadNextEvent())	{
		dataBlock->Print(1);
	}
	delete rawReader;
	delete dataBlock;

	return 0;
}

