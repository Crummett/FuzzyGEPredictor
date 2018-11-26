#pragma once
#include "FuzzyGEPredictor.h"

struct GEResult
{
   int      iGEYear;

   vector<string> svName;
   vector<string> svMP;
   vector<string> svCounty;
   vector<int>    ivElectorate;
   vector<int>    ivCON;
   vector<int>    ivLAB;
   vector<int>    ivLIB;
   vector<int>    ivNAT;
   vector<int>    ivMIN;
   vector<int>    ivOTH;

   vector<int>    ivUKIP;
   vector<int>    ivGreen;
   bool           bUKIPGreen;
};

class ConstituencyArea
{
public:
   ConstituencyArea();
   ConstituencyArea( int area );
   virtual ~ConstituencyArea();

   bool fetchResults();

private:
   int iArea;
   vector<GEResult> vResults;
};

