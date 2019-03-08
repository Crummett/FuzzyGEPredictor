#include "ConstituencyRegion.h"

ConstituencyRegion::ConstituencyRegion()
{
   iArea = 0;
}

ConstituencyRegion::ConstituencyRegion( int area )
{
   iArea = area;
}

ConstituencyRegion::~ConstituencyRegion()
{}


bool ConstituencyRegion::fetchResults( vector<int> * vElectionDates )
{
   // Fetch csv files and take their contents
   vector<filesystem::path> svElectData;

   for ( const auto & entry : fs::directory_iterator( sElectDataDirectory ) )
   {
      svElectData.push_back( entry );
   }

   for each ( fs::path pElectFile in svElectData )
   {
      ifstream currentFile( pElectFile );
      GEResult newResult;

      if ( currentFile.is_open() )
      {
         string::size_type sizetype;

         //Ensure correct delimiter
         string firstLine;
         char delim = ','; // Comma is standard for csv
         getline( currentFile, firstLine );
         if ( firstLine == "sep=;" ) // if ';' is delimiter, will be specified on first line
         {
            delim = ';';
            getline( currentFile, firstLine ); // move to next line
         }

         int iElectionDate = stoi( firstLine, &sizetype ); //Read date
         newResult.iGEDate = iElectionDate;
         vElectionDates->push_back( iElectionDate );

         currentFile.ignore( 9999, '\n' ); // Ignore column titles

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
                  newResult.ivCON.push_back( stoi( constituencyDetails[5], &sizetype ) );
                  newResult.ivLAB.push_back( stoi( constituencyDetails[6], &sizetype ) );
                  newResult.ivLIB.push_back( stoi( constituencyDetails[7], &sizetype ) );

                  if ( constituencyDetails.size() > 11 )
                  {
                     newResult.bUKIPGreen = true;
                     newResult.ivUKIP.push_back( stoi( constituencyDetails[8], &sizetype ) );
                     newResult.ivGreen.push_back( stoi( constituencyDetails[9], &sizetype ) );

                     newResult.ivNAT.push_back( stoi( constituencyDetails[10], &sizetype ) );
                     newResult.ivMIN.push_back( stoi( constituencyDetails[11], &sizetype ) );
                     newResult.ivOTH.push_back( stoi( constituencyDetails[12], &sizetype ) );
                  }
                  else
                  {
                     newResult.bUKIPGreen = false;
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
            vResults.push_back( newResult );
         }
      }
      else
      {
         cout << "\n" << pElectFile << "not read";
         return false;
      }
   }

   return true;
}

bool ConstituencyRegion::CalcConstituencyVoteChanges()
{
   if ( vResults.size() < 2 )
   {
      return false; // can't compare 1 or 0 results!
   }

   int currentGE = 0;

   for each ( GEResult result in vResults )
   {
      region.viElectionDates.push_back( result.iGEDate );
      region.vbUKIPGreen.push_back( result.bUKIPGreen );

      for each ( string constituency in result.svName )
      {
         int location = 0;

         // find
         auto contains = std::find( region.svName.begin(), region.svName.end(), constituency );

         if ( contains != region.svName.end() )
         { // Add result to existing ConstituencyHistory

            contains = std::find( result.svName.begin(), result.svName.end(), constituency );
            location = distance( result.svName.begin(), contains );

            auto containsRegion = std::find( region.svName.begin(), region.svName.end(), constituency );
            int regionlocation = distance( region.svName.begin(), containsRegion );

            int iElectorate = result.ivElectorate[location];

            for ( int i = region.vchConstituencies[regionlocation].viElectorate.size(); i < currentGE; i++ )
            {
               region.vchConstituencies[regionlocation].viElectorate.push_back( 0 );
               region.vchConstituencies[regionlocation].viTurnout.push_back( 0 );

               for ( int p = 0; p < NumOfParties; p++ )
               {
                  region.vchConstituencies[regionlocation].voteshare[p].push_back( 0 );
               }
            }

            int turnout = ( result.ivCON[location] + result.ivLAB[location] + result.ivLIB[location] + result.ivNAT[location] +
                            result.ivMIN[location] + result.ivOTH[location] + result.ivUKIP[location] + result.ivGreen[location] )
               / iElectorate;

            region.vchConstituencies[regionlocation].viTurnout.push_back( turnout );
            region.vchConstituencies[regionlocation].viElectorate.push_back( iElectorate );

            vector<float> * voteshare = region.vchConstituencies[regionlocation].voteshare;

            voteshare[Conservatives].push_back( (float) result.ivCON[location] / (float) turnout * 100 );
            voteshare[Labour].push_back( (float) result.ivLAB[location] / (float) turnout * 100 );
            voteshare[LibDems].push_back( (float) result.ivLIB[location] / (float) turnout * 100 );
            voteshare[Nationalist].push_back( (float) result.ivNAT[location] / (float) turnout * 100 );
            voteshare[Minor].push_back( (float) result.ivMIN[location] / (float) turnout * 100 );
            voteshare[Other].push_back( (float) result.ivOTH[location] / (float) turnout * 100 );
            voteshare[UKIP].push_back( (float) result.ivUKIP[location] / (float) turnout * 100 );
            voteshare[Green].push_back( (float) result.ivGreen[location] / (float) turnout * 100 );
         }
         else // Not in previous results so create new ConstituencyHistory
         {

            region.svName.push_back( constituency );
            ConstituencyHistory newConstitHist;

            for ( int i = newConstitHist.viElectorate.size(); i < currentGE; i++ )
            {
               newConstitHist.viElectorate.push_back( 0 );
               newConstitHist.viTurnout.push_back( 0 );

               for ( int p = 0; p < NumOfParties; p++ )
               {
                  newConstitHist.voteshare[p].push_back( 0 );
               }
            }

            contains = find( result.svName.begin(), result.svName.end(), constituency );
            location = distance( result.svName.begin(), contains );

            // And add new result
            int iElectorate = result.ivElectorate[location];
            newConstitHist.viElectorate.push_back( iElectorate );

            int turnout = ( result.ivCON[location] + result.ivLAB[location] + result.ivLIB[location] + result.ivNAT[location] +
                            result.ivMIN[location] + result.ivOTH[location] + result.ivUKIP[location] + result.ivGreen[location] )
               / iElectorate;

            newConstitHist.viTurnout.push_back( turnout );

            vector<float> * voteshare = newConstitHist.voteshare;

            voteshare[Conservatives].push_back( (float) result.ivCON[location] / (float) turnout * 100 );
            voteshare[Labour].push_back( (float) result.ivLAB[location] / (float) turnout * 100 );
            voteshare[LibDems].push_back( (float) result.ivLIB[location] / (float) turnout * 100 );
            voteshare[Nationalist].push_back( (float) result.ivNAT[location] / (float) turnout * 100 );
            voteshare[Minor].push_back( (float) result.ivMIN[location] / (float) turnout * 100 );
            voteshare[Other].push_back( (float) result.ivOTH[location] / (float) turnout * 100 );
            voteshare[UKIP].push_back( (float) result.ivUKIP[location] / (float) turnout * 100 );
            voteshare[Green].push_back( (float) result.ivGreen[location] / (float) turnout * 100 );

            region.vchConstituencies.push_back( newConstitHist );
         }

      }
      currentGE++;
   }

   return true;
}