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

   bool AggregatePolling( int numMonths, vector<int> vElectionDates, vector<vector<tuple<string, float>>> * rankedquestions );

private:
   vector<ConstituencyRegion> vConstituencies;
   vector<PollSet> vPollSet;
   PollingReader* PollReader;

   bool CalcConstituencyVoteChanges( ConstituencyRegion * constituency );
};