//Author  : Group#2
//Date    : 03/12/2022
//Version : 2.0
//Filename: Calculator.cpp

#include "Calculator.h"

using namespace std;

//Calculate algebraic expressions
string Calculator::calculateExpression(string inExpr)
{
    //Validate input expression (i.e. containing valid characters)
    if (validateInputString(inExpr, EXP_CHAR))
    {

        //Parse input expression into tokens using spaces and operators as delimiters
        vector<string> expTokens = expTokenize(inExpr);

        //Convert the tokens into RPN notation
        expTokens = convertToRPN(expTokens);

        //Calculate and return the expression
        return to_string(calculateRPN(expTokens));
    }
    else
    {
        //if input is not correct, throw an exception
        throw invalid_argument(INVALID_EXPRESSION);
    }
}

//Performs conversion between hexadecimal, binary and decimal
string Calculator::convertor(const string& choice, string inValue)
{

    //If input is blank, throw an exception
    if (choice.empty())
        throw invalid_argument(INVALID_ARG);

    //Declare a string to store results
    string result;

    //Capture the requested conversion format
    int c = stoi(choice) - 1;
    switch(c)
    {
        //Convert from Binary to Hex
        case 0:
            //Validate input characters
            if(validateInputString(inValue, BIN_CHAR))
            {
                //Perform conversion
                result = binToDec(inValue);
                break;
            }
            else
            {
                throw invalid_argument(INVALID_ARG);
            }
        case 1:
            //Validate input characters
            if(validateInputString(inValue, DEC_CHAR))
            {
                //Perform conversion
                result = decToBin(inValue);
                break;
            }
            else
            {
                throw invalid_argument(INVALID_ARG);
            }
        case 2:
            //Validate input characters
            if(validateInputString(inValue, BIN_CHAR))
            {
                //Perform conversion
                result = binToHex(inValue);
                break;
            }
            else
            {
                throw invalid_argument(INVALID_ARG);
            }
        case 3:
            //Validate input characters
            if (inValue.substr(0, 2) == "0x")
            {
                inValue = inValue.substr(2);
            }
            if(validateInputString(inValue, HEX_CHAR))
            {
                //Perform conversion
                result = hexToBin(inValue);
                break;
            }
            else
            {
                throw invalid_argument(INVALID_ARG);
            }
        case 4:
            //Validate input characters
            if(validateInputString(inValue, DEC_CHAR))
            {
                //Perform conversion
                result = decToHex(inValue);
                break;
            }
            else
            {
                throw invalid_argument(INVALID_ARG);
            }
        case 5:
            //Validate input characters
            if (inValue.substr(0, 2) == "0x")
            {
                inValue = inValue.substr(2);
            }
            if(validateInputString(inValue, HEX_CHAR))
            {
                //Perform conversion
                result = hexToDec(inValue);
                break;
            }
            else
            {
                throw invalid_argument(INVALID_ARG);
            }
        default:
            throw invalid_argument(INVALID_EXPRESSION);
    }

    //return result
    return result;
}

//Calculates the mean/average of a set of numbers
float Calculator::mean(const vector<float> &vec)
{
    // Check that vector is not empty
    if (vec.empty())
        throw invalid_argument("Dataset in vec must have size > 0.");

    float elementSum = 0.0; // this will store the sum of elements

    // Sum the elements of the vector
    for (float elm : vec)
        elementSum += elm;

    // calculate the mean and return
    float mean = elementSum / ((float) vec.size());

    return mean;

}


//Helper function to calculate percentiles for a set of numbers
float Calculator::percentile(vector<float> vec, float nth) {

    // Check that vector is not empty
    if (vec.empty())
        throw invalid_argument("Dataset in vec must have size > 0.");

    // Check to see if the vector is sorted - if not, sort it. 
    if (!is_sorted(vec.begin(), vec.end()))
        sort(vec.begin(), vec.end());


    // Get the index of the quantile.
    float index = (vec.size() - 1) * nth;

    // Get the low/high index for the nth percentile (use floor/cieling
    // for case when n is even
    int indexLow = floor(index);
    int indexHigh = ceil(index);

    float qs = vec[indexLow];
    float h = index - indexLow;
    return (1-h) * qs + h * vec[indexHigh];
}

//Calculates percentiles/quantiles
vector<float> Calculator::quantiles(vector<float> vec, float quantCut)
{
    float initQuantCut = quantCut;

    // Check that vector is not empty
    if (vec.empty())
        throw invalid_argument("Dataset in vec must have size > 0.");

    vector<float> quants; // this will hold the calculated quantiles

    // Sort the vector if it isn't already
    // Check to see if the vector is sorted - if not, sort it. 
    if (!is_sorted(vec.begin(), vec.end()))
        sort(vec.begin(), vec.end());

    // Repeatedly call percntile to calulate values at each quanitle cutpoint.
    while (quantCut < 1.0) {
        // Check to make sure that quantCut is between 0 and 1
        if ((quantCut > 1.0) | (quantCut < 0.0))
            throw invalid_argument("Quantile cuts must be between 0.0 and 1.0");

        // calculate percentile, add add resutl to quants vector
        quants.push_back(percentile(vec, quantCut));
        // Move to the next quantCut
        quantCut += initQuantCut;
    }

    return quants;
}

