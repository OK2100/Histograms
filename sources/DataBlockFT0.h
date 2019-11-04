// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
// file DataBlockFT0.h structure of FT0 data blocks
// Artur.Furs afurs@cern.ch

#ifndef ALICEO2_FIT_DataBlockFT0_cxx
#define ALICEO2_FIT_DataBlockFT0_cxx
//#define NCHANNELS_FIT 208
//#define NCHANNELS_PM 12
//#define NBITS_EVENTDATA 9
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>
typedef int Int_t;

namespace o2
{
namespace ft0
{
constexpr int NCHANNELS_FT0=208;
constexpr int NCHANNELS_PM=12;
constexpr int NPMs=18;
constexpr int NBITS_EVENTDATA=9;
struct EventHeader	{
	uint64_t startDescriptor:4,
		Nwords:4,
		reservedField:32, ///will be used as event id
		orbit:32,
		bc:12;
//	ClassDefNV(EventHeader, 1);
};
///for data conversion

/*
		//Fill data
		EventDataUnion foo;
		foo.ED2.time = ...
		//Extract data
		... = foo.ED.time
*/
	struct EventData	{
		uint64_t time:12,
			charge:13,
			numberADC:1,
			isDoubleEvent:1,
			is1TimeLostEvent:1,
			is2TimeLostEvent:1,
			isADCinGate:1,
			isTimeInfoLate:1,
			isAmpHigh:1,
			isEventInTVDC:1,
			isTimeInfoLost:1,
			reservedField:2,
			channelID:4; /// equal to zero in case of half-word
//		ClassDefNV(EventData, 1);
	};
	struct EventDataReal {
			int time:12;
			int	charge:13;
			bool	numberADC:1,
				isDoubleEvent:1,
				is1TimeLostEvent:1,
				is2TimeLostEvent:1,
				isADCinGate:1,
				isTimeInfoLate:1,
				isAmpHigh:1,
				isEventInTVDC:1,
				isTimeInfoLost:1;
			unsigned short	reservedField:2,
				channelID:4;
//			ClassDefNV(EventData, 1);
	};
	union DataConversion {
		EventData dataBlocks;
		EventDataReal dataReal;
//		ClassDefNV(EventData, 1);
	};

class DataBlockFT0
{
public:
	DataBlockFT0();
	virtual ~DataBlockFT0();
	void GenerateHeader(Int_t nWords); ///Generate header for nWords (range: 0-4)
	void GenerateData();
	void GenerateRandomHeader();
	void GenerateRandomData();
	void GenerateRandomEvent();
	EventHeader *GetEventHeaderPtr()	{return &mEventHeader;}
	EventData *GetEventDataPtr()	{return mEventData;}
	void Print(bool doPrintData=false);

	const static size_t sHeaderSize = sizeof(struct EventHeader);
	const static size_t sDataSize = sizeof(struct EventData);
	const static int sHeaderDescriptor = 0x0000000f;
	enum EEventDataBit { kNumberADC,
						   kIsDoubleEvent,
						   kIs1TimeLostEvent,
						   kIs2TimeLostEvent,
						   kIsADCinGate,
						   kIsTimeInfoLate,
						   kIsAmpHigh,
						   kIsEventInTVDC,
						   kIsTimeInfoLost };
protected:
	uint64_t mNwords;
	uint64_t mOrbit;
	uint64_t mBC;
	int mTime[NCHANNELS_PM];
	int mCharge[NCHANNELS_PM];
	std::bitset<NBITS_EVENTDATA> mEventDataBits[NCHANNELS_PM];
	int mNchannels;
	/*
	// concept which will be realized soon
	std::map<int,int> mTimeMap; // pair(channelID,time)
	std::map<int,int> mChargeMap; // pair(channelID,charge)
	std::map<int, std::bitset<NBITS_EVENTDATA>> mBitsMap; // pair(channelID,eventDataBits)
	*/
//	o2::InteractionRecord mIntRecord;
	bool mIsHalfWord;	//case of half-word at the end of FT0 data block sequences
	EventHeader mEventHeader;
	EventData mEventData[NCHANNELS_PM];
private:

	/////////////////////////////////////////////////
//	ClassDefNV(DataBlockFT0,1);
};
}	//namespace ft0
}	//namespace o2
#endif
