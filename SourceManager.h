//
// Created by 绯夜 on 2018/4/6.
//

#ifndef CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H
#define CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <list>
#include <map>
#include <array>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include <boost/filesystem.hpp>

#define fs boost::filesystem
using namespace std;


struct NetMessage{
    sockaddr addr;
    socklen_t len;
};

struct MyFile{
    string filename;
    array<char, 256> sha_256;
};

const size_t SHA256_LEN = 256/8;
const size_t SALT_LEN = 8;

using FileCollection = vector<MyFile>;

class SourceManager {
public:
    static void add_online_user(string username, NetMessage& addr);
    static void fetch_fileList(char* username);
    static FileCollection fetch_fileList();
    static void add_user_share(const string& username, vector<MyFile>& content);
    static const NetMessage& get_user_addr(const string& username);
    static void bind_db(const string &dbpath);
    static bool sign_up(const string &username, const char sha256_pwd[256]);

    /*if success, add user to online list; failed when user already online.*/
    static bool check_login(const string &username, const char *digest, const string &salt);
    static const size_t MAXFILENAME = 256;

private:
    static map<string, NetMessage> online_users;
    static map<string, list<MyFile>> users_files;
    static SQLite::Database *db_ptr;
};

#endif //CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H
