#include "PredictorEngine.h"

PredictorEngine::PredictorEngine(
   vector<int> vElectionDates,
   vector<bool> vbUKIPGreen,
   vector<PollSet> unrankedQuestions,
   vector<vector<tuple<string, float>>> rankedquestions,
   string constitName,
   ConstituencyHistory* constitHist )
{
   sName = constitName;
   sName.erase(
      remove_if(
         sName.begin(), sName.end(), isspace ), sName.end() );
   thisConstitHist = constitHist;
   this->setName( sName );
   generateInputs( unrankedQuestions );
   generateOutputs();

   //cout << this->toString() << endl;

   if ( not generateRuleBlock( vElectionDates, vbUKIPGreen, rankedquestions, constitHist ) )
   {
      cout << "Failed to add rules to " + sName + "!";
   }

   //cout << this->toString() << endl;
}

PredictorEngine::~PredictorEngine()
{}

bool PredictorEngine::generateInputs( vector<PollSet> vPollSet )
{
   // produce an input set for each polling question
   for each ( PollSet poll in vPollSet )
   {
      for each ( string question in poll.svQuestions )
      {
         InputVariable* newInput = new InputVariable;

         newInput->setName( question );
         newInput->setDescription( "" );
         newInput->setEnabled( true );
         newInput->setLockValueInRange( true );
         newInput->setRange( 0.000, 10.000 );

         newInput->addTerm( new Trapezoid( "top", 5.000, 7.500, 10.000, 10.000 ) );
         newInput->addTerm( new Triangle( "medium", 2.500, 5.000, 7.500 ) );
         newInput->addTerm( new Trapezoid( "bottom", 0.000, 0.000, 2.500, 5.000 ) );

         addInputVariable( newInput );

         //cout << newInput->fuzzify( 10 ) << endl;
         //cout << newInput->fuzzify( 5 ) << endl;
         //cout << newInput->fuzzify( 0 ) << endl;
      }
   }

   return true;
}

void PredictorEngine::generateOutputs()
{
   // use full set of party names
   for each ( string party in saPartyNames )
   {
      OutputVariable* newOutput = new OutputVariable;

      newOutput->setName( party );
      newOutput->setDescription( "" );
      newOutput->setEnabled( true );
      newOutput->setRange( MassiveGain, MassiveLoss );
      newOutput->setAggregation( new AlgebraicSum );
      newOutput->setDefuzzifier( new WeightedAverage );
      newOutput->setDefaultValue( fl::nan );
      newOutput->setLockPreviousValue( false );

      // Set gains
      newOutput->addTerm( new Ramp( "massiveGain", 0.000, MassiveGain ) );
      newOutput->addTerm( new Ramp( "hugeGain", 0.000, HugeGain ) );
      newOutput->addTerm( new Ramp( "bigGain", 0.000, BigGain ) );
      newOutput->addTerm( new Ramp( "majorGain", 0.000, MajorGain ) );
      newOutput->addTerm( new Ramp( "minorGain", 0.000, MinorGain ) );
      newOutput->addTerm( new Ramp( "tinyGain", 0.000, TinyGain ) );

      newOutput->addTerm( new Ramp( "noChange", NoChange, 0 ) );

      // set losses
      newOutput->addTerm( new Ramp( "massiveLoss", MassiveLoss, 0.0 ) );
      newOutput->addTerm( new Ramp( "hugeLoss", HugeLoss, 0.0 ) );
      newOutput->addTerm( new Ramp( "bigLoss", BigLoss, 0.0 ) );
      newOutput->addTerm( new Ramp( "majorLoss", MajorLoss, 0.0 ) );
      newOutput->addTerm( new Ramp( "minorLoss", MinorLoss, 0.0 ) );
      newOutput->addTerm( new Ramp( "tinyLoss", TinyLoss, 0.0 ) );

      newOutput->addTerm( new Ramp( "left", 1.000, 0.000 ) );
      newOutput->addTerm( new Ramp( "right", 0.000, 1.000 ) );

      this->addOutputVariable( newOutput );
   }
}

bool PredictorEngine::generateRuleBlock( vector<int> vElectionDates,
   vector<bool> vbUKIPGreen,
   vector<vector<tuple<string, float>>> rankedquestions,
   ConstituencyHistory* constitHist )
{
   bool rulesAdded = false;

   for ( unsigned int year = 1; year < vElectionDates.size(); year++ )
   {
      RuleBlock* newRuleBlock = new RuleBlock();
      newRuleBlock->setName( sName + "Rules" + to_string( vElectionDates[year] ) );
      newRuleBlock->setDescription( "" );
      newRuleBlock->setEnabled( true );
      newRuleBlock->setConjunction( new Minimum );
      newRuleBlock->setDisjunction( new AlgebraicSum );
      newRuleBlock->setImplication( new Minimum );
      newRuleBlock->setActivation( new General );

      vector<tuple<string, float>> questionSet = rankedquestions[year];

      int relevantQuestions = questionSet.size();
      bool bUKIPGreen = vbUKIPGreen[year];

      // rule takes form:
      // 'if *question* is *input* then *party* is *output*'
      for ( unsigned int qNum = 0; qNum < questionSet.size(); qNum++ )
      {
         tuple<string, float> qTuple = questionSet[qNum];

         string question = get<0>( qTuple );

         if ( question != "DontKnow" and question != "Other" )
         {
            string input = getFuzzyInputForRule( qNum, relevantQuestions );

            float weight;
            if ( input == "top" )
            {
               weight = ( float ) 1.0;
            }
            else if ( input == "medium" )
            {
               weight = ( float ) 0.50;
            }
            else
            {
               weight = ( float )0.10;
            }

            int parties = Other;
            if ( bUKIPGreen )
            {
               parties = NumOfParties;
            }

            for ( int iP = 0; iP < parties; iP++ )
            {
               string party = getOutputVariable( iP )->getName();

               vector<float> partyHist = constitHist->voteshare[iP];

               if ( partyHist.size() > year )
               {
                  float change = 0;
                  change = partyHist[year] - partyHist[year - 1];

                  string output = getFuzzyOutput( change );

                  // combine the above and make the rule
                  string rule = "if " + question + " is " + input
                     + " then " + party + " is " + output;

                  Rule* newRule = Rule::parse( rule, this );
                  newRule->setWeight( weight );
                  newRuleBlock->addRule( newRule );

                  totalRulesCount++;
               }
            }
         }
         else
         {
            //cout << "EXCLUDED" << endl;
         }
      }
      if ( newRuleBlock->numberOfRules() > 0 )
      {
         this->addRuleBlock( newRuleBlock );

         rulesAdded = true;
      }
   }

   return rulesAdded;
}

