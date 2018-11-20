// FuzzyGEPredictor.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>

#include "fl/Headers.h"

using namespace fl;
using namespace std;

const enum Area
{
   NotSure,
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
{
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

#include "Constituency.h"

#include <windows.h>