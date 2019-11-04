#include "DataBlockFileReaderFT0.cxx"
#include "DataBlockFT0.cxx"
#include "DataBlockReaderFT0.cxx"
using namespace o2::ft0;
int main()	{
	DataBlockFT0 *dataBlock = new DataBlockFT0();
	DataBlockFileReaderFT0 *rawReader = new DataBlockFileReaderFT0("","srcData.bin",dataBlock);
	for(int iEvent=0;iEvent<1000;iEvent++)	{
		dataBlock->GenerateRandomEvent();
		dataBlock->Print();
		rawReader->WriteNextEvent();
	}
	delete rawReader;
	delete dataBlock;

return 0;
}

