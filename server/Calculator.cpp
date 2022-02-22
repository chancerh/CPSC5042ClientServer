#include "Calculator.h"
#include <regex>

using namespace std;

string Calculator::calculateExpression(const string &inExpr)
{
    return "";
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