bool PredictorEngine::makePrediction( vector<tuple<string, float>> recentQuestions )
{
   string status;
   if ( not this->isReady( &status ) )
   {
      throw Exception( "[engine error] engine is not ready:n" + status, FL_AT );
      return false;
   }
   else
   {
      int q = 0;
      for each ( tuple<string, float> question in recentQuestions )
      {
         string qString = get<0>( question );
         InputVariable* input = this->getInputVariable( qString );
         string istring = getFuzzyInputForRule( q, recentQuestions.size() );
         if ( istring == "top" )
         {
            input->setValue( 10 );
         }
         else if ( istring == "medium" )
         {
            input->setValue( 5 );
         }
         else
         {
            input->setValue( 0 );
         }
         q++;
      }
   }

   if ( not this->isReady( &status ) )
   {
      throw Exception( "[engine error] engine is not ready:n" + status, FL_AT );
      return false;
   }
   else
   {
      this->process();

      vector<float> predictions;
      vector<float> swings;
      float total = 0;
      vector<float> lastResults;

      // Get predicted swings
      for ( size_t i = 0; i < NumOfParties; i++ )
      {
         OutputVariable* output = this->getOutputVariable( saPartyNames[i] );
         float val = (float)output->getValue();
         swings.push_back( val );
      }

      // Calculate new vote shares

      for ( size_t pred = 0; pred < swings.size(); pred++ )
      {
         float prediction = thisConstitHist->voteshare[pred].back()
            + swings[pred];
         //prediction = prediction / ( thisConstitHist->voteshare[pred].back() / 2 );
         predictions.push_back( prediction );
         lastResults.push_back( thisConstitHist->voteshare[pred].back() );
      }

      //auto largestfirst = max_element( begin( predictions ), end( predictions ) );
      //if ( distance( begin( predictions ), largestfirst ) == UKIP )
      //{
      //   predictions.erase( largestfirst );
      //}

      for each ( float pred in predictions )
      {
         total += pred;
      }

      for each ( float pred in predictions )
      {
         pred = ( pred / total * 100 );
      }

      predicted = static_cast<Parties>( 0 );
      previous = static_cast<Parties>( 0 );

      auto largest = max_element( begin( predictions ), end( predictions ) );
      predicted = static_cast<Parties>( distance( begin( predictions ), largest ) );

      swing = swings[predicted];

      largest = max_element( begin( lastResults ), end( lastResults ) );
      previous = static_cast<Parties>( distance( begin( lastResults ), largest ) );

      return true;
   }
}

bool PredictorEngine::getPrediction( Parties* holder, Parties* pred, float* swi )
{
   *holder = previous;
   *pred = predicted;
   *swi = swing;

   return true;
}

string PredictorEngine::getFuzzyInputForRule( int question, int relevantQuestions )
{
   float percentile = 0.0;
   percentile = ( (float)question + 1 ) / (float)relevantQuestions * 100;

   if ( percentile <= 25 )
   {
      return "top";
   }
   else if ( percentile <= 75 )
   {
      return "medium";
   }
   else
   {
      return "bottom";
   }
}

string PredictorEngine::getFuzzyOutput( float value )
{
   if ( value < HugeLoss )
   {
      return "massiveLoss";
   }
   else if ( value < BigLoss )
   {
      return "hugeLoss";
   }
   else if ( value < MajorLoss )
   {
      return "bigLoss";
   }
   else if ( value < MinorLoss )
   {
      return "majorLoss";
   }
   else if ( value < TinyLoss )
   {
      return "minorLoss";
   }
   else if ( value < NoChange )
   {
      return "tinyLoss";
   }
   else if ( value == 0.0 )
   {
      return "noChange";
   }
   else if ( value <= TinyGain )
   {
      return "tinyGain";
   }
   else if ( value <= MinorGain )
   {
      return "minorGain";
   }
   else if ( value <= MajorGain )
   {
      return "majorGain";
   }
   else if ( value <= BigGain )
   {
      return "bigGain";
   }
   else if ( value <= HugeGain )
   {
      return "hugeGain";
   }
   else
   {
      return "massiveGain";
   }
}