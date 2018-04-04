//
// Created by redim on 18-4-5.
//

#ifndef CRYPTOGRAPHYSYSTEM_RESPONSER_H
#define CRYPTOGRAPHYSYSTEM_RESPONSER_H

extern "C"{
    #include "csapp.h"
}


class Responser {
public:
    explicit Responser(int fd);
    int serve(); /*run until client quit or error occur*/
    ~Responser();
private:
    rio_t* rp;
    int client_fd;
    int sign_up();
    int sign_in();
    int upload();
    int get_file_list();
    int request_file();
};


#endif //CRYPTOGRAPHYSYSTEM_RESPONSER_H
