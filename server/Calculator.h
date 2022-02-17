#ifndef CPSC5042CLIENTSERVER_CALCULATOR_H
#define CPSC5042CLIENTSERVER_CALCULATOR_H

#include <string>

using namespace std;

class Calculator
{
public:
    string calculateExpression(const string& inExpr);
    string binToHex(string& s);
    string hexToBin(string& s);

private:

};


#endif //CPSC5042CLIENTSERVER_CALCULATOR_H
