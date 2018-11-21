#pragma once

#include "FuzzyGEPredictor.h"

class Constituency
{
public:
   Constituency( int id );
   ~Constituency();

   void setID( int newID );

   void setArea( string newArea );

   void setCounty( string newCounty );

   void setElectorate( string newElectorate );

   void setCON( string newCON );

   void setLAB( string newLAB );

   void setLIB( string newLIB );

   void setNAT( string newVal );

   void setName( string newName );

   void setMP( string newMP );

   void setMIN( string newVal );

   void setOTH( string newVal );

   void setUKIP( string newVal );

   void setGreen( string newVal );

   void fetchPastResults();

private:
   int iID;
   vector<string> svName;
   vector<string> svMP;
   vector<Area> evArea;
   vector<string> svCounty;
   vector<int> ivElectorate;
   vector<int> ivCON;
   vector<int> ivLAB;
   vector<int> ivLIB;
   vector<int> ivNAT;
   vector<int> ivMIN;
   vector<int> ivOTH;

   vector<int> ivUKIP;
   vector<int> ivGreen;
};

