// Class for an individual constituency.

#include "Constituency.h"

Constituency::Constituency( int id )
{
   iID = id;
}

Constituency::~Constituency()
{}

////////////////////////
//SETTERS
////////////////////////
void Constituency::setID( int newID )
{
   iID = newID;
   fetchPastResults();
}

void Constituency::setName( string newName )
{
   svName.push_back( newName );
}

void Constituency::setMP( string newMP )
{
   svMP.push_back( newMP );
}

void Constituency::setArea( string newArea )
{
   string::size_type sizetype;
   int intArea;

   intArea = stoi( newArea, &sizetype );

   evArea.push_back( Area( intArea ) );
}

void Constituency::setCounty( string newCounty )
{
   svCounty.push_back( newCounty );
}

void Constituency::setElectorate( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivElectorate.push_back( intVal );
}

void Constituency::setCON( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivCON.push_back( intVal );
}

void Constituency::setLAB( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivLAB.push_back( intVal );
}

void Constituency::setLIB( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivLIB.push_back( intVal );
}

void Constituency::setNAT( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivNAT.push_back( intVal );
}

void Constituency::setMIN( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivMIN.push_back( intVal );
}

void Constituency::setOTH( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivOTH.push_back( intVal );
}

void Constituency::setUKIP( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivElectorate.push_back( intVal );
}

void Constituency::setGreen( string newVal )
{
   string::size_type sizetype;
   int intVal;

   intVal = stoi( newVal, &sizetype );

   ivGreen.push_back( intVal );
}

////////////////////////
// ACTUAL FUNCTIONS
////////////////////////
void Constituency::fetchPastResults()
{ // Fetch csv files and take their contents
   vector<string> constituencyLines;

   for each ( string sElectFile in saElectData )
   {
      string sLine;
      ifstream currentFile( sElectFile );
      if( currentFile.is_open() )
      {
         //Ignore first 2 lines
         currentFile.ignore( 256, '\n' );
         currentFile.ignore( 256, '\n' );

         for( int i = 0; i == iID; i++ )
         {
            getline( currentFile, sLine );
            constituencyLines.push_back( sLine );
         }
      }
      else
      {
         cout << "\n" << sElectFile << "not read";
      }
   }

   for each ( string constLine in constituencyLines )
   {
      stringstream ssLine( constLine );
      vector<string> constituencyDetails;

      while( ssLine.good() )
      {
         string substring;
         getline( ssLine, substring, ';' );
         constituencyDetails.push_back( substring );
      }

      setName( constituencyDetails[0] );
      setMP( constituencyDetails[1] );
      setArea( constituencyDetails[2] );
      setCounty( constituencyDetails[3] );
      setElectorate( constituencyDetails[4] );

      setCON( constituencyDetails[5] );
      setLAB( constituencyDetails[6] );
      setLIB( constituencyDetails[7] );

      if( constituencyDetails.size() > 11 )
      {
         setUKIP( constituencyDetails[8] );
         setGreen( constituencyDetails[9] );

         setNAT( constituencyDetails[10] );
         setMIN( constituencyDetails[11] );
         setOTH( constituencyDetails[12] );
      }
      else
      {
         setUKIP( "0" );
         setGreen( "0" );

         setNAT( constituencyDetails[8] );
         setMIN( constituencyDetails[9] );
         setOTH( constituencyDetails[10] );
      }
   }
}