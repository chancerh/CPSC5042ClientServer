#ifndef CPSC5042CLIENTSERVER_CALCULATOR_H
#define CPSC5042CLIENTSERVER_CALCULATOR_H

#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

class Calculator
{
public:
    string calculateExpression(string inExpr);

private:
    /**
     * Parses the input expression using operands or spaces as delimiters,
     * and inserts the output into a vector
     * @param inExpression Input algebraic expression
     * @return A vector containing the operands and operators in order entered
     */
    vector<string> expTokenize(string& inExpression);

    /**
     * Converts order of a vector of operands and operators into RPN order
     * @param expTokens Vector containing the original expression tokens
     * @return A vector containing the operands and operators in RPN ordering
     */
    vector<string> convertToRPN(vector<string>& expTokens);

    /**
     * Calculates an expression in RPN notation. Expression passed in as a
     * vector of strings containing the operations and operators in RPN notation
     * @param rpnStack Vector containing the expression in RPN notation
     * @return The result of the calculation
     */
    double calculateRPN(vector<string>& rpnStack);

    //Unordered map containing the supported operators for calculateRPN
    // function, and their precedence order
    unordered_map<string, int> precedenceMap ={
            { "^", 3},
            { "*", 2},
            { "/", 2},
            { "+", 1},
            { "-", 1},
            { "(", -1},
            { ")", -1},
    };

};

//#include "Calculator.cpp"
#endif //CPSC5042CLIENTSERVER_CALCULATOR_H
