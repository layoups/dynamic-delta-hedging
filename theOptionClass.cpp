#include "theOptionClass.h"
#include "Normal.h"
#include <cmath>
#include <iostream>
#include <string>

theOptionClass::theOptionClass(double Expiry_, double Strike_, double Spot_, double Rate_, double Vol_, string flag_) : Expiry(Expiry_), Strike(Strike_), Spot(Spot_), Rate(Rate_), Vol(Vol_), flag(flag_)
{}

void theOptionClass::init()
{
    this->Expiry = 1;
    this->Strike = 10;
    this->Spot = 5;
    this->Rate = 0.06;
    this->Vol = 0.3;
    this->flag = "C";
}

theOptionClass::theOptionClass()
{
    init();
}

double theOptionClass::Black_Scholes_Option_Price() const
{

    double S = GetSpot();
    double K = GetStrike();
    double r = GetRate();
    double T = GetExpiry();
    double sig = GetVol();
    string flag = GetFlag();

    double d1 = (log(S / K) + (r + 0.5 * pow(sig, 2)) * T) / (sig * pow(T, 0.5));
    double d2 = d1 - sig * sqrt(T);

    if (flag.compare("c") == 0 || flag.compare("C") == 0) 
    {
        return S * NormalCDF(d1) - K * exp(-r * T) * NormalCDF(d2);
    }
    else if (flag.compare("p") == 0 || flag.compare("P") == 0)
    {
        return -S * NormalCDF(-d1) + K * exp(-r * T) * NormalCDF(-d2);
    }
    return -1;
}

double theOptionClass::Delta() const
{
    double S = GetSpot();
    double K = GetStrike();
    double r = GetRate();
    double T = GetExpiry();
    double sig = GetVol();
    string flag = GetFlag();

    double d1 = (log(S / K) + (r + 0.5 * pow(sig, 2)) * T) / (sig * pow(T, 0.5));

    if (flag.compare("c") == 0 || flag.compare("C") == 0) 
    {
        return NormalCDF(d1);
    }
    else if (flag.compare("p") == 0 || flag.compare("P") == 0)
    {
        return NormalCDF(d1) - 1;
    }
    return -1;

}

double theOptionClass::Implied_Volatility(double MarketPrice, double LB, double UB, double eps) const 
{
    double S = GetSpot();
    double K = GetStrike();
    double r = GetRate();
    double T = GetExpiry();

    double ImpliedVol = (LB + UB) / 2.0;

    theOptionClass theOption(T, K, S, r, ImpliedVol, "C");

    double BS_Price = theOption.Black_Scholes_Option_Price();

    int stop = 0;

    do {
        LB = BS_Price < MarketPrice ? ImpliedVol : LB;
        UB = BS_Price > MarketPrice ? ImpliedVol : UB;
        ImpliedVol = (LB + UB) / 2.0;

        theOptionClass theNewOption(T, K, S, r, ImpliedVol, "C");
        BS_Price = theNewOption.Black_Scholes_Option_Price();

        stop++;

    } while (abs(MarketPrice - BS_Price) > eps & stop < 1000);

    return ImpliedVol;
}

double theOptionClass::GetExpiry() const
{
    return Expiry;
}

double theOptionClass::GetRate() const
{
    return Rate;
}

double theOptionClass::GetSpot() const
{
    return Spot;
}

double theOptionClass::GetStrike() const
{
    return Strike;
}

double theOptionClass::GetVol() const
{
    return Vol;
}

string theOptionClass::GetFlag() const
{
    return flag;
}

theOptionClass::~theOptionClass()
{

}

string theOptionClass::toString() const
{
    return GetFlag() + "(" + "T =" + to_string(GetExpiry()) + 
    ", K =" + to_string(GetStrike()) + ", S =" + 
    to_string(GetSpot()) + ", r =" + to_string(GetRate()) +
    ", vol =" + to_string(GetVol());
}

//int main()
//{
//    PayOffCall* thePayoff = new PayOffCall(4);
//    Option theOption(*thePayoff, 1, 1, 1, 1, 1);
//
//}