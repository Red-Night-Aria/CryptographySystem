//
// Created by 绯夜 on 2018/4/6.
//

#include "SourceManager.h"

bool SourceManager::check_login(const string &username, const string &password) {
    return true;
}

void SourceManager::add_user_share(const string &username, vector<MyFile> &content) {

}

void SourceManager::fetch_fileList(char *username) {

}

FileCollection SourceManager::fetch_fileList() {
    return vector<MyFile>();
}

const NetMessage &SourceManager::get_user_addr(const string &username) {
    return <#initializer#>;
}
