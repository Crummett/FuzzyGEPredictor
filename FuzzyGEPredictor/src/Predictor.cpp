#include "..\headers\Predictor.h"

Predictor::Predictor()
{
   for ( int constit = RegionError; constit < SouthEast; constit++ )
   {
      vConstituencies.push_back( ConstituencyRegion( constit ) );
   }
}

Predictor::~Predictor()
{}

void Predictor::Predict()
{
   cout << "FUZZY ELECTION PREDICTOR\n";

   bool bLoaded = false;
   vector<int> vElectionDates;

   cout << "\nFetching past election results...";

   for each ( ConstituencyRegion constArea in vConstituencies )
   {
      bLoaded = constArea.fetchResults( &vElectionDates );

      bLoaded = constArea.CalcConstituencyVoteChanges();
   }
   if ( bLoaded )
   {
      cout << "\nFetched";
   }
   else
   {
      cout << "\nError reading results!";
   }

   cout << "\nFetching polls...";
   if ( PollReader->fetchPolls( &vPollSet ) )
   {
      cout << "\nFetched";
   }
   else
   {
      cout << "\nPolls not found!";
   }

   int iPollingMonths;
   cout << "\n\nEnter months of polling to use: ";
   cin >> iPollingMonths;
   cout << "Aggregating " << iPollingMonths << " months of polling.";

   vector<vector<tuple<string, float>>> rankedquestions;

   AggregatePolling( iPollingMonths, vElectionDates, &rankedquestions );

   int iNumInputs;
   cout << "\n\nEnter number of inputs to use: ";
   cin >> iNumInputs;
   cout << "Using " << iNumInputs << " polling inputs.";

   int iElectionYear, iElectionMonth;
   cout << "\n\nEnter election year: ";
   cin >> iElectionYear;
   cout << "And election month: ";
   cin >> iElectionMonth;
   cout << "Predicting election in " << iElectionMonth << "/" << iElectionYear << ".";

   //============================//
   Engine* engine = new Engine;
   engine->setName( "FuzzyGEPredictor" );
}

bool Predictor::AggregatePolling( int numMonths, vector<int> vElectionDates, vector<vector<tuple<string, float>>> * rankedquestions )
{
   for each ( PollSet pollset in vPollSet )
   {
      vector<Poll> polls = pollset.pvPolls;
      vector<string> questions = pollset.svQuestions;

      for each ( int date in vElectionDates )
      {
         vector<float> combinedpolls;

         for each ( Poll poll in polls )
         {
            int mindate = ( date - ( date % 100 ) - ( numMonths - 1 ) * 100 );
            int maxdate = ( date - ( date % 100 ) );

            if ( poll.iPollDate <= maxdate && poll.iPollDate >= mindate )
            {
               if ( combinedpolls.size() == 0 )
               {
                  combinedpolls = poll.fvPercentages;
               }
               else
               {
                  for ( unsigned int i = 0; i < poll.fvPercentages.size(); i++ )
                  {
                     float newval = poll.fvPercentages[i];
                     combinedpolls[i] += newval;
                  }
               }
            }
         }

         vector<tuple<string, float>> newranks;
         unsigned int vectorsize = combinedpolls.size();
         for ( unsigned int i = 0; i < vectorsize; i++ )
         {
            auto largest = max_element( begin( combinedpolls ), end( combinedpolls ) );
            int location = distance( begin( combinedpolls ), largest );

            if ( combinedpolls[location] > 0 )
            {
               newranks.push_back( { questions[location], combinedpolls[location] } );
            }
         }

         rankedquestions->push_back( newranks );
      }
   }

   return true;
}

bool Predictor::CalcConstituencyVoteChanges( ConstituencyRegion * constitArea )
{
   return true;
}
