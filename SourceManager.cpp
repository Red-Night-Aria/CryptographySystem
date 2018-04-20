//
// Created by 绯夜 on 2018/4/6.
//

#include <iostream>
#include "SourceManager.h"
#include <openssl/sha.h>


bool SourceManager::check_login(const string &username, const char *digest, const string &salt) {
    SQLite::Statement query(*db_ptr, "SELECT password FROM user WHERE username = ?");
    query.bind(1, username);
    if (query.executeStep()){
        const char* sha256_pwd = query.getColumn(0);

        char concat_str[SHA256_LEN+SALT_LEN+1];
        for (int i=0; i<SHA256_LEN; i++){
            concat_str[i] = sha256_pwd[i];
        }
        for (int i=0; i<SALT_LEN; i++){
            concat_str[SHA256_LEN+i] = salt[i];
        }

        unsigned char expect_digest[SHA256_LEN];
        SHA256((unsigned char *)concat_str, SHA256_LEN+SALT_LEN, expect_digest);

        bool flag = true;
        for (int i=0; i<SHA256_LEN; i++)
            if (expect_digest[i] != (unsigned char)digest[i]) {
                flag = false;
                break;
            }
        return flag;
    }
    return false;
}

void SourceManager::add_user_share(const string &username, vector<MyFile> &content) {

}

void SourceManager::fetch_fileList(char *username) {

}

FileCollection SourceManager::fetch_fileList() {
    return vector<MyFile>();
}

const NetMessage &SourceManager::get_user_addr(const string &username) {
}

void SourceManager::add_online_user(string username, NetMessage &addr) {

}

void SourceManager::bind_db(const string &dbpath) {
    fs::path db_path(dbpath);

    if (fs::exists(db_path)){
        db_ptr = new SQLite::Database(dbpath, SQLite::OPEN_READWRITE);
    }
    else{
        db_ptr = new SQLite::Database(dbpath, SQLite::OPEN_CREATE|SQLite::OPEN_READWRITE);
        db_ptr->exec("CREATE TABLE user (username TINYTEXT PRIMARY KEY, password CHAR(32))");
    }


}

bool SourceManager::sign_up(const string &username, const char *sha256_pwd) {
    SQLite::Statement query(*db_ptr, "SELECT * FROM user WHERE username = ?");
    query.bind(1, username);
    try{
        if (query.executeStep()){
            cout << "username already exist." << endl;
            return false;
        }
        SQLite::Statement insert_query(*db_ptr, "INSERT INTO user VALUES (?, ?)");
        insert_query.bind(1, username);
        insert_query.bind(2, sha256_pwd, 32); //sha256_pwd doesn't has a /0 tail. so it can't be passed as a string.
        if (insert_query.exec() != 1){
            cout << "insert failed." << endl;
            return false;
        }
    }
    catch(exception& err){
        cout << err.what() << endl;
        return false;
    }

    return true;
}
