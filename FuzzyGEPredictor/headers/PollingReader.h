#pragma once
#include "FuzzyGEPredictor.h"

struct Poll
{
   int iPollDate;

   vector<float> fvPercentages;
};

struct PollSet
{
   vector<string> svQuestions;

   vector<Poll> pvPolls;
};

class PollingReader
{
public:
   PollingReader();
   ~PollingReader();

   bool fetchPolls( vector<PollSet> * vPollSet );

private:
   Month convertMonth( string month );
   int convertYear( string year );
   float convertPercentString( string percent );
};


