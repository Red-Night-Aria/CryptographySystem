extern "C" {
#include "csapp.h"
}
#include <memory>
#include <list>
#include "Responser.h"

using namespace std;

list<pthread_t* > threadList = {};

void* client_handler(void *pargs);

int main(int argc, char** argv){
    //Signal(SIG_INT)
    struct sockaddr_in clientaddr = {};
    socklen_t clientlen;
    char LISTENPORT[6];
    strcpy(LISTENPORT, "2345");
    int listenfd = open_listenfd(LISTENPORT);
    while (true){
        int clientfd = Accept(listenfd, (SA* )&clientaddr, &clientlen);
        if (clientfd < 0) continue;
        pthread_t* newThread = new pthread_t;
        if (pthread_create(newThread, NULL, client_handler, &clientfd) == 0){
            /* create thread successfully */
            threadList.push_back(newThread);
        }
        else{
            rio_writen(clientfd, (void* )"0", 1); //tell client error
            Close(clientfd);
        }
    }

}

void *client_handler(void *pargs) {
    int clientfd = *(int* )pargs;
    //rio_writen(clientfd, (void* )"hello~", 6);
    Responser responser(clientfd);
    int* state_ptr = new int(responser.serve());

    return state_ptr; /*hopefully responser will close clientfd itself.*/
}