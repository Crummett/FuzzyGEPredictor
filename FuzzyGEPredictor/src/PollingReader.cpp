#include "PollingReader.h"

PollingReader::PollingReader()
{}


PollingReader::~PollingReader()
{}

bool PollingReader::fetchPolls()
{
   // Fetch csv files and take their contents
   for each ( string sPollingFile in saPollData )
   {
      ifstream currentFile( sPollingFile );
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
            getline( currentFile, firstLine );
         }

         PollSet newPollSet;

         if ( firstLine.size() > 0 )
         {
            stringstream sStreamFirstLine( firstLine );
            vector<string> vsQuestions;

            // Extract questions
            if ( sStreamFirstLine.good() )
            {
               sStreamFirstLine.ignore( 999, ';' );
               sStreamFirstLine.ignore( 999, ';' );

               string substring;

               while ( getline( sStreamFirstLine, substring, delim ) )
               {
                  vsQuestions.push_back( substring );
               }

               newPollSet.svQuestions = vsQuestions;
            }

            int iTotalCols = vsQuestions.size() + 2; // Number of questions + Month and Year columns

            while ( not currentFile.eof() )
            {
               string sLine;
               getline( currentFile, sLine );

               if ( sLine.size() > 0 )
               {
                  stringstream sStreamLine( sLine );
                  vector<string> pollDetails;

                  while ( sStreamLine.good() )
                  {
                     string substring;
                     getline( sStreamLine, substring, delim );
                     pollDetails.push_back( substring );
                  }

                  if ( pollDetails.size() == iTotalCols ) // if all questions answered
                  {
                     Poll newPoll;

                     newPoll.eMonth = convertMonth( pollDetails[0] );
                     newPoll.iYear = convertYear( pollDetails[1] );

                     for ( int percent = 2; percent < iTotalCols; percent++ )
                     {
                        newPoll.fvPercentages.push_back(
                           convertPercentString( pollDetails[percent] ) );
                     }

                     newPollSet.pvPolls.push_back( newPoll );
                  }
               }
            }
         }

         vPollSet.push_back( newPollSet );
      }
      else
      {
         cout << "\n" << sPollingFile << "not read";
         return false;
      }

   }
   return true;
}

Month PollingReader::convertMonth( string month )
{
   transform( month.begin(), month.end(), month.begin(), ::toupper );

   if ( month == "JAN" )
   {
      return January;
   }
   else if ( month == "FEB" )
   {
      return February;
   }
   else if ( month == "MAR" )
   {
      return March;
   }
   else if ( month == "APR" )
   {
      return April;
   }
   else if ( month == "MAY" )
   {
      return May;
   }
   else if ( month == "JUN" )
   {
      return June;
   }
   else if ( month == "JUL" )
   {
      return July;
   }
   else if ( month == "AUG" )
   {
      return August;
   }
   else if ( month == "SEP" )
   {
      return September;
   }
   else if ( month == "OCT" )
   {
      return October;
   }
   else if ( month == "NOV" )
   {
      return November;
   }
   else if ( month == "DEC" )
   {
      return December;
   }

   return MonthErr;
}

int PollingReader::convertYear( string year )
{
   string::size_type sizetype;

   return stoi( year, &sizetype );
}

float PollingReader::convertPercentString( string percent )
{
   string::size_type sizetype;
   if ( percent.size() == NULL )
   {
      return 0;
   }
   else
   {
      return stof( percent, &sizetype );
   }
}
