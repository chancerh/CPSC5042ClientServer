//Author  : Group#2
//Date    : 03/12/2022
//Version : 2.0
//Filename: Authenticator.h

#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H
#include <string>
#include <unordered_map>

using namespace std;
/**
 * Authenticates users using a username and password data read from a
 * CSV file.
 */
class Authenticator {
public:
    /**
     * Constructor - creates an instance of the Authenticator class with
     * usernames and passwords read from the file passed in to the fileName
     * parameter.
     * @param fileName the path to the CSV file containing usernames and
     * passwords. The first item in a row will be used as the username, and
     * the second item will be used as a password.
     */
    Authenticator(const string &fileName);
    ~Authenticator();

    /**
     * Checks the credentials passed in to the username and password parameters
     * @param username a string with the username to check
     * @param password a string with the password to check
     * @return true if the username exists and is associated with the password,
     *         false otherwise
     */
    bool authenticate(const string &username, const string &password);
private:
    unordered_map<string, string> m_users; // Store username and passwords
    // read from file passed into constructor

    /**
     * Reads a username and password data from a char delimited file. Defaults
     * to using a comma as the delimiter.
     * @param filename The path to the file containing username and password
     * data.
     * @param delim The delimiter to use when reading the file. Defaults to
     * comma.
     */
    void readFile(const string &filename, char delim = ',');

};
#endif //AUTHENTICATOR_H
