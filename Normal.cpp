#include "Normal.h"
#include <cmath>
#include <iostream>

using namespace std;

double NormalCDF(double x)
{
    double a0 = 0.2316419, a1 = 0.319381530, a2 = 0.356563782, a3 = 1.781477937, a4 = 1.821255978, a5 = 1.330274429;

    double xp = x >= 0 ? x : -x;

    double z = 1 / (1 + a0 * xp);
    double R = a1 * z - a2 * pow(z, 2) + a3 * pow(z, 3) - a4 * pow(z, 4) + a5 * pow(z, 5);

    double commonN = 1 / sqrt(2 * M_PI) * exp(-pow(xp, 2) / 2) * R;

    return x >= 0 ? 1 - commonN : commonN;
}

//int main()
//{
//    cout << NormalCDF(-1.63) << endl;
//}