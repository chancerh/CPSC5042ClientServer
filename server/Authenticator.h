#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H
#include <string>
#include <unordered_map>

using namespace std;

class Authenticator {
public:
    Authenticator(const string &fileName);

    bool authenticate(const string &username, const string &password);
private:
    unordered_map<string, string> m_users;
    void readFile(const string &filename, char delim = ',');


};


#endif //AUTHENTICATOR_H