//Calculates the variance for a set of numbers
float Calculator::var(const vector<float> &vec)
{

    // Check that vector is not empty
    if (vec.size() < 2)
        throw invalid_argument("Dataset in vec must have size > 1.");

    // Meed the mean the calculation
    float vecMean = mean(vec);
    // This will hold the accumulated numerator calculations
    float variance = 0.0;
    // Use a for loop for the sums
    for (auto elm : vec)
    {
        variance += pow((elm - vecMean), 2);
    }

    // returned the unbiased variance
    return variance / (vec.size() - 1);
}


//Calculate the standard deviation for a set of numbers
float Calculator::sd(const vector<float> &vec)
{
    // Check that vector is not empty
    if (vec.size() < 2)
        throw invalid_argument("Dataset in vec must have size > 1.");

    // unbiased sd is just the square root of the unbiased variance
    return sqrt(var(vec));
}

//Calculates all stats for a set of numbers
string Calculator::summary(const string &inValue)
{
    vector<float> vec;

    // For now, assume someone typed in mean followed by numbers separated by
    // spaces make a whitespace regex
    auto const re = regex{R"(\s+)"};

    //Check that all the input characters are valid
    if (!validateInputString(inValue, FLOAT_CHAR))
        throw invalid_argument(INVALID_ARG);

    auto const vecString =
            vector<string>(sregex_token_iterator{begin(inValue),
                                                 end(inValue), re, -1},
                           sregex_token_iterator{});

    // put results into single string
    string resultString;

    for (string str : vecString)
    {
        try {
            vec.push_back(stof(str));
        }
        catch(const invalid_argument& is)
        {
            throw invalid_argument(INVALID_ARG);
        }
    }
    // Check that vector is not empty
    if (vec.empty())
        throw invalid_argument(INVALID_ARG);

    // This will hold the results of the calculations and be returned
    vector<float> summaryOut;

    //assign the calculated values to variables
    auto min = *min_element(vec.begin(), vec.end());
    auto max = *max_element(vec.begin(), vec.end());
    auto quartiles = quantiles(vec, .25);
    auto avg = mean(vec);

    //Setup the output of the calculated values
    summaryOut.push_back(min);
    summaryOut.push_back(quartiles[0]);
    summaryOut.push_back(quartiles[1]);
    summaryOut.push_back(avg);
    summaryOut.push_back(quartiles[2]);
    summaryOut.push_back(max);
    if (vec.size() > 1)
    {
        auto stdDev = sd(vec);
        auto variance = var(vec);
        summaryOut.push_back(stdDev);
        summaryOut.push_back(variance);
    }


    //convert output to string format
    resultString = to_string(summaryOut[0]) + ";" +
                   to_string(summaryOut[1]) + ";" +
                   to_string(summaryOut[2]) + ";" +
                   to_string(summaryOut[3]) + ";" +
                   to_string(summaryOut[4]) + ";" +
                   to_string(summaryOut[5]);

    //If more than 1 number exists in the set, get the sd and var
    if (vec.size() > 1)
    {
        resultString = resultString + + ";" + to_string(summaryOut[6]) + ";" +
                       to_string(summaryOut[7]);
    }
    return resultString;
}

