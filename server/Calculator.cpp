#include "Calculator.h"
#include <algorithm> // for sort function
#include "math.h"
#include <stdexcept>
#include <regex>

using namespace std;

string Calculator::calculateExpression(string inExpr)
{
   //Validate input expression (i.e. containing valid characters)
   set<char> validChars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                           ',', '.', '+', '-', '*', '/', '^', '(', ')', ' '};
   if (validateInputString(inExpr, validChars))
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
      throw invalid_argument(INVALID_EXPRESSION);
   }
}

string Calculator::convertorMenu(string choice, string s){
    //Validate input
    set<char> binInput = {'0', '1'};
    set<char> decInput = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    set<char> hexInput = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                          'A', 'B', 'C', 'D', 'E', 'F'};
    string token;
    int c = stoi(choice);
    switch(c){
        case 1:
            if(validateInputString(s, binInput)){
                token = binToDec(s);
                break;
            }
            else
            {
                throw invalid_argument("Invalid input expression.");
            }
        case 2:
            if(validateInputString(s, hexInput)){
                token = decToBin(s);
                break;
            }
            else
            {
                throw invalid_argument("Invalid input expression.");
            }
        case 3:
            if(validateInputString(s, hexInput)){
                token = hexToDec(s);
                break;
            }
            else
            {
                throw invalid_argument("Invalid input expression.");
            }
        case 4:
            if(validateInputString(s, decInput)){
                token = decToBin(s);
                break;
            }
            else
            {
                throw invalid_argument("Invalid input expression.");
            }
        default:
            throw invalid_argument(INVALID_EXPRESSION);
    }
    return token;
}

float Calculator::mean(vector<float> vec) 
{
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");
  
    float elementSum = 0.0; // this will store the sum of elements

    // Sum the elements of the vector
    for (float elm : vec) 
        elementSum += elm;

    // calculate the mean and return
    float mean = elementSum / ((float) vec.size());

    return mean;

}

float Calculator::median(vector<float> vec)
{
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");   

    // Then median is the .5 quantile
    return quantiles(vec, .5)[0];
}


float Calculator::percentile(vector<float> vec, float nth) {

    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");

    // Check to see if the vector is sorted - if not, sort it. 
    if (!is_sorted(vec.begin(), vec.end()))
	sort(vec.begin(), vec.end());
	
    
    // Get the index of the quantile. There may be two if n is even, so 
    float index = (vec.size() - 1) * nth;

    // If n is odd, indexLow and indexHigh will be the same
    int indexLow = floor(index);
    int indexHigh = ceil(index);

    float qs = vec[indexLow];
    float h = index - indexLow;
    return (1-h) * qs + h * vec[indexHigh];
}


vector<float> Calculator::quantiles(vector<float> vec, float quantCut)
{
    float initQuantCut = quantCut;
    
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");

    vector<float> quants; // this will hold the calculated quantiles

    // Sort the vector if it isn't already
    // Check to see if the vector is sorted - if not, sort it. 
    if (!is_sorted(vec.begin(), vec.end()))
	sort(vec.begin(), vec.end());

    
    while (quantCut < 1.0) {
        if ((quantCut > 1.0) | (quantCut < 0.0))
            throw invalid_argument("Quantile cuts must be between 0.0 and 1.0");


	quants.push_back(percentile(vec, quantCut));

        // Either way, add the elements of indexLow and indexHigh together and
        // divide by 2.
        // float quant = (vec[indexHigh] + vec[indexLow]) / 2.0;
        // quants.push_back(quant);

        // Move to the next quantile
        quantCut += initQuantCut;
    }

    return quants;
}

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

float Calculator::sd(vector<float> vec)
{
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 1.");

    // unbiased sd is just the square root of the unbiased variance
    return sqrt(var(vec));
}

vector<float> Calculator::summary(const vector<float> &vec)
{
    // Check that vector is not empty
    if (vec.size() < 1)
        throw invalid_argument("Dataset in vec must have size > 0.");
    
    // This will hold the results of the calculations and be returned
    vector<float> summaryOut;
    
    auto min = *min_element(vec.begin(), vec.end());
    auto max = *max_element(vec.begin(), vec.end());
    auto quartiles = quantiles(vec, .25);
    auto avg = mean(vec);
     
    summaryOut.push_back(min);
    summaryOut.push_back(quartiles[0]);
    summaryOut.push_back(quartiles[1]);
    summaryOut.push_back(avg);
    summaryOut.push_back(quartiles[2]);
    summaryOut.push_back(max);


    return summaryOut;

}

