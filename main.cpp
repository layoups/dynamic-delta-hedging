#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

#include "theOptionClass.h"
#include "Test.h"

#include <ql/quantlib.hpp>
#include <ql/time/date.hpp>
#include <ql/time/calendar.hpp>
#include <boost/random.hpp>

#include "matplotlibcpp.h"

using namespace std;
using namespace QuantLib;
namespace plt = matplotlibcpp;

int main()
{

    std::cout << "ISyE6767 - Interim Project - Karim Layoun" << endl << endl;
    std::cout << "Part I" << endl;

    boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
            normRVGen(boost::mt19937(123), boost::normal_distribution<>());

    std::vector<int> periods(100);
    std::iota(periods.begin(), periods.end(), 0);

    // European Call Parameters
    double S = 100.0;
    double r = 0.025;
    double vol = 0.24;
    double T = 0.4;
    double u = 0.05;
    double N = 100.0;
    double delta_T = (double) (T / N);
    double K = 105;
    double dt = (double) 1.0/252.0;

    double thePricePaths[100][1000];
    vector<double> Z(100, 0);
    vector<double> theX;
    vector<double> theY;


    for (int j = 0; j < 1000; ++j) {
        for (int k = 0; k < 100; ++k) {
            Z[k] = normRVGen();
        }
        for (int i = 0; i < 100; ++i) {
            if (i == 0) {
                thePricePaths[i][j] = S;
            } else {
                thePricePaths[i][j] = thePricePaths[i-1][j] + u * thePricePaths[i-1][j] * delta_T + vol * thePricePaths[i-1][j] * sqrt(delta_T) * Z[i];
            }
        }
    }

    for (int i = 0; i < 100; i++) {
        theX.push_back(i * delta_T);
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            theY.push_back(thePricePaths[j][i]);
        }
        plt::plot(theX, theY);
        theY.clear();

    }

    theX.clear();
    plt::save("out/thePricePaths");
    plt::clf();

    std::cout << "price series generated" << endl;
    std::cout << "price series plot exported to out/thePricePaths" << endl;


    double theCallPricesBS[100][1000];
    double theCallDeltasBS[100][1000];

    // swap option and option price functions, change vars
    for (int j = 0; j < 1000; ++j) {
        for (int i = 0; i < 100; ++i) {

            // create option, get option price, get option delta
            theOptionClass theOption(T - (i + 1) * delta_T, K, thePricePaths[i][j], r, vol, "C");

            theCallPricesBS[i][j] = theOption.Black_Scholes_Option_Price();
            theCallDeltasBS[i][j] = theOption.Delta();
        }
    }

    for (int i = 0; i < 100; i++) {
        theX.push_back(i * delta_T);
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            theY.push_back(theCallPricesBS[j][i]);
        }
        plt::plot(theX, theY);
        theY.clear();

    }

    theX.clear();
    plt::save("out/theCallPaths");
    plt::clf();

    std::cout << "call price and call delta series generated" << endl;
    std::cout << "call price series plot exported to out/theCallPaths" << endl;

    double theBs[100][1000];

    for (int j = 0; j < 1000; ++j) {
        for (int i = 0; i < 100; ++i) {
            if (i == 0) {
                theBs[i][j] = theCallPricesBS[i][j] - theCallDeltasBS[i][j] * thePricePaths[i][j];
            } else {
                theBs[i][j] = theCallDeltasBS[i-1][j] * thePricePaths[i][j] + theBs[i-1][j] * exp(r*delta_T) -
                        theCallDeltasBS[i][j] * thePricePaths[i][j]; 
            }
        }
    }

    std::cout << "B series generated" << endl;


    double theHedgingError[100][1000];

    for (int j = 0; j < 1000; j++) {
        for (int i = 0; i < 100; i++) {
            if (i == 0) {
                theHedgingError[i][j] = 0;
            } else {
                theHedgingError[i][j] = theCallDeltasBS[i-1][j] * thePricePaths[i][j] + theBs[i-1][j] * exp(r * delta_T) - theCallPricesBS[i][j]; 
            }
        }
        theX.push_back(theHedgingError[99][j]);
    }

    plt::hist(theX, 50);
    plt::save("out/CumHedgingDistribution");
    plt::clf();

    theX.clear();

    std::cout << "cumulative hedging error series generated" << endl;
    std::cout << "cumulative hedging error distribution plot exported to out/CumHedgingDistribution" << endl;

    // Save all Files
    std::cout << "exporting the price series, the call price series, the call delta series, the B series, and the cumulative hedging error series to the folder out" << endl;

    std::ofstream thePricingPaths_OUT("./out/thePricingPaths.csv");
    for (auto& row : thePricePaths) {
        for (auto col : row)
            thePricingPaths_OUT << col << ',';
        thePricingPaths_OUT << '\n';
    }
    thePricingPaths_OUT.close();

    std::ofstream theCallPricesBS_OUT("./out/theCallPricesBS.csv");
    for (auto& row : theCallPricesBS) {
        for (auto col : row)
            theCallPricesBS_OUT << col << ',';
        theCallPricesBS_OUT << '\n';
    }
    theCallPricesBS_OUT.close();

    std::ofstream theCallDeltasBS_OUT("./out/theCallDeltasBS.csv");
    for (auto& row : theCallDeltasBS) {
        for (auto col : row)
            theCallDeltasBS_OUT << col << ',';
        theCallDeltasBS_OUT << '\n';
    }
    theCallDeltasBS_OUT.close();

    std::ofstream theBs_OUT("./out/theBs.csv");
    for (auto& row : theBs) {
        for (auto col : row)
            theBs_OUT << col << ',';
        theBs_OUT << '\n';
    }
    theBs_OUT.close();

    ofstream theHedgingError_OUT("./out/theHedgingError.csv");
    for (auto& row : theHedgingError) {
        for (auto col : row)
            theHedgingError_OUT << col << ',';
        theHedgingError_OUT << '\n';
    }
    theHedgingError_OUT.close();

    std::cout << "file export successful" << endl;
    std::cout << "End of Part I" << endl << endl;
    std::cout << "Part II" << endl;

    Calendar theCalendar = UnitedStates();
    string theUserStart_T;
    string theUserEnd_T;
    string theUser_T;
    string theUser_K;

