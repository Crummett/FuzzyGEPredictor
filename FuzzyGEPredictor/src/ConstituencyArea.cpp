#include "ConstituencyArea.h"

ConstituencyArea::ConstituencyArea()
{
   iArea = 0;
}

ConstituencyArea::ConstituencyArea( int area )
{
   iArea = area;
}

ConstituencyArea::~ConstituencyArea()
{}


bool ConstituencyArea::fetchResults()
{
   // Fetch csv files and take their contents
   int iGE = 0;

   for each ( string sElectFile in saElectData )
   {
      ifstream currentFile( sElectFile );
      GEResult newResult;

      if ( currentFile.is_open() )
      {
         //Ensure correct delimiter
         string firstLine;
         char delim = ','; // Comma is standard for csv
         getline( currentFile, firstLine );
         if ( firstLine == "sep=;" ) // if ';' is delimiter, will be specified on first line
         {
            delim = ';';
            currentFile.ignore( 999, '\n' ); // Ignore column titles
         }

         string::size_type sizetype;

         while ( not currentFile.eof() )
         {
            string sLine;
            getline( currentFile, sLine );

            if ( sLine.size() > 0 )
            {

               stringstream sStreamLine( sLine );
               vector<string> constituencyDetails;

               while ( sStreamLine.good() )
               {
                  string substring;
                  getline( sStreamLine, substring, delim );
                  constituencyDetails.push_back( substring );
               }

               int currentArea;

               currentArea = stoi( constituencyDetails[2], &sizetype );

               if ( currentArea == iArea )
               {
                  newResult.svName.push_back( constituencyDetails[0] );
                  newResult.svMP.push_back( constituencyDetails[1] );
                  newResult.svCounty.push_back( constituencyDetails[3] );

                  newResult.ivElectorate.push_back( stoi( constituencyDetails[4], &sizetype ) );
                  newResult.ivCON.push_back( stoi( constituencyDetails[4], &sizetype ) );
                  newResult.ivLAB.push_back( stoi( constituencyDetails[4], &sizetype ) );
                  newResult.ivLIB.push_back( stoi( constituencyDetails[4], &sizetype ) );

                  if ( constituencyDetails.size() > 11 )
                  {
                     newResult.ivUKIP.push_back( stoi( constituencyDetails[8], &sizetype ) );
                     newResult.ivGreen.push_back( stoi( constituencyDetails[9], &sizetype ) );

                     newResult.ivNAT.push_back( stoi( constituencyDetails[10], &sizetype ) );
                     newResult.ivMIN.push_back( stoi( constituencyDetails[11], &sizetype ) );
                     newResult.ivOTH.push_back( stoi( constituencyDetails[12], &sizetype ) );
                  }
                  else
                  {
                     newResult.ivUKIP.push_back( 0 );
                     newResult.ivGreen.push_back( 0 );

                     newResult.ivNAT.push_back( stoi( constituencyDetails[8], &sizetype ) );
                     newResult.ivMIN.push_back( stoi( constituencyDetails[9], &sizetype ) );
                     newResult.ivOTH.push_back( stoi( constituencyDetails[10], &sizetype ) );
                  }
               }
            }

         }

         if ( newResult.ivElectorate.size() > 0 )
         {
            newResult.iGEYear = ElectionYears[iGE];
            vResults.push_back( newResult );
         }

         iGE++;
      }
      else
      {
         cout << "\n" << sElectFile << "not read";
         return false;
      }
   }

   return true;
}