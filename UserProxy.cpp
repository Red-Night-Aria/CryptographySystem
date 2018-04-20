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

int UserProxy::serve() {
    //TODO: close dead connection
    int state = 0;
    char opt;
    bool quitFlag = false;
    while (true){
        rio_readnb(rp, &opt, 1);
        //write(stdout->_fileno, &opt, 1);
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
    return state;
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
    if (Rio_readlineb(rp, buf1, MAXUSERNAME)<=0 || Rio_readnb(rp, buf2, SHA256_LEN)<=0)
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
    if (!hasLogin) {
        Rio_writen(client_fd, (void*)"0", 1);
        return -1;
    }

    vector<MyFile> data = {};
    char filename_buf[SourceManager::MAXFILENAME+1];

    unsigned int num;
    Rio_readnb(rp, &num, 4);
    for (int i=0; i<num; i++){
        MyFile tmp = {};

        Rio_readlineb(rp, filename_buf, SourceManager::MAXFILENAME);
        Rio_readlineb(rp, tmp.sha_256.data(), SourceManager::MAXFILENAME);
        tmp.filename = filename_buf;

        data.push_back(tmp);
    }

//    for (auto item: data){
//        cout << item.filename << " "<< item.sha_256.data() << endl;
//    }
    SourceManager::add_user_share(username, data);
    return 0;
}

int UserProxy::get_file_list() {
    if (!hasLogin) return -1;

    FileCollection fc = SourceManager::fetch_fileList();
    size_t num = fc.size();
    Rio_writen(client_fd, &num, 4);
    for (auto& file: fc){
        Rio_writen(client_fd, (void *)(file.filename.c_str()), file.filename.size());
        Rio_writen(client_fd, (void* )("\n"), 1);
        Rio_writen(client_fd, file.sha_256.data(), 256);
    }

    return 0;
}

int UserProxy::request_addr() {
    if (!hasLogin) return -1;

    char user_buf[MAXUSERNAME];
    Rio_readlineb(rp, user_buf, MAXUSERNAME);
    string req_user = user_buf;
    const NetMessage& req_netmsg = SourceManager::get_user_addr(user_buf);
//    char write_buf[sizeof(NetMessage)];
//    memcpy(write_buf, &req_netmsg, sizeof(NetMessage));
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

