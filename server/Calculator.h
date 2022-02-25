#ifndef CPSC5042CLIENTSERVER_CALCULATOR_H
#define CPSC5042CLIENTSERVER_CALCULATOR_H

#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <set>


using namespace std;

class Calculator
{
public:

    string calculateExpression(string inExpr);

    string binToHex(string& s);
    string hexToBin(string& s);
    string decToBin(string& s);
    string binToDec(string& s);



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

    /**
     * Helper function to validate input strings
     * @param inExpression String to be validated
     * @param validChars Set of valid char values (e.g. {'1', '2', '*', '.'})
     * @return True if all string characters are found in set, otherwise false.
     */
    bool validateInputString (string inExpression, set<char> validChars);

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
