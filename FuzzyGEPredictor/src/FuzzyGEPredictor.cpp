// FuzzyGEPredictor.cpp : Defines the entry point for the application.
//

#include "FuzzyGEPredictor.h"

int main()
{
   Constituency * Example = new Constituency;

   cout << "fetching...";
   Example->fetchPastResults();
   cout << "\nfetched";
}
