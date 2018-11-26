// FuzzyGEPredictor.cpp : Defines the entry point for the application.

#include "FuzzyGEPredictor.h"

int main()
{
   ConstituencyArea * Example = new ConstituencyArea( 1 );

   cout << "fetching...";
   if ( Example->fetchResults() )
   {
      cout << "\nfetched";
   }

}
