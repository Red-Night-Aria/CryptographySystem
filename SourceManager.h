//
// Created by 绯夜 on 2018/4/6.
//

#ifndef CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H
#define CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <set>
#include <map>
#include <array>
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include <boost/filesystem.hpp>

#define fs boost::filesystem
using namespace std;

const size_t SHA256_LEN = 256/8;
const size_t SALT_LEN = 8;
const size_t MAXFILENAME = 256;

struct NetMessage{
    sockaddr addr;
    socklen_t len;
};

struct MyFile{
    string filename;
    char sha_256[SHA256_LEN];

    friend bool operator< (const MyFile& f1, const MyFile& f2){
        for (int i=0; i<SHA256_LEN; i++) {
            if (f1.sha_256[i] < f2.sha_256[i])
                return true;
            else if (f1.sha_256[i] > f2.sha_256[i])
                return false;
        }
        return true;
    }
};

using FileCollection = vector<MyFile>;

class SourceManager {
public:
    static void add_online_user(string username, NetMessage& addr);
    static const map<string, set<MyFile>> &fetch_userfile_List();
    static void add_user_share(const string& username, vector<MyFile>& content);
    static const NetMessage& get_user_addr(const string& username);
    static void bind_db(const string &dbpath);
    static bool sign_up(const string &username, const char sha256_pwd[256]);

    /*failed when user already online or password error*/
    static bool check_login(const string &username, const char *digest, const string &salt);

private:
    static map<string, NetMessage> online_users;
    static map<string, set<MyFile>> users_files;
    static SQLite::Database *db_ptr;
};

#endif //CRYPTOGRAPHYSYSTEM_SOURCEMANAGER_H
