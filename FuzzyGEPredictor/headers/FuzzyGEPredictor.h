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

const string saPartyNames[8]
{
   "Conservatives",
   "Labour",
   "LibDems",
   "Nationalist",
   "Minor",
   "Other",
   "UKIP",
   "Green"
};

const string saFullPartyNames[14]
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

const enum Swing
{
   MassiveLoss = -30,
   HugeLoss = -24,
   BigLoss = -18,
   MajorLoss = -12,
   MinorLoss = -6,
   TinyLoss = -3,
   NoChange = 0,
   TinyGain = 3,
   MinorGain = 6,
   MajorGain = 12,
   BigGain = 18,
   HugeGain = 24,
   MassiveGain = 30
};

const string saOutputTerms[12]
{
   "massiveLoss",
   "hugeLoss",
   "bigLoss",
   "majorLoss",
   "minorLoss",
   "tinyLoss",
   "tinyGain",
   "minorGain",
   "majorGain",
   "bigGain",
   "hugeGain",
   "massiveGain"
};

const string saInputTerms[3]
{
   "top",
   "medium",
   "bottom",
};

#include "ConstituencyRegion.h"
#include "PollingReader.h"
#include "Predictor.h"
#include "PredictorEngine.h"