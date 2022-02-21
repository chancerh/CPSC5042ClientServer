#include "Calculator.h"

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
