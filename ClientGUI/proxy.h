#ifndef PROXY_H
#define PROXY_H

extern "C"{
#include <csapp.h>
}

#include <string>
#include <stdio.h>
#include <vector>
#include <openssl/sha.h>

using namespace std;

struct doc{
    string filename;
    string uploader;
    string sha;
};

const size_t SHA256_LEN = 256/8;
const size_t SALT_LEN = 8;
const size_t MAXFILENAME = 256;
const size_t MAXUSERNAME = 40;

class Proxy
{
public:
    explicit Proxy(char hostname[], char port[]);
    ~Proxy();

    int clientfd;

    bool signup_client(const string &username, const string &password);
    bool signin_client(const string &username, const string &password, const string &salt);
    string getsalt();
    vector<doc> findall();
    vector<doc> search(const string &key);
    void upload(const string &files);

private:
    rio_t* rp;

    char signup(char* msg, size_t msglen);
    char signin(char* msg, size_t msglen);
    string getfilelist();
    string request_addr(const string &username);
    void calsha(const string &source, unsigned char* hash);
    void calsha(const unsigned char* source, size_t sourcelen, unsigned char* hash);
};

#endif // PROXY_H