//Helper function to calculate expressions (tokenizes input string)
vector<string> Calculator::expTokenize(string &inExpression)
{
    //Create a vector to store the tokens
    vector<string> tokens = {};

    string temp = ""; //temporary string for storage
    int bracketRefCount = 0;

    //Loop through input string and populate the vector
    for(char c : inExpression)
    {
        //If char is a space, then do nothing
        if( c == ' ')
            continue;

        //if char is an open bracket, flush temp and add multiply
        else if (c == '(')
        {
            bracketRefCount++;
            if (!temp.empty())
            {
                tokens.push_back(temp);
                temp.clear();
                tokens.push_back("*");
            }
            //If char before was a closing bracket, flush temp and add multiply
            else if (!tokens.empty() && tokens.back() == ")")
            {
                tokens.push_back("*");
            }

            tokens.push_back(string(1, c));
        }

        //if char is an operator and not a negative/minus sign
        else if (precedenceMap.find(string(1, c)) != precedenceMap.end() &&
                 c != '-')
        {
            //Throw an exception if we do have b2b operators (invalid expression)
            if(!tokens.empty() &&
               !isdigit(tokens.back()[tokens.back().length()- 1]) &&
               (tokens.back() != ")") &&
               temp.empty())
            {
                throw invalid_argument(INVALID_EXPRESSION);
            }

            //Throw an exception if the first char in expression is an operator
            else if (tokens.empty() && temp.empty())
            {
                throw invalid_argument(INVALID_EXPRESSION);
            }

            //if temp storage had a number in it
            if(!temp.empty())
                //add the number to the vector first
                tokens.push_back(temp);

            //reset temp storage
            temp.clear();

            //if closing bracket with no previous open brackets, throw exception
            if (c == ')' && bracketRefCount <= 0)
            {
                throw invalid_argument(INVALID_EXPRESSION);
            }
            else if (c == ')' && bracketRefCount > 0)
            {
                bracketRefCount --;
            }

            //add operator to the vector
            tokens.push_back(string(1, c));


        }

        //if char is a negative/minus sign
        else if (c == '-')
        {
            // if previous token is a number or a closing bracket or previous
            // character is a number, then treat as a minus sign
            if (!temp.empty() ||
                (!tokens.empty() &&
                 (isdigit((tokens.back()[tokens.back().length()-1])) ||
                  tokens.back()[tokens.back().length() - 1] == ')')))
            {
                //if temp storage has a number in it
                if(!temp.empty())
                {
                    //add the number to the vector first
                    tokens.push_back(temp);
                }
                //reset temp storage
                temp.clear();

                //add operator to the vector
                tokens.push_back(string(1, c));
            }

            //else treat as a negative sign
            else
            {
                temp += c;
            }
        }

            //else if the char is a number, add to the temp storage (until a
            // delimiter is reach)
        else
            temp += c;
    }

    //add the last number to the stack
    if(!temp.empty())
    {
        //handle case of a closing bracket before the last number
        if (!tokens.empty() && tokens.back() == ")")
            tokens.push_back("*");
        tokens.push_back(temp);
    }

    //Add closing brackets for any open brackets
    for (; bracketRefCount > 0; bracketRefCount--)
    {
        tokens.push_back(string(1, ')'));
    }

    //return the tokens vector
    return tokens;
}

//Covert a string tokens to RPN notation
vector<string> Calculator::convertToRPN(vector<string>& expTokens)
{
    //Create a vector to store the result
    vector<string> rpnStack = {};

    //Create a temp storage for operations within parenthesis
    vector<string> temp = {"("};
    expTokens.push_back(")");
    for(string token : expTokens)
    {
        //if the token is a number, add it to the rpnStack
        if(precedenceMap.find(token) == precedenceMap.end()) // oparand
            rpnStack.push_back(token);

        //if token is an opening parenthesis, add it to the temp stack
        else if( token == "(")
            temp.push_back(token);

        //if token is a closing parenthesis, then process the temp stack and
        // add its contents to the rpn stack
        else if (token == ")")
        {
            //while the temp stack has tokens, push them in reverse order into
            // rpnStack
            while(!temp.empty() && temp.back() != "(")
            {
                rpnStack.push_back(temp.back());
                temp.pop_back();
            }

            if (!temp.empty())
                temp.pop_back();
        }

        //if token is an operator
        else
        {
            //while temp vector is not empty and the token has higher precedence
            // than this token, add the temp token to the stack
            while(!temp.empty() &&
                  precedenceMap[token] <= precedenceMap[temp.back()])
            {
                rpnStack.push_back(temp.back());
                temp.pop_back();
            }

            //add operator to the temp stack
            temp.push_back(token);
        }
    }

    //return the rpn stack
    return rpnStack;
}

//Calculate RPN notation
double Calculator::calculateRPN(vector<string>& rpnStack)
{
    //Create a vector to store the operands
    vector<double> operandStack = {};

    if (rpnStack.size() < 3)
    {
        throw invalid_argument(INVALID_EXPRESSION);
    }

    //Loop through each token and perform necessary calculations
    for(string token : rpnStack)
    {
        //if token is an operator, perform the calculation
        if(precedenceMap.find(token) != precedenceMap.end())
        {
            //if number of arguments is less than 2, then throw an exception
            if(operandStack.size() < 2)
            {
                throw invalid_argument(INVALID_EXPRESSION);
            }

            //Get operands for the operation
            double operand1 = operandStack[operandStack.size() - 1];
            double operand2 = operandStack[operandStack.size() - 2];

            //perform the calculation
            switch(token[0])
            {
                case '^':
                    operand2 = pow(operand2, operand1);
                    break;
                case '+':
                    operand2 = operand2 + operand1;
                    break;
                case '-':
                    operand2 = operand2 - operand1;
                    break;
                case '*':
                    operand2 = operand2 * operand1;
                    break;
                case '/':
                    operand2 = operand2 / operand1;
                    break;
                default:
                    throw invalid_argument(INVALID_OPERATOR);
            }

            //remove the operands from the stack, and add result to the stack
            operandStack.pop_back();
            operandStack.pop_back();
            operandStack.push_back(operand2);
        }

        //if token is a number, add to the operand stack
        else
        {
            operandStack.push_back(atof(token.c_str()));
        }
    }

    //return the result
    return operandStack[0];
}

