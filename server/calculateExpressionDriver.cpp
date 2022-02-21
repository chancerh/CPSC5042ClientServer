//
// Created by Aacer on 2/20/2022.
//

#include <string>
#include <iostream>
#include "Calculator.h"

using namespace std;

int main()
{
   string exp;
   cout << "Enter expression: ";
   getline(cin, exp);

   Calculator myCalc;

   string result = myCalc.calculateExpression(exp);

   cout << result << endl;

   return 0;

}