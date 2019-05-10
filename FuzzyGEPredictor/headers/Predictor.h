#pragma once
#include "FuzzyGEPredictor.h"

class ConstituencyRegion;
class PollingReader;

struct PollSet;
struct Poll;

class Predictor
{
public:
   Predictor();
   ~Predictor();

   void Predict();

   bool AggregatePolling( int numMonths, vector<int> vElectionDates, vector<vector<tuple<string, float>>>* rankedquestions );

   int totalRulesCount;

private:
   vector<ConstituencyRegion *> vConstitRegions;
   vector<PollSet> vPollSet;
   PollingReader* PollReader;

   bool mostRecentPolling( int pollingMonths, vector<tuple<string, float>>* recentQuestions );
   string intToDateString( int date );
};