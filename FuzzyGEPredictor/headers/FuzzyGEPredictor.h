// FuzzyGEPredictor.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include "fl/Headers.h"

using namespace fl;
using namespace std;

const enum Area
{ // The constituency regions
   AreaErr,
   NorthernIreland,
   Scotland,
   NorthEast,
   NorthWest,
   YorksHumber,
   Wales,
   WestMidlands,
   EastMidlands,
   Anglia,
   SouthWest,
   London,
   SouthEast
};

const string saElectData[9]
{ // Filepaths of the electoral data files
   ".\\electdata\\electdata_1983.csv",
   ".\\electdata\\electdata_1987.csv",
   ".\\electdata\\electdata_1992ob.csv",
   ".\\electdata\\electdata_1997.csv",
   ".\\electdata\\electdata_2001ob.csv",
   ".\\electdata\\electdata_2005ob.csv",
   ".\\electdata\\electdata_2010.csv",
   ".\\electdata\\electdata_2015.csv",
   ".\\electdata\\electdata_2017.csv",
};

const string saPollData[1]
{ // Filepaths of the polling data files, seperated by decade/question set
   ".\\pollingdata\\polling.csv",
};

const int ElectionYears[9]
{ //Election years of the equivalent election data files
   1983,
   1987,
   1992,
   1997,
   2001,
   2005,
   2010,
   2015,
   2017,
};

const enum Month
{
   MonthErr,
   January,
   February,
   March,
   April,
   May,
   June,
   July,
   August,
   September,
   October,
   November,
   December
};

#include "ConstituencyArea.h"
#include "PollingReader.h"

#include <windows.h>