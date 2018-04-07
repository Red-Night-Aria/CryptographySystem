//
// Created by redim on 18-4-5.
//

#ifndef CRYPTOGRAPHYSYSTEM_RESPONSER_H
#define CRYPTOGRAPHYSYSTEM_RESPONSER_H

extern "C"{
    #include "csapp.h"
}

#include <string>
#include <exception>
#include "SourceManager.h"

class NetworkException: public std::exception{
public:
    const char* what() const throw(){
        return "network error! close connection.";
    }
};

class UserProxy {
public:
    explicit UserProxy(int fd, NetMessage user_net_msg);
    int serve(); /*run until client quit or error occur*/
    ~UserProxy();
private:
    rio_t* rp;
    int client_fd;
    NetMessage net_msg;
    int sign_up();
    int sign_in();
    int upload();
    int get_file_list();
    int request_file();
    static const int MAXUSERNAME = 40;
    static const int MAXPASSWORD = 40;
    std::string username;
    bool hasLogin = false;

    /*verify and delete unavailable files*/
    void update_available_files(std::string username);
};


#endif //CRYPTOGRAPHYSYSTEM_RESPONSER_H
