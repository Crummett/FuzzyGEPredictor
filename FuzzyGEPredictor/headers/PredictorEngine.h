#pragma once
#include "FuzzyGEPredictor.h"

struct ConstituencyHistory;

class PredictorEngine :
   public Engine
{
public:
   PredictorEngine( vector<int> vElectionDates,
      vector<bool> vbUKIPGreen,
      vector<PollSet> unrankedQuestions,
      vector<vector<tuple<string, float>>> rankedquestions,
      string constitName,
      ConstituencyHistory* constitHist );

   ~PredictorEngine();

   bool generateInputs( vector<PollSet> vPollSet );
   bool makePrediction( vector<tuple<string, float>> recentQuestions );

   bool getPrediction( Parties * holder, Parties * pred, float * swi );

   int totalRulesCount;

private:
   string sName;
   Parties previous;
   Parties predicted;
   float swing;
   ConstituencyHistory* thisConstitHist;
   void generateOutputs();
   bool generateRuleBlock( vector<int> vElectionDates, vector<bool> vbUKIPGreen, vector<vector<tuple<string, float>>> rankedquestions, ConstituencyHistory* constitHist );
   string getFuzzyInputForRule( int question, int relevantQuestions );
   string getFuzzyOutput( float value );
};