//    UNITTEST
//    theUserStart_T = "2011-07-05";
//    theUserEnd_T = "2011-07-29";
//    theUser_T = "2011-09-17";
//    theUser_K = "500";


    std::cout << "please enter the required parameters:" << endl;
    std::cout << "the option series start date in yyyy-mm-dd format:" << endl;
    std::cin >> theUserStart_T;
    std::cout << "the option series end date in yyyy-mm-dd format:" << endl;
    std::cin >> theUserEnd_T;
    std::cout << "the option series maturity date in yyyy-mm-dd format:" << endl;
    std::cin >> theUser_T;

    if (theUserStart_T.length() != 10 || theUserEnd_T.length() != 10 || theUser_T.length() != 10) {
        cout << "please enter the dates in the yyyy-mm-dd format" << endl;
        exit(2);
    }

    Date theFormattedStartDate, theFormattedEndDate, theFormattedMaturityDate;
    theFormattedStartDate = DateParser::parseFormatted(theUserStart_T, "%Y-%m-%d");
    theFormattedEndDate = DateParser::parseFormatted(theUserEnd_T, "%Y-%m-%d");
    theFormattedMaturityDate = DateParser::parseFormatted(theUser_T, "%Y-%m-%d");
    
    double fromEndToStart = theCalendar.businessDaysBetween(theFormattedStartDate, theFormattedEndDate, true, true);
    double fromMaturityToStart = theCalendar.businessDaysBetween(theFormattedStartDate, theFormattedMaturityDate, true, true);
    if (fromEndToStart <= 0 || fromMaturityToStart <= 0) {
        cout << "ERROR: the start date is after the end or maturity date" << endl;
        std::exit(2);
    }

    std::cout << "the option series strike price:" << endl;
    std::cin >> theUser_K;


    vector<double> theInterestRates;
    vector<string> theInterestRateDates;
    string theInterestRateDatesStr;
    string theInterestRateStr;
    bool theImportBool = false;
    bool theEndBool = false;

    ifstream theInterestFile("./data/interest.csv");
    if (theInterestFile.fail()) {
        cout << "ERROR: the interest data file is not in the data folder" << endl;
        exit(1);
    }

    while (!theInterestFile.eof() & !theEndBool) {
        getline(theInterestFile, theInterestRateDatesStr, ',');
        getline(theInterestFile, theInterestRateStr, '\n');

        if (theInterestRateDatesStr == theUserStart_T) {
            theImportBool = true;
        }

        if (theImportBool) {
            theInterestRateDates.push_back(theInterestRateDatesStr);
            theInterestRates.push_back((stod(theInterestRateStr)/100.0));
        }

        theEndBool = theInterestRateDatesStr == theUserEnd_T ? true : false;
    }
    theInterestFile.close();
    cout << "relevant interest data imported" << endl;


    vector<double> theClosePrice;
    vector<string> theCloseDate;
    string theCloseDateStr;
    string theClosePriceStr;
    theImportBool = false;
    theEndBool = false;

    ifstream theCloseDataGOOG("./data/sec_GOOG.csv");
    if (theCloseDataGOOG.fail()) {
        cout << "ERROR: the sec_GOOG data file is not in the data folder" << endl;
        exit(1);
    }

    while (!theCloseDataGOOG.eof() & !theEndBool) {
        getline(theCloseDataGOOG, theCloseDateStr, ',');
        getline(theCloseDataGOOG, theClosePriceStr, '\n');

        if (theCloseDateStr == theUserStart_T) {
            theImportBool = true;
        }
        if (theImportBool) {
            theCloseDate.push_back(theCloseDateStr);
            theClosePrice.push_back(stod(theClosePriceStr));
        }

        theEndBool = theCloseDateStr == theUserEnd_T ? true : false;
    }
    theCloseDataGOOG.close();
    cout << "relevant GOOG closing price data imported" << endl;


    vector<string> theOptionDate;
    vector<string> theOptionMaturity;
    vector<string> theOptionType;
    vector<double> theOptionK;
    vector<double> theOptionBid;
    vector<double> theOptionOffer;
    vector<double> theOptionPrice;

    string theOptionDateStr;
    string theOptionMaturityStr;
    string theOptionTypeStr;
    string theOptionKStr;
    string theOptionBidStr;
    string theOptionOfferStr;
    theImportBool = false;
    theEndBool = false;

    ifstream theOptionDataGOOG("./data/op_GOOG.csv");
    if (theOptionDataGOOG.fail()) {
        cout << "ERROR: the op_GOOG data file is not in the data folder" << endl;
        exit(1);
    } 

    while (!theOptionDataGOOG.eof() & !theEndBool) {
        getline(theOptionDataGOOG, theOptionDateStr, ',');
        getline(theOptionDataGOOG, theOptionMaturityStr, ',');
        getline(theOptionDataGOOG, theOptionTypeStr, ',');
        getline(theOptionDataGOOG, theOptionKStr, ',');
        getline(theOptionDataGOOG, theOptionBidStr, ',');
        getline(theOptionDataGOOG, theOptionOfferStr, '\n');

        if (theOptionDateStr == theUserStart_T) {
            theImportBool = true;
        }
        if (theImportBool) {
            if (theOptionMaturityStr == theUser_T) {
                if (theOptionKStr == theUser_K) {
                    if (theOptionTypeStr == "C") {

                        theOptionDate.push_back(theOptionDateStr);
                        theOptionMaturity.push_back(theOptionMaturityStr);

                        theOptionType.push_back(theOptionTypeStr);
                        theOptionK.push_back(stod(theOptionKStr));

                        theOptionBid.push_back(stod(theOptionBidStr));
                        theOptionOffer.push_back(stod(theOptionOfferStr));
                        
                        theOptionPrice.push_back((stod(theOptionBidStr) +
                                               stod(theOptionOfferStr)) / 2);
                        
                        theEndBool = theOptionDateStr == theUserEnd_T ? true : false;
                    }
                }
            }
        }
    }
    theOptionDataGOOG.close();
    cout << "relevant GOOG option data imported" << endl;

    vector<double> theImpliedVol;
    vector<double> theOptionDelta;
    vector<double> theOptionB;
    vector<double> theOptionHedgingError;
    vector<double> theHoldPNL;
    vector<double> theHedgingDiff;
    vector<double> theTime;

    double tolerance = 1e-25;
    double LB = 1e-20;
    double UB = 1.000000;

    for(int i = 0; i < theOptionPrice.size(); i++){
        Date theTempOptionDate, theTempMaturityDate;
        theTempOptionDate = DateParser::parseFormatted(theOptionDate[i], "%Y-%m-%d");
        theTempMaturityDate = DateParser::parseFormatted(theUser_T, "%Y-%m-%d");

        double TimeInterval = theCalendar.businessDaysBetween(theTempOptionDate, theTempMaturityDate, true, true);
        double timetoMaturity = TimeInterval * dt;

        theOptionClass initOption(timetoMaturity, theOptionK[i], theClosePrice[i], theInterestRates[i], 0.0, "c");
        double theCalculatedImpliedVol = initOption.Implied_Volatility(theOptionPrice[i], LB, UB, tolerance);

        theOptionClass theVolOption(timetoMaturity, theOptionK[i], theClosePrice[i], theInterestRates[i], theCalculatedImpliedVol, "c");
        double delta = theVolOption.Delta();

        theImpliedVol.push_back(theCalculatedImpliedVol);
        theOptionDelta.push_back(delta);

        theTime.push_back(i);

        if (i == 0) {
            theOptionB.push_back(theOptionPrice[i] - (theOptionDelta[i] * theClosePrice[i]));
            theOptionHedgingError.push_back(0.0);
            theHedgingDiff.push_back(theOptionHedgingError[0]);
        } else {
            theOptionB.push_back((theOptionDelta[i-1] * theClosePrice[i]) + (theOptionB[i-1] * exp(theInterestRates[i-1] * dt)) -
            (theOptionDelta[i] * theClosePrice[i]));

            theOptionHedgingError.push_back((theOptionDelta[i-1]*theClosePrice[i]) + (theOptionB[i-1]*exp(theInterestRates[i-1]*dt)) -
                               (theOptionPrice[i]));

            theHedgingDiff.push_back(theOptionHedgingError[i] - theOptionHedgingError[i-1]);
        }
        theHoldPNL.push_back(theOptionPrice[0] - theOptionPrice[i]);
    }

    plt::plot(theTime, theOptionHedgingError);
    plt::plot(theTime, theHoldPNL);
    plt::save("out/theHedgeAdvantage");
    plt::clf();

    ofstream Results_OUT("./out/results.csv");
    Results_OUT << "t0: " << theUserStart_T << "\n";
    Results_OUT << "tN: " << theUserEnd_T << "\n";
    Results_OUT << "T: " << theUser_T << "\n";
    Results_OUT << "K: " << theUser_K << "\n";


    Results_OUT << "date" << "," << "S" << "," << "V" << "," << "implied volatility" << "," << "delta" << "," << "hedging error" << "," << "PNL" << "," << "PNL with Hedge" << "\n";
    for (int i = 0; i < theOptionPrice.size(); ++i) {
        Results_OUT << theCloseDate[i] << "," << theClosePrice[i] << "," << theOptionPrice[i] << "," << theImpliedVol[i] << "," << theOptionDelta[i] << "," << theHedgingDiff[i] << "," << theHoldPNL[i] << "," << theOptionHedgingError[i] << "\n";
    }
    Results_OUT.close();
    std::cout << "delta hedging backtest complete" << endl;
    std::cout << "results exported to out/results.csv" <<endl;

    std::cout << "End of Part II" << endl << endl;

    std::cout << "find below the unit test cases for delta and implied volatility calculations" << endl;

    testDelta();
    testImpliedVolatility();

    std::cout << endl << "End of Interim Project" << endl;

    return 0;
}
