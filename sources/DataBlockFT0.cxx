// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
#include "DataBlockFT0.h"
using namespace o2::ft0;
using namespace std;

//ClassImp(DataBlockFT0)

DataBlockFT0::DataBlockFT0(){
    //cout<<"\n////////////////////////////////////////////////////////////////";
    //cout<<"\n/Initializating object DataBlockFT0...";
    //cout<<"\n////////////////////////////////////////////////////////////////";

    //cout<<"\n/Initialization complete!";
    //cout<<"\n////////////////////////////////////////////////////////////////\n";

}
/*******************************************************************************************************************/


/*******************************************************************************************************************/
void DataBlockFT0::GenerateData()	{
	for(int iCh=0;iCh<2*mEventHeader.Nwords;iCh++)	{
		mEventData[iCh].channelID=iCh;
		mEventData[iCh].charge = 1000;
		mEventData[iCh].time = 500;
		mEventData[iCh].is1TimeLostEvent=1;
		mEventData[iCh].is2TimeLostEvent=1;
		mEventData[iCh].isADCinGate=1;
		mEventData[iCh].isAmpHigh=1;
		mEventData[iCh].isDoubleEvent=1;
		mEventData[iCh].isEventInTVDC=1;
		mEventData[iCh].isTimeInfoLate=1;
		mEventData[iCh].isTimeInfoLost=1;
		mEventData[iCh].numberADC=1;
	}
}
/*******************************************************************************************************************/
void DataBlockFT0::GenerateHeader(Int_t nWords)	{
	mEventHeader.startDescriptor = sHeaderDescriptor;
	mEventHeader.Nwords = nWords;
	mEventHeader.reservedField = 0;
	mEventHeader.bc=200;
	mEventHeader.orbit=100;
}
/*******************************************************************************************************************/
void DataBlockFT0::GenerateRandomHeader()	{
	mEventHeader.startDescriptor = sHeaderDescriptor;
	mEventHeader.Nwords = std::rand()%7;
	mEventHeader.bc = std::rand()%2000; // 1999-max bc
	mEventHeader.orbit = std::rand()%100;
}
/*******************************************************************************************************************/
void DataBlockFT0::GenerateRandomData()	{
    DataConversion converter;

	mIsHalfWord = std::rand()%2;
	for(int iCh=0;iCh<2*mEventHeader.Nwords;iCh++)	{
		mEventData[iCh].channelID=std::rand()%208+1;

        converter.dataReal.charge = std::rand()%4195-100;
        mEventData[iCh].charge = converter.dataBlocks.charge;

        converter.dataReal.time = std::rand()%4095-2047;
        mEventData[iCh].time = converter.dataBlocks.time;

		mEventData[iCh].is1TimeLostEvent=std::rand()%2;
		mEventData[iCh].is2TimeLostEvent=std::rand()%2;
		mEventData[iCh].isADCinGate=std::rand()%2;
		mEventData[iCh].isAmpHigh=std::rand()%2;
		mEventData[iCh].isDoubleEvent=std::rand()%2;
		mEventData[iCh].isEventInTVDC=std::rand()%2;
		mEventData[iCh].isTimeInfoLate=std::rand()%2;
		mEventData[iCh].isTimeInfoLost=std::rand()%2;
		mEventData[iCh].numberADC=std::rand()%2;
	}
	if(mIsHalfWord)	mEventData[2*mEventHeader.Nwords-1].channelID = 0; //in case of half-word at the end of FT0 data block sequences
}
/*******************************************************************************************************************/
void DataBlockFT0::GenerateRandomEvent()	{
	GenerateRandomHeader();
	GenerateRandomData();
}
/*******************************************************************************************************************/
void DataBlockFT0::Print(bool doPrintData)	{
    //cout<<endl<<"==================Raw event data=================="<<endl;
    //cout<<"##################Header##################"<<endl;
    //cout<<"startDescriptor: "<<mEventHeader.startDescriptor<<endl;
    //cout<<"Nwords: "<<mEventHeader.Nwords<<endl;
    //cout<<"BC: "<<mEventHeader.bc<<endl;
    //cout<<"Orbit: "<<mEventHeader.orbit<<endl;
    //cout<<"##########################################"<<endl;
	if(!doPrintData)	return;
    //cout<<"###################DATA###################"<<endl;
	for(int iCh=0;iCh<2*mEventHeader.Nwords;iCh++)	{
        //cout<<"------------Channel "<<mEventData[iCh].channelID<<"------------"<<endl;
        //cout<<"Charge: "<<mEventData[iCh].charge<<endl;
        //cout<<"Time: "<<mEventData[iCh].time<<endl;
        //cout<<"1TimeLostEvent: "<<mEventData[iCh].is1TimeLostEvent<<endl;
        //cout<<"2TimeLostEvent: "<<mEventData[iCh].is2TimeLostEvent<<endl;
        //cout<<"ADCinGate: "<<mEventData[iCh].isADCinGate<<endl;
        //cout<<"AmpHigh: "<<mEventData[iCh].isAmpHigh<<endl;
        //cout<<"DoubleEvent: "<<mEventData[iCh].isDoubleEvent<<endl;
        //cout<<"EventInTVDC: "<<mEventData[iCh].isEventInTVDC<<endl;
        //cout<<"TimeInfoLate: "<<mEventData[iCh].isTimeInfoLate<<endl;
        //cout<<"TimeInfoLost: "<<mEventData[iCh].isTimeInfoLost<<endl;
        //cout<<"numberADC: "<<mEventData[iCh].numberADC<<endl;
	}
    //cout<<"##########################################"<<endl;
}
/*******************************************************************************************************************/
DataBlockFT0::~DataBlockFT0()	{
    //cout<<"\n////////////////////////////////////////////////////////////////";
    //cout<<"\n/Deleting object DataBlockFT0...";
    //cout<<"\n////////////////////////////////////////////////////////////////";
    //cout<<"\n/Deleting completed!";
    //cout<<"\n////////////////////////////////////////////////////////////////\n";
}

/*******************************************************************************************************************/
