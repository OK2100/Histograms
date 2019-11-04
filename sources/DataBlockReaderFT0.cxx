#include "DataBlockReaderFT0.h"

using namespace std;
using std::cout;
using std::endl;

//ClassImp(DataBlockReaderFT0)

DataBlockReaderFT0::DataBlockReaderFT0(DataBlockFT0 *dataBlockFT0):
    mEventHeader(*dataBlockFT0->GetEventHeaderPtr()),
	fEventDataFT0(*dataBlockFT0->GetEventDataPtr())
{
    //cout<<"\n////////////////////////////////////////////////////////////////";
    //cout<<"\n/Initializating object DataBlockReaderFT0...";
    //cout<<"\n////////////////////////////////////////////////////////////////";
    mPtrEventHeader = dataBlockFT0->GetEventHeaderPtr();
    mPtrEventData = dataBlockFT0->GetEventDataPtr();
    //cout<<"\n/Initialization complete!";
    //cout<<"\n////////////////////////////////////////////////////////////////\n";

}
/*******************************************************************************************************************/

/*******************************************************************************************************************/
DataBlockReaderFT0::~DataBlockReaderFT0()	{
    //cout<<"\n////////////////////////////////////////////////////////////////";
    //cout<<"\n/Deleting object DataBlockReaderFT0...";
    //cout<<"\n////////////////////////////////////////////////////////////////";
    //cout<<"\n/Deleting completed!";
    //cout<<"\n////////////////////////////////////////////////////////////////\n";
}

/*******************************************************************************************************************/
