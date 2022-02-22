#ifndef CPSC5042CLIENTSERVER_CALCULATOR_H
#define CPSC5042CLIENTSERVER_CALCULATOR_H

#include <string>
#include <vector>

using namespace std;

class Calculator
{
public:
    string calculateExpression(const string& inExpr);

    // Calculate the mean of a set of numbers
    float mean(vector<float>);
    // Calculate the median of a set of numbers
    float median(vector<float>);
    // Calculate the sample variance (meaning it uses n-1 in the denominator)
    float var(const vector<float> &vec);
    // calculate the sample standard deviation (meaning it uses n-1 in the denomicator). 
    float sd(vector<float>);
    // calculate a 5 number + mean summary
    vector<float> summary(const vector<float> &vec);
    // Caclulate quantiles
    vector<float> quantiles(vector<float> data, float quantCuts);
    
};


#endif //CPSC5042CLIENTSERVER_CALCULATOR_H
