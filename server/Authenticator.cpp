//Author  : Group#2
//Date    : 03/12/2022
//Version : 2.0
//Filename: Authenticator.cpp

#include "Authenticator.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


//Constructor
Authenticator::Authenticator(const string &fileName)
{
    // populate username and passwords with data from a file
    readFile(fileName);
}

//Destructor
Authenticator::~Authenticator() = default;

//Authenticates user against credentials parsed from file
bool Authenticator::authenticate(const string &username, const string &password)
{
    // First check if the user exists
    unordered_map<string, string>::const_iterator mapIterator =
            m_users.find(username);

    // If the user doesn't exist, return false
    if (mapIterator == m_users.end())
        return false;

    // Check that the password matches the record for this username
    if (m_users[username] == password)
        return true;

    // Otherwise, return false
    return false;
}

//Reads a file containing credentials and populating contents into a map
void Authenticator::readFile(const string &filename, char delim)
{

    // open the file
    ifstream infile(filename);

    // Make sure the file opened successfully
    if(infile.is_open())
    {
        string line; // This will hold each line of the file
        while(getline(infile, line))
        {
            // Parse each line for username and password
            string username, password;
            stringstream strStrm(line);

            // Assume the first two delimited values of the line
            // are username and password, respectively
            getline(strStrm, username, delim);
            getline(strStrm, password, delim);

            // Store extracted username and password in the map attribute
            m_users[username] = password;
        }

        //Close file handle
        infile.close();
    }
}