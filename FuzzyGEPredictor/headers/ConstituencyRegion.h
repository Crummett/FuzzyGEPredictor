#pragma once
#include "FuzzyGEPredictor.h"

struct GEResult
{
   int            iGEDate;

   vector<string> svName;
   vector<string> svMP;
   vector<string> svCounty;
   vector<int>    ivElectorate;

   // FOR ENGLAND/SCOTLAND/WALES: (eRegions 2 to 12)
   vector<int>    ivCON; // votes received by the Conservative candidate
   vector<int>    ivLAB; // votes received by the Labour candidate
   vector<int>    ivLIB; // votes received by the Liberal Democrat candidate, or Liberal candidate pre - 1988
   vector<int>    ivNAT; // votes received by the nationalist candidate (SNP in Scotland, Plaid Cymru in Wales)
   vector<int>    ivMIN; // votes received by a minor party candidate (separated from 'Other' to preserve rankings), or the SDP candidate 1983-1987
   vector<int>    ivOTH; // votes received by all other candidates

   vector<int>    ivUKIP; // votes received by the UKIP candidate
   vector<int>    ivGreen; // votes received by the Green candidate
   bool           bUKIPGreen; // bool as to whether UKIP/Green are included

   /*
      FOR NORTHERN IRELAND: (eRegion 1)
      CON : votes received by the OUP, UUP, or UCUNF candidate
      LAB : votes received by the SDLP candidate
      LIB : votes received by the DUP candidate
      NAT : votes received by the Sinn Fein candidate
   */

   // Electoral results courtesy of taken from electoralcalculus.co.uk/flatfile.html
};

struct ConstituencyHistory
{
   vector<int> viElectorate;
   vector<int> viTurnout;

   // Changes in percentage of vote share
   vector<float> voteshare[NumOfParties];
};

struct RegionHistory
{
   vector<int> viElectionDates;
   vector<string> svName;
   vector<bool> vbUKIPGreen;
   vector<ConstituencyHistory> vchConstituencies;
};

class ConstituencyRegion
{
public:
   ConstituencyRegion();
   ConstituencyRegion( int area );
   virtual ~ConstituencyRegion();

   bool fetchResults( vector<int>* vElectionDates );

   bool CalcConstituencyVoteChanges();

private:
   int iArea;
   vector<GEResult> vResults;
   RegionHistory region;
};