//Convert Binary number to Hexadecimal
string Calculator::binToHex(string &input)
{
    //pad input
    while (input.size() % 4 != 0)
    {
        input = "0" + input;
    }
    string result, bits;
    vector<string> container;

    for(unsigned long i = 0; i < input.size()-3; i+=4) {
        bits = input.substr(i, 4);
        if(bits == "0000")
            result += '0';
        else if(bits == "0001")
            result += '1';
        else if(bits == "0010")
            result += '2';
        else if(bits == "0011")
            result += '3';
        else if(bits == "0100")
            result += '4';
        else if(bits == "0101")
            result += '5';
        else if(bits == "0110")
            result += '6';
        else if(bits == "0111")
            result += '7';
        else if(bits == "1000")
            result += '8';
        else if(bits == "1001")
            result += '9';
        else if(bits == "1010")
            result += 'A';
        else if(bits == "1011")
            result += 'B';
        else if(bits == "1100")
            result += 'C';
        else if(bits == "1101")
            result += 'D';
        else if(bits == "1110")
            result += 'E';
        else if(bits == "1111")
            result += 'F';
        else
            throw invalid_argument(INVALID_EXPRESSION);
    }

    // Removes leading zeroes, output '0' if all zeroes
    result = regex_replace(result, regex("^0+(?!$)"), "");
    return "0x" + result;
}

//Convert Hexadecimal number to Binary
string Calculator::hexToBin(string& input)
{
    //String to store results
    string result;

    //Convert each char to its binary representation
    for(unsigned long i = 0; i < input.size(); i++)
    {
        switch(toupper(input[i]))
        {
            case '0':
                result += "0000";
                break;
            case '1':
                result += "0001";
                break;
            case '2':
                result += "0010";
                break;
            case '3':
                result += "0011";
                break;
            case '4':
                result += "0100";
                break;
            case '5':
                result += "0101";
                break;
            case '6':
                result += "0110";
                break;
            case '7':
                result += "0111";
                break;
            case '8':
                result += "1000";
                break;
            case '9':
                result += "1001";
                break;
            case 'A':
                result += "1010";
                break;
            case 'B':
                result += "1011";
                break;
            case 'C':
                result += "1100";
                break;
            case 'D':
                result += "1101";
                break;
            case 'E':
                result += "1110";
                break;
            case 'F':
                result += "1111";
                break;
            default:
                throw invalid_argument(INVALID_EXPRESSION);
        }
    }

    return result;
}

//Convert Decimal number to Binary
string Calculator::decToBin(string& input)
{

    //convert string to int
    int num = stoi(input);
    vector<unsigned int> tempResult;
    stringstream temp;

    // storing remainder in tempResult
    while (num > 0) {
        tempResult.push_back(num % 2) ;
        num = num / 2;
    }

    //make a binary format ex. 0001
    while(tempResult.size()%4 != 0){
        tempResult.push_back(0);
    }

    //reverse order and convert
    while(!tempResult.empty()){
        temp<<tempResult.back();
        tempResult.pop_back();
    }
    return temp.str();
}

//Convert Binary number toDecimal
string Calculator::binToDec(string &input) {
    unsigned long result = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    //start the loop from the back
    for (int i = input.size() - 1; i >= 0; i--) {
        int tmp = stoi(to_string(input[i])) - '0';

        //calculate and store in temp
        result = result + tmp * base;
        base = base * 2;
    }

    return to_string(result);
}

//Convert Hexadecimal number to decimal
string Calculator::hexToDec(string &input)
{
    string result;
    //convert hex to bin
    result = hexToBin(input);
    //use the result to covert to dec
    result = binToDec(result);

    return result;
}

//Convert decimal number to Hexadecimal
string Calculator::decToHex(string &input)
{
    string result;
    //covert dec to bin
    result = decToBin(input);
    //use the result to convert to hex
    result = binToHex(result);

    return result;
}

//helper function to validate the string input
bool Calculator::validateInputString(const string &inExpression,
                                     set<char>validChars)
{
    //if input value is empty, return false
    if (validChars.empty())
        return false;

    //For each character in the string
    for (char c : inExpression)
    {
        //if character not in valid set, return false
        if (validChars.find(toupper(c)) == validChars.end())
            return false;
    }

    //if all characters passed test, return true
    return true;
}