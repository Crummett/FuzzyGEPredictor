#include "..\headers\Predictor.h"

Predictor::Predictor()
{
   for ( int constit = RegionError + 1; constit <= SouthEast; constit++ )
   {
      vConstitRegions.push_back( new ConstituencyRegion( constit ) );
   }
}

Predictor::~Predictor()
{}

void Predictor::Predict()
{
   cout << "FUZZY ELECTION PREDICTOR\n" << endl;

   bool bLoaded = false;
   vector<int> vElectionDates;

   cout << "Fetching past election results..." << endl;

   for each ( ConstituencyRegion* constReg in vConstitRegions )
   {
      bLoaded = constReg->fetchResults( &vElectionDates );

      bLoaded = constReg->CalcConstituencyVoteChanges();
   }
   if ( bLoaded )
   {
      cout << "Fetched election results from " <<
         intToDateString( vElectionDates.front() )
         << " to " << intToDateString( vElectionDates.back() ) << endl;
   }
   else
   {
      cout << "Error reading results!" << endl;
   }

   cout << "Fetching polls..." << endl;
   if ( PollReader->fetchPolls( &vPollSet ) )
   {
      cout << "Fetched polls from " <<
         intToDateString( vPollSet.front().pvPolls.front().iPollDate / 100 )
         << " to "
         << intToDateString( vPollSet.back().pvPolls.back().iPollDate / 100 )
         << endl;
      cout << "Prediction is therefore for the following month." << endl;
   }
   else
   {
      cout << "Polls not found!" << endl;
   }

   int iPollingMonths;
   cout << "\nEnter months of polling to use: " << endl;
   cin >> iPollingMonths;
   //iPollingMonths = 5;
   cout << "Aggregating " << iPollingMonths << " months of polling..." << endl;

   vector<vector<tuple<string, float>>> rankedquestions;
   AggregatePolling( iPollingMonths, vElectionDates, &rankedquestions );

   vector<tuple<string, float>> recentQuestions;
   mostRecentPolling( iPollingMonths, &recentQuestions );

   //============================//
   totalRulesCount = 0;
   int totalPreviousMPs[14] = { 0 };
   int totalNewMPs[14] = { 0 };

   time_t then = time( 0 );

   // Set up results spreadsheet
   ofstream spreadsheet;
   spreadsheet.open( "./results/results_" + to_string( then ) + " .csv" );
   spreadsheet << "sep=;" << endl;
   string parties = "Region;";
   for each ( string party in saFullPartyNames )
   {
      parties += ( party + ";" );
   }
   spreadsheet << parties << endl;

   for each ( ConstituencyRegion* constitRegion in vConstitRegions )
   {
      if ( constitRegion->iArea > 0 )
      {
         RegionHistory region = constitRegion->region;
         int previousMPs[NumOfParties] = { 0 };
         int newMPs[NumOfParties] = { 0 };
         int newMPExport[14] = { 0 };

         for ( unsigned int constituency = 0; constituency < constitRegion->region.vchConstituencies.size(); constituency++ )
         {
            ConstituencyHistory* constitHist = &region.vchConstituencies[constituency];
            if ( constitHist->viParticipatedElections.back() == vElectionDates.back() )
            {
               // Fire up the engine and make a prediction
               string constitName = region.svConstitNames[constituency];

               PredictorEngine * engine = new PredictorEngine(
                  vElectionDates,
                  region.vbUKIPGreen,
                  vPollSet,
                  rankedquestions,
                  constitName,
                  constitHist
               );

               // Export engine settings (rules etc.)
               // Export is fuzzylitelanguage (.fll) as when using .fis for matlab it causes matlab to explode.
               // Probably because this usually makes over 1300 rules...
               FllExporter exporter;
               //cout << exporter.toString( engine );
               //cout << engine.toString();
               ofstream myfile;
               myfile.open( "./rulesets/" + engine->getName() + ".fll" );

               for ( size_t r = 0; r < engine->ruleBlocks().size(); r++ )
               {
                  myfile << exporter.toString( engine->ruleBlocks()[r] ) << endl;
               }
               myfile.close();

               engine->makePrediction( recentQuestions );

               Parties prev = static_cast<Parties>( 0 );
               Parties pred = static_cast<Parties>( 0 );
               float swi = 0;

               engine->getPrediction( &prev, &pred, &swi );

               previousMPs[prev]++;
               newMPs[pred]++;

               for ( int i = 0; i < NumOfParties; i++ )
               {
                  newMPExport[i] = newMPs[i];
               }

               // Delete the engine when done. Keeping them fills up to a gig of memory.
               engine->~PredictorEngine();
            }
         }
         // Report results and add to the right lists
         Region reg = static_cast<Region>( constitRegion->iArea );

         cout << "\n" + saRegionNames[reg] << endl;
         for ( int p = 0; p < NumOfParties; p++ )
         {
            string party = saFullPartyNames[p];

            switch ( reg )
            {
            case Scotland:
               if ( p == 3 )
               { // SNP
                  party = saFullPartyNames[8];
                  int mp = newMPExport[3];
                  newMPExport[3] = 0;
                  newMPExport[8] = mp;
               }
               break;
            case Wales:
               if ( p == 3 )
               { // Plaid Cymru
                  party = saFullPartyNames[9];
                  int mp = newMPExport[3];
                  newMPExport[3] = 0;
                  newMPExport[9] = mp;
               }
               break;
            case NorthernIreland:
               if ( p <= Nationalist )
               { // UUP, SDLP, DUP, Sinn Fein
                  party = saFullPartyNames[p + 10];
                  int mp = newMPExport[p];
                  newMPExport[p] = 0;
                  newMPExport[p + 10] = mp;
               }
               break;
            }

            // Print results
            cout << party + ": " + to_string( previousMPs[p] )
               + " => " + to_string( newMPs[p] ) << endl;
         }

         string regresult = saRegionNames[constitRegion->iArea] + ";";
         for ( int i = 0; i < 14; i++ )
         {
            if ( i != Nationalist ) // Nationalist is Plaid and SNP, no need to repeat
            {
               regresult += to_string( newMPExport[i] );
               regresult += ";";

               totalNewMPs[i] += newMPExport[i];
            }
         }
         spreadsheet << regresult << endl;
      }
   }

   string regresult = "TOTALS;";
   for ( int i = 0; i < 14; i++ )
   {
      if ( i != Nationalist )
      {
         regresult += to_string( totalNewMPs[i] );
         regresult += ";";
      }
   }
   spreadsheet << regresult << endl;
   spreadsheet.close();

   cout << "\nTOTALS" << endl;
   for ( int i = 0; i < 14; i++ )
   {
      if ( i != Nationalist )
      {
         cout << saFullPartyNames[i] << ": " << to_string( totalNewMPs[i] ) << endl;
      }
   }

   time_t now = time( 0 );

   cout << "\nRuntime: " << now - then << endl;

   //cout << "\nRules generated: " << totalRulesCount << endl;
}