vector<string> Calculator::expTokenize(string &inExpression)
{
   //Create a vector to store the tokens
   vector<string> tokens = {};

   string temp = ""; //temporary string for storage

   //Loop through input string and populate the vector
   for(char c : inExpression)
   {
      //If char is a space, then do nothing
      if( c == ' ')
         continue;

      //if char is an operator
      else if (precedenceMap.find(string(1, c)) != precedenceMap.end())
      {
         //if temp storage had a number in it
         if(temp != "")

            //add the number to the vector first
            tokens.push_back(temp);

         //reset temp storage
         temp = "";

         //add operator to the vector
         tokens.push_back(string(1, c));
      }

      //else if the char is a number, add to the temp storage (until a
      // delimiter is reach)
      else
         temp += c;
   }

   //add the last number to the stack
   if(temp != "")
      tokens.push_back(temp);

   //return the tokens vector
   return tokens;
}

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
         while(temp.back() != "(")
         {
            rpnStack.push_back(temp.back());
            temp.pop_back();
         }
         temp.pop_back();
      }

      //if token is an operator
      else
      {
         //while temp vector is not empty and the token has higher precedence
         // than this token, add the temp token to the stack
         while(temp.size() > 0 &&
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

double Calculator::calculateRPN(vector<string>& rpnStack)
{
   //Create a vector to store the operands
   vector<double> operandStack = {};

   //Loop through each token and perform necessary calculations
   for(string token : rpnStack)
   {
      //if token is an operator, perform the calculation
      if(precedenceMap.find(token) != precedenceMap.end())
      {
         //get the operands
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

string Calculator::binToHex(string &s) {
    s = string(4-s.size() % 4, '0') + s;
    string tmp, bits;
    vector<string> container;

    for(unsigned long i = 0; i < container.size()-3; i+=4) {
        bits = s.substr(i, 4);
        if(bits == "0000") {
            tmp += '0';
        } else if(bits == "0001") {
            tmp += '1';
        } else if(bits == "0010") {
            tmp += '2';
        } else if(bits == "0011") {
            tmp += '3';
        } else if(bits == "0100") {
            tmp += '4';
        } else if(bits == "0101") {
            tmp += '5';
        } else if(bits == "0110") {
            tmp += '6';
        } else if(bits == "0111") {
            tmp += '7';
        } else if(bits == "1000") {
            tmp += '8';
        } else if(bits == "1001") {
            tmp += '9';
        } else if(bits == "1010") {
            tmp += 'A';
        } else if(bits == "1011") {
            tmp += 'B';
        } else if(bits == "1100") {
            tmp += 'C';
        } else if(bits == "1101") {
            tmp += 'D';
        } else if(bits == "1110") {
            tmp += 'E';
        } else if(bits == "1111"){
            tmp += 'F';
        } else{
            throw invalid_argument(INVALID_EXPRESSION);
        }
    }
    // Removes leading zeroes, output '0' if all zeroes
    return regex_replace(tmp, regex("^0+(?!$)"), "");
}

string Calculator::hexToBin(string &s) {
    string tmp;
    vector<unsigned long> hexContainer;
    for(unsigned long i = 0; i < s.size(); i++) {
        switch(toupper(hexContainer[i])) {
            case '0':
                tmp += "0000";
                break;
            case '1':
                tmp += "0001";
                break;
            case '2':
                tmp += "0010";
                break;
            case '3':
                tmp += "0011";
                break;
            case '4':
                tmp += "0100";
                break;
            case '5':
                tmp += "0101";
                break;
            case '6':
                tmp += "0110";
                break;
            case '7':
                tmp += "0111";
                break;
            case '8':
                tmp += "1000";
                break;
            case '9':
                tmp += "1001";
                break;
            case 'A':
                tmp += "1010";
                break;
            case 'B':
                tmp += "1011";
                break;
            case 'C':
                tmp += "1100";
                break;
            case 'D':
                tmp += "1101";
                break;
            case 'E':
                tmp += "1110";
                break;
            case 'F':
                tmp += "1111";
                break;
            default: throw invalid_argument(INVALID_EXPRESSION);
        }
    }
    return tmp;
}
string Calculator::decToBin(string &s){


    //convert string to int
    int num = stoi(s);
    vector<unsigned int> tempResult;
    stringstream temp;

    // storing remainder in tempResult
    while (num > 0) {
        tempResult.push_back(num % 2) ;
        num = num / 2;
    }

    //reverse order and convert
    while(!tempResult.empty()){
        temp<<tempResult.back();
        tempResult.pop_back();
    }
    return temp.str();
}

string Calculator::binToDec(string &s){
    //convert string to int
    int num = stoi(s);
    int result = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    while (num) {
        int last_digit = num % 10;
        num = num / 10;

        result += last_digit * base;

        base = base * 2;
    }

    return to_string(result);
}


bool Calculator::validateInputString(string inExpression,
                                     set<char>validChars)
{
   //For each character in the string
    for (char c : inExpression)
    {
       //if character not in valid set, return false
        if (validChars.find(c) == validChars.end())
            return false;
    }

    //if all characters passed test, return true
    return true;
}

string Calculator::hexToDec(string s) {
    unsigned long len = s.size();

    // Initializing base value to 1, i.e 16^0
    int base = 1;

    int dec_val = 0;

    // Extracting characters as digits from last character
    for (unsigned long i=len-1; i>=0; i--)
    {
        // if character lies in '0'-'9', converting
        // it to integral 0-9 by subtracting 48 from
        // ASCII value.
        if (s[i]>='0' && s[i]<='9')
        {
            dec_val += (s[i] - 48)*base;

            // incrementing base by power
            base = base * 16;
        }

            // if character lies in 'A'-'F' , converting
            // it to integral 10 - 15 by subtracting 55
            // from ASCII value
        else if (s[i]>='A' && s[i]<='F')
        {
            dec_val += (s[i] - 55)*base;

            // incrementing base by power
            base = base*16;
        }
    }

    return to_string(dec_val);
}

//string Calculator::decToHex(string s) {
//}




