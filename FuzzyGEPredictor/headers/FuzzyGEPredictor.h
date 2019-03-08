// FuzzyGEPredictor.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include "fl/Headers.h"

#include <windows.h>

using namespace std;
using namespace fl;

namespace fs = std::filesystem;


// CONSTANTS
const enum Region
{ // Constituencies are organised into their EU constituencies
   RegionError,
   NorthernIreland,
   Scotland,
   NorthEast,
   NorthWest,
   YorksHumber,
   Wales,
   WestMidlands,
   EastMidlands,
   EastEngland,
   SouthWest, // Includes Gibraltar
   London,
   SouthEast
};

const string saRegionNames[13]
{ // Constituencies are organised into their EU constituencies
   "Error",
   "Northern Ireland",
   "Scotland",
   "North East England",
   "North West England",
   "Yorkshire and the Humber",
   "Wales",
   "West Midlands",
   "East Midlands",
   "East of England",
   "South West England", // Includes Gibraltar
   "London",
   "South East England"
};

const string sElectDataDirectory = "./electdata/";

const string sPollDataDirectory = "./pollingdata/";

const string sEngineSettings = "./enginesettings/";

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

const enum Parties
{
   Conservatives,
   Labour,
   LibDems,
   Nationalist,
   Minor,
   Other,
   UKIP,
   Green,

   NumOfParties = 8
};

const string saPartyNames[14]
{
   "Conservatives",
   "Labour",
   "Liberal Democrats",
   "Nationalist",
   "Minor",
   "Other",
   "UKIP",
   "Green",
   "SNP",
   "Plaid Cymru",
   "UUP",
   "SDLP",
   "DUP",
   "Sinn Fein"
};

#include "ConstituencyRegion.h"
#include "PollingReader.h"
#include "Predictor.h"