//
// Created by 绯夜 on 2018/4/6.
//

#ifndef CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H
#define CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H

extern "C"{
#include <sys/socket.h>
#include <netdb.h>
};

#include <string>
#include <list>
#include <map>
#include <array>
#include <vector>

using namespace std;


struct NetMessage{
    sockaddr addr;
    socklen_t len;
};

struct MyFile{
    string filename;
    array<char, 256> sha_256;
};

using FileCollection = vector<MyFile>;

class SourceManager {
public:
    static void add_online_user(string username, NetMessage& addr);
    static void fetch_fileList(char* username);
    static FileCollection fetch_fileList();
    static void add_user_share(const string& username, vector<MyFile>& content);
    static const NetMessage& get_user_addr(const string& username);

    /*if success, add user to online list; failed when user already online.*/
    static bool check_login(const string& username, const string& password);
    static const size_t MAXFILENAME = 256;

private:
    static map<string, NetMessage> online_users;
    static map<string, list<MyFile>> users_files;
};


#endif //CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H
