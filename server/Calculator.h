#ifndef CPSC5042CLIENTSERVER_CALCULATOR_H
#define CPSC5042CLIENTSERVER_CALCULATOR_H

#include <cstring>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm> // for sort function
#include <stdexcept>
#include <regex>


using namespace std;

class Calculator {
public:

    /**
     * Calculates an algebraic expression
     * Valid operators: '+', '-', '*', '/', '^'
     * @param inExpr Expression to be evaluated
     * @return Result of the algebraic expression
     */
    string calculateExpression(string inExpr);

    /**
     * Converts numeric value to a different base
     * @param choice
     *          0- Binary to Decimal
     *          1- Decimal to Binary
     *          2- Binary to Hexadecimal
     *          3- Hexadecimal to Binary
     *          4- Decimal to Hexadecimal
     *          5- Hexadecimal to decimal
     *
     * @param inValue Value to be converted
     * @return Result of conversion
     */
    string convertor(const string &choice, string inValue);

    /**
     * Calculate Statistical data on a vector of numbers
     */
    string summary(const string &inValue);

private:
    /**
     * Parses the input expression using operands or spaces as delimiters,
     * and inserts the output into a vector
     * @param inExpression Input algebraic expression
     * @return A vector containing the operands and operators in order entered
     */
    vector<string> expTokenize(string &inExpression);

    /**
     * Converts order of a vector of operands and operators into RPN order
     * @param expTokens Vector containing the original expression tokens
     * @return A vector containing the operands and operators in RPN ordering
     */
    vector<string> convertToRPN(vector<string> &expTokens);


    /**
     * Calculates an expression in RPN notation. Expression passed in as a
     * vector of strings containing the operations and operators in RPN notation
     * @param rpnStack Vector containing the expression in RPN notation
     * @return The result of the calculation
     */
    double calculateRPN(vector<string> &rpnStack);

    /**
     * Helper function to validate input strings
     * @param inExpression String to be validated
     * @param validChars Set of valid char values (e.g. {'1', '2', '*', '.'})
     * @return True if all string characters are found in set, otherwise false.
     */
    bool validateInputString(const string &inExpression, set<char> validChars);

    //*************************************
    // Conversion functions
    //*************************************

    /**
     * Helper function to convert binary to hex
     * @param input Binary number
     * @return Hex representation of the binary number
     */
    string binToHex(string &input);

    /**
     * Helper function to convert hex to binary
     * @param input Hexadecimal number
     * @return Binary representation of the hex number
     */
    string hexToBin(string &input);

    /**
     * Helper function to convert decimal to bin
     * @param input Decimal number
     * @return Binary representation of the decimal number
     */
    string decToBin(string &input);

    /**
     * Helper function to convert binary to decimal
     * @param input Binary number
     * @return Decimal representation of the binary number
     */
    string binToDec(string &input);

    /**
     * Helper function to convert hex to decimal
     * @param input Hexadecimal number
     * @return Decimal representation of the hex number
     */
    string hexToDec(string &input);

    /**
     * Helper function to convert decimal to hex
     * @param input Decimal number
     * @return Hex representation of the decimal number
     */
    string decToHex(string &input);

    //*************************************
    // Stats functions
    //*************************************
    // Calculate the mean of a set of numbers
    float mean(vector<float>);

    // Calculate the median of a set of numbers
    float median(vector<float>);

    // Calculate the sample variance (meaning it uses n-1 in the denominator)
    float var(const vector<float> &vec);

    // calculate the sample standard deviation (meaning it uses n-1 in the denomicator).
    float sd(vector<float>);

    // Caclulate quantiles
    vector<float> quantiles(vector<float> data, float quantCuts);

    // Calculate the value representing the nth percentile (where 0 <= n <= 1)
    float percentile(vector<float> vec, float nth);


    //Unordered map containing the supported operators for calculateRPN
    // function, and their precedence order
    unordered_map<string, int> precedenceMap = {
            {"^", 3},
            {"*", 2},
            {"/", 2},
            {"+", 1},
            {"-", 1},
            {"(", -1},
            {")", -1},
    };

    //Exception Strings
    const string INVALID_EXPRESSION = "Invalid expression entered.",
                 INVALID_OPERATOR = "Invalid operator entered.",
                 INVALID_ARG = "Invalid argument entered.";

    //Sets containing valid char values for different numeric basis
    const set<char> BIN_CHAR = {'0', '1'};
    const set<char> DEC_CHAR = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                '9'};
    const set<char> FLOAT_CHAR = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                '9', '.', ' ', ',', '-'};

    const set<char> HEX_CHAR = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    const set<char> EXP_CHAR = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                                '9', ',', '.', '+', '-', '*', '/', '^', '(',
                                ')', ' '};
};

//#include "Calculator.cpp"
#endif //CPSC5042CLIENTSERVER_CALCULATOR_H
