#ifndef THEOPTIONCLASS_H
#define THEOPTIONCLASS_H

#include <string>

using namespace std;

class theOptionClass
{
public:

    theOptionClass(double Expiry_, double Strike_, double Spot_, double Rate_, double Vol_, string flag);
    theOptionClass();
    ~theOptionClass();

    double Black_Scholes_Option_Price() const;
    double Implied_Volatility(double, double, double, double) const;
    double Delta() const;

    double GetExpiry() const;
    double GetStrike() const;
    double GetSpot() const;
    double GetRate() const;
    double GetVol() const;
    string GetFlag() const;

    string toString() const;


private:

    double Expiry;
    double Strike;
    double Spot;
    double Rate;
    double Vol;
    string flag;

    void init();

};

#endif