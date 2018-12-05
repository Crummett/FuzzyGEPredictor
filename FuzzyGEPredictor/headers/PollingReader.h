#pragma once
#include "FuzzyGEPredictor.h"

struct Poll
{
   int iYear;
   Month eMonth;

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

   bool fetchPolls();

private:
   Month convertMonth( string month );
   int convertYear( string year );
   float convertPercentString( string percent );

   vector<PollSet> vPollSet;
};


