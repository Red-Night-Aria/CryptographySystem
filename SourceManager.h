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

using namespace std;

struct NetMessage{
    sockaddr addr;
    socklen_t len;
};

struct File{
    string filename;
    char sha_256[256];
};

class SourceManager {
public:
    static void add_online_user(char* username, NetMessage* addr);
    static void fetch_user_fileList(char* username);

    /*if success, add user to online list; failed when user already online.*/
    static bool check_login(const string& username, const string& password);

private:
    static map<string, NetMessage> online_users;
    static map<string, list<File>> available_files;
};


#endif //CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H
