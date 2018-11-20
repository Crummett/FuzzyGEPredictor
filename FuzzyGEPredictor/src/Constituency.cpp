// Class for an individual constituency.

#include "Constituency.h"

Constituency::Constituency()
{}

Constituency::Constituency(int id, string Name )
{
   iID = id, sName = Name;
}

Constituency::~Constituency()
{}

void Constituency::SetName( string newName )
{
   sName = newName;
}

void Constituency::setID( int newID )
{
   iID = newID;
}

void Constituency::fetchPastResults()
{
   for each ( string sElectFile in saElectData )
   {
      char result[MAX_PATH];
      string sCWD = string( result, GetModuleFileName( NULL, result, MAX_PATH ) );

      ifstream currentFile( sElectFile );
      if( currentFile.is_open() )
      {
         cout << "\nword up";
      }
      else
      {
         cout << "\nuh oh";
      }
   }
}