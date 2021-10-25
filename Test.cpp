#include "Test.h"
#include <iostream>

using namespace std;


void testDelta() {
    theOptionClass theOption(0.4, 105.0, 100, 0.025, 0.24, "C");
    double theDelta = theOption.Delta();

    std::cout << theOption.toString() << ") has a delta of " << to_string(theDelta) << std::endl;
}

void testImpliedVolatility() {
    theOptionClass theOption(0.4, 105.0, 100, 0.025, 0.24, "C");

    double marketPrice = theOption.Black_Scholes_Option_Price();

    double theCalculatedImpliedVol = theOption.Implied_Volatility(marketPrice, 1e-20, 1.0, 1e-10);

    theOptionClass theNewOption(0.4, 105.0, 100, 0.025, theCalculatedImpliedVol, "C");

    cout << theOption.toString() << ") with a BLS price of " << marketPrice << " has an implied volatility of " << to_string(theCalculatedImpliedVol) << std::endl;

}