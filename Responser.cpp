//
// Created by redim on 18-4-5.
//

#include "Responser.h"

Responser::Responser(int fd):rp(new rio_t()), client_fd(fd) {
    rio_readinitb(rp, fd);
}

int Responser::serve() {
    int state = 0;
    char opt;
    bool quitFlag = false;
    while (true){
        rio_readnb(rp, &opt, 1);
        char tmp[2]; rio_readnb(rp, &tmp, 2); //drop out \r\n. just for telnet test
        write(stdout->_fileno, &opt, 1);
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
                request_file();
                break;
            case '0':
            default:
                quitFlag = true;
                break;
        }
        fprintf(stdout, "flag is %d now\n", quitFlag);
        if (quitFlag) break;
    }
    return state;
}

Responser::~Responser() {
    Close(client_fd);
    free(rp);
}

int Responser::sign_up() {
    rio_writen(client_fd, (void* )"1\n", 2);
    return 0;
}

int Responser::sign_in() {
    rio_writen(client_fd, (void* )"2\n", 2);
    return 0;
}

int Responser::upload() {
    return 0;
}

int Responser::get_file_list() {
    return 0;
}

int Responser::request_file() {
    return 0;
}
