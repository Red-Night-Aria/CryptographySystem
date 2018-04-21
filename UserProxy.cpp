//
// Created by redim on 18-4-5.
//

#include "UserProxy.h"
#include <string>
#include <vector>
#include <array>
#include <iostream>

using namespace std;
UserProxy::UserProxy(int fd, NetMessage user_net_msg):rp(new rio_t()), client_fd(fd), net_msg(user_net_msg) {
    rio_readinitb(rp, fd);
}

void UserProxy::serve() {
    //int state = 1;
    char opt;
    bool quitFlag = false;
    while (true){
        if (rio_readnb(rp, &opt, 1)<1){
            /*connection closed*/
            break;
        }

        /*
        * if enter other func when hasLogin=False, then it must be an invaild request.
        * so we just close the connection by return -1;
        *
        * TODO deal with connection broken
        * TODO unwanted sign_up
        * */
        if (!hasLogin && (opt != '1' && opt != '2')){
            break;
        }
        switch (opt){
            case '1':
                sign_up();
                break;
            case '2':
                sign_in();
                break;
            case '3':
                upload();
                break;
            case '4':
                get_file_list();
                break;
            case '5':
                request_addr();
                break;
            case '0':
            default:
                quitFlag = true;
                break;
        }
        if (quitFlag) break;
    }
    //return state;
}

UserProxy::~UserProxy() {
    Close(client_fd);
    free(rp);
}

int UserProxy::sign_up() {
    char buf1[MAXUSERNAME], buf2[SHA256_LEN];

    /*network error. no reply*/
    if (Rio_readlineb(rp, buf1, MAXUSERNAME)<=0 || Rio_readnb(rp, buf2, SHA256_LEN)<SHA256_LEN)
        return -1;

    string newusr = buf1;

    /*db error. reply 0*/
    if (!SourceManager::sign_up(newusr, buf2)){
        Rio_writen(client_fd, (void*)"0", 1);
        return -2;
    }

    Rio_writen(client_fd, (void*)"1", 1);
    return 0;
}

int UserProxy::sign_in() {
    string salt = randstr();
    Rio_writen(client_fd, (void *)salt.c_str(), SALT_LEN);

    /*read username and password from socket*/
    char buf1[MAXUSERNAME], buf2[SHA256_LEN];
    if (Rio_readlineb(rp, buf1, MAXUSERNAME)<=0 || Rio_readnb(rp, buf2, SHA256_LEN)<SHA256_LEN)
        return -1;

    username = buf1;

    if (!SourceManager::check_login(username, buf2, salt)){
        Rio_writen(client_fd, (void*)"0", 1);
    }
    else {
        Rio_writen(client_fd, (void*)"1", 1);
        hasLogin = true;
        SourceManager::add_online_user(username, net_msg);
    }

    return 0;
}

int UserProxy::upload() {
    vector<MyFile> data = {};
    char filename_buf[MAXFILENAME+1];

    u_int32_t num;
    Rio_readnb(rp, &num, 4);
    for (int i=0; i<num; i++){
        MyFile tmp = {};
        Rio_readlineb(rp, filename_buf, MAXFILENAME);
        Rio_readnb(rp, tmp.sha_256, SHA256_LEN);
        tmp.filename = filename_buf;
        data.push_back(tmp);
    }

    SourceManager::add_user_share(username, data);
    Rio_writen(client_fd, (void*)"1", 1);
    return 0;
}

int UserProxy::get_file_list() {
    const map<string, set<MyFile>> user_file = SourceManager::fetch_userfile_List();
    u_int32_t user_num = user_file.size();

    Rio_writen(client_fd, &user_num, 4);
    for (auto& item: user_file){
        string username = item.first;
        Rio_writen(client_fd, (void *)username.c_str(), username.size());
        Rio_writen(client_fd, (void *)"\n", 1);
        u_int32_t book_num = item.second.size();
        Rio_writen(client_fd, &book_num, 4);
        for (auto& file: item.second){
            Rio_writen(client_fd, (void *)file.filename.c_str(), file.filename.size());
            Rio_writen(client_fd, (void *)"\n", 1);
            Rio_writen(client_fd, (void *)file.sha_256, SHA256_LEN);
        }
    }

    return 0;
}

int UserProxy::request_addr() {
    char user_buf[MAXUSERNAME+1];
    Rio_readlineb(rp, user_buf, MAXUSERNAME);
    string req_user = user_buf;
    const NetMessage& req_netmsg = SourceManager::get_user_addr(user_buf);
    Rio_writen(client_fd, (void* )(&req_netmsg), sizeof(NetMessage));

    return 0;
}

string UserProxy::randstr(const int n) {
    string s = {};
    for (int i=0; i<n; i++){
        s[i] = rand()%256;
    }
    return s;
}

//void UserProxy::update_available_files(std::string username) {
//    //TODO: save files' hash value in server to accelerate this proceduce
//    void* buf = malloc(4);
//    rio_readlineb(rp, buf, MAXLINE);
//    char* endPtr;
//    long nums = strtol(reinterpret_cast<char* >(buf), &endPtr, 10);
//    if (*endPtr != '\0')
//}