bool Predictor::AggregatePolling( int numMonths, vector<int> vElectionDates,
   vector<vector<tuple<string, float>>> * rankedquestions )
{
   for each ( PollSet pollset in vPollSet )
   {
      vector<Poll> polls = pollset.pvPolls;
      vector<string> questions = pollset.svQuestions;

      for each ( int date in vElectionDates )
      {
         vector<float> combinedpolls;

         int mindate = ( date - ( date % 100 ) - ( numMonths - 1 ) * 100 );
         int maxdate = ( date - ( date % 100 ) );

         for each ( Poll poll in polls )
         {
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

            float pollvalue = combinedpolls[location] / numMonths;

            if ( pollvalue > 0.0 )
            {
               newranks.push_back( { questions[location], pollvalue } );
            }

            combinedpolls[location] = 0;
         }

         rankedquestions->push_back( newranks );
      }
   }

   return true;
}

bool Predictor::mostRecentPolling( int pollingMonths, vector<tuple<string, float>> * recentQuestions )
{
   for each ( PollSet pollset in vPollSet )
   {
      vector<Poll> polls = pollset.pvPolls;
      vector<string> questions = pollset.svQuestions;
      vector<float> combinedpolls;

      for ( unsigned int p = ( polls.size() - 1 );
         p > ( polls.size() - pollingMonths );
         p-- )
      {
         Poll poll = polls[p];

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
      for ( unsigned int i = 0; i < combinedpolls.size(); i++ )
      {
         auto largest = max_element( begin( combinedpolls ), end( combinedpolls ) );
         int location = distance( begin( combinedpolls ), largest );

         float pollvalue = combinedpolls[location] / pollingMonths;

         if ( pollvalue > 0.0 )
         {
            recentQuestions->push_back( { questions[location], pollvalue } );
         }

         combinedpolls[location] = 0;
      }
   }
   return true;
}

string Predictor::intToDateString( int date )
{
   string sdate = to_string( date );
   string finalstring = "";

   switch ( sdate.length() )
   {
   case 8:
      // day
      finalstring.push_back( sdate[6] );
      finalstring.push_back( sdate[7] );
      finalstring.push_back( '/' );
   case 6:
      // month
      finalstring.push_back( sdate[4] );
      finalstring.push_back( sdate[5] );
      finalstring.push_back( '/' );
      // year
      finalstring.push_back( sdate[0] );
      finalstring.push_back( sdate[1] );
      finalstring.push_back( sdate[2] );
      finalstring.push_back( sdate[3] );
   default:
      return finalstring;
   }
}