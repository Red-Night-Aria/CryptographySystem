#include "proxy.h"

using namespace std;

Proxy::Proxy(char hostname[], char port[])
{
    clientfd = open_clientfd(hostname, port);
    if(clientfd != -1){
        rp = new rio_t();
        rio_readinitb(rp, clientfd);
    }
}

Proxy::~Proxy()
{
    if(clientfd != -1)
        close(clientfd);
}

bool Proxy::signup_client(const string &username, const string &password)
{
    unsigned char* usermsg= new unsigned char[SHA256_LEN]();
    calsha(username + password, usermsg);
    size_t msglen = username.size()+SHA256_LEN+1;
    char concat_str[msglen];
    for (int i=0; i<username.size(); i++){
        concat_str[i] = username[i];
    }
    concat_str[username.size()] = '\n';
    for (int i=0; i<SHA256_LEN; i++){
        concat_str[username.size()+1+i] = usermsg[i];
    }
    char result = signup(concat_str, msglen);
    if(result == '1')
        return true;
    return false;
}

bool Proxy::signin_client(const string &username, const string &password, const string &salt)
{
    unsigned char* sha256_pwd= new unsigned char[SHA256_LEN]();
    calsha(username + password, sha256_pwd);
    unsigned char concat_str[SHA256_LEN + SALT_LEN];
    for (int i=0; i<SHA256_LEN; i++){
        concat_str[i] = sha256_pwd[i];
    }
    for (int i=0; i<SALT_LEN; i++){
        concat_str[SHA256_LEN+i] = salt[i];
    }
    unsigned char* usermsg= new unsigned char[SHA256_LEN]();
    calsha(&concat_str[0], SHA256_LEN + SALT_LEN, usermsg);
    size_t msglen = username.size()+SHA256_LEN+1;
    char msg[msglen];
    for (int i=0; i<username.size(); i++){
        msg[i] = username[i];
    }
    msg[username.size()] = '\n';
    for (int i=0; i<SHA256_LEN; i++){
        msg[username.size()+1+i] = usermsg[i];
    }
    char result = signin(msg, msglen);
    if(result == '1')
        return true;
    return false;
}

string Proxy::getsalt()
{
    char *salt = new char[SALT_LEN]();
    rio_writen(clientfd, (void* )"2", 1);
    rio_readnb(rp, salt, SALT_LEN);
    return string(salt);
}

vector<doc> Proxy::findall()
{

}

vector<doc> Proxy::search(const string &key)
{

}

void Proxy::upload(const string &files)
{

}

char Proxy::signup(char* msg, size_t msglen)
{
    char result;
    rio_writen(clientfd, (void* )"1", 1);
    rio_writen(clientfd, msg, msglen);
    rio_readnb(rp, &result, 1);
    return result;
}

char Proxy::signin(char* msg, size_t msglen)
{
    char result;
    rio_writen(clientfd, msg, msglen);
    rio_readnb(rp, &result, 1);
    return result;
}

string Proxy::getfilelist()
{

}

string Proxy::request_addr(const string &username)
{

}

void Proxy::calsha(const string &source, unsigned char* hash)
{
    SHA256((unsigned char *)source.c_str(), source.size(), hash);
}

void Proxy::calsha(const unsigned char* source, size_t sourcelen, unsigned char* hash)
{
    SHA256(source, sourcelen, hash);
}
