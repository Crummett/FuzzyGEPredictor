#include "PollingReader.h"

PollingReader::PollingReader()
{}

PollingReader::~PollingReader()
{}

bool PollingReader::fetchPolls( vector<PollSet> * vPollSet )
{
   // Fetch csv files and take their contents
   vector < fs::path > svPollData;
   for ( const auto & entry : fs::directory_iterator( sPollDataDirectory ) )
   {
      svPollData.push_back( entry );
   }

   for each ( fs::path pPollingFile in svPollData )
   {
      ifstream currentFile( pPollingFile );
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
               sStreamFirstLine.ignore( 9999, ';' );
               sStreamFirstLine.ignore( 9999, ';' );

               string substring;

               while ( getline( sStreamFirstLine, substring, delim ) )
               {
                  // remove whitespace
                  substring.erase(
                     remove_if(
                        substring.begin(), substring.end(), isspace ), substring.end() );
                  // remove slashes
                  substring.erase( remove( substring.begin(), substring.end(), '/' ),
                     substring.end() );
                  // remove brackets
                  substring.erase( remove( substring.begin(), substring.end(), '(' ),
                     substring.end() );
                  substring.erase( remove( substring.begin(), substring.end(), ')' ),
                     substring.end() );
                  // remove apostrophes
                  substring.erase( remove( substring.begin(), substring.end(), '\'' ),
                     substring.end() );
                  vsQuestions.push_back( substring );
               }

               newPollSet.svQuestions = vsQuestions;
            }

            unsigned int iTotalCols = vsQuestions.size() + 2; // Number of questions + Month and Year columns

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

                     newPoll.iPollDate = convertYear( pollDetails[1] ) * 10000;
                     newPoll.iPollDate = newPoll.iPollDate + ( convertMonth( pollDetails[0] ) * 100 );

                     // Add the results. Start from column 2 to avoid year and month columns.
                     for ( unsigned int percent = 2; percent < iTotalCols; percent++ )
                     {
                        newPoll.fvPercentages.push_back(
                           convertPercentString( pollDetails[percent] ) );
                     }

                     newPollSet.pvPolls.push_back( newPoll );
                  }
               }
            }
         }

         vPollSet->push_back( newPollSet );
      }
      else
      {
         cout << "\n" << pPollingFile << "not read";
         return false;
      }
   }
   return true;
}

Month PollingReader::convertMonth( string month )
{
   // Capitalise
   transform( month.begin(),
      month.end(),
      month.begin(),
      []( char c ) { return static_cast<char>( std::toupper( c ) ); } );

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