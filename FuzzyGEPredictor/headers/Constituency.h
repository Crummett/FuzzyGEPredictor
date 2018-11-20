#pragma once

#include "FuzzyGEPredictor.h"

class Constituency
{
public:
    Constituency();
    Constituency( int id, string Name );
    ~Constituency();

    void SetName( string newName );

    void setID( int newID );

    void fetchPastResults();

private:
   int iID;
   string sName;
};

