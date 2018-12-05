// FuzzyGEPredictor.cpp : Defines the entry point for the application.

#include "FuzzyGEPredictor.h"

int main()
{
   ConstituencyArea * Example = new ConstituencyArea( SouthEast );

   cout << "fetching constituencies...";
   if ( Example->fetchResults() )
   {
      cout << "\nfetched";
   }

   PollingReader * PollExample = new PollingReader();

   cout << "\nfetching polls...";
   if ( PollExample->fetchPolls() )
   {
      cout << "\nfetched";
   }

}
