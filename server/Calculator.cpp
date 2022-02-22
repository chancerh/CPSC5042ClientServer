#include "Calculator.h"
#include <regex>

using namespace std;

string Calculator::calculateExpression(string inExpr)
{
   //Parse input expression into tokens using spaces and operators as delimiters
   vector<string> expTokens = expTokenize(inExpr);

   //Convert the tokens into RPN notation
   expTokens = convertToRPN(expTokens);

   //Calculate and return the expression
   return to_string(calculateRPN(expTokens));
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

string Calculator::binToHex(string& s) {
    s = string(4-s.size() % 4, '0') + s;
    string tmp, bits;
    vector<string> container;

    for(int i = 0; i < container.size()-3; i+=4) {
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
        } else {
            tmp += 'F';
        }
    }
    // Removes leading zeroes, output '0' if all zeroes
    return regex_replace(tmp, regex("^0+(?!$)"), "");
}

string Calculator::hexToBin(string& s) {
    string tmp;
    vector<unsigned long> hexContainer;
    for(int i = 0; i < s.size(); i++) {
        switch(toupper(hexContainer[i])) {
            case '0': tmp += "0000";
            case '1': tmp += "0001";
            case '2': tmp += "0010";
            case '3': tmp += "0011";
            case '4': tmp += "0100";
            case '5': tmp += "0101";
            case '6': tmp += "0110";
            case '7': tmp += "0111";
            case '8': tmp += "1000";
            case '9': tmp += "1001";
            case 'A': tmp += "1010";
            case 'B': tmp += "1011";
            case 'C': tmp += "1100";
            case 'D': tmp += "1101";
            case 'E': tmp += "1110";
            default: tmp += "1111";
        }
    }
    return tmp;
}

