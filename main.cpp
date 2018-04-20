extern "C" {
#include "csapp.h"
}
#include <memory>
#include <list>
#include "UserProxy.h"
#include <tuple>


using namespace std;

typedef tuple<int, NetMessage> args_t;

void* client_handler(void *pargs);
SQLite::Database* SourceManager::db_ptr = nullptr;
map<string, NetMessage> SourceManager::online_users = {};
map<string, list<MyFile>> SourceManager::users_files = {};

int main(int argc, char** argv){
    //Signal(SIG_INT)
    SourceManager::bind_db("/home/redim/user.db3");

    list<pthread_t* > threadList = {};
    sockaddr clientaddr = {};
    socklen_t clientlen;
    char LISTENPORT[6];
    strcpy(LISTENPORT, "2345");

    int listenfd = open_listenfd(LISTENPORT);
    while (true){
        int clientfd = Accept(listenfd, &clientaddr, &clientlen);
        if (clientfd < 0) continue;
        pthread_t* newThread = new pthread_t;
        NetMessage net_mes{clientaddr, clientlen};
        args_t args = make_tuple(clientfd, net_mes);
        if (pthread_create(newThread, NULL, client_handler, &args) == 0){
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
    args_t* args = (args_t* )pargs;
    UserProxy responser(get<0>(*args), get<1>(*args));
    int* state_ptr = new int(responser.serve());

    return state_ptr; /*hopefully responser will close clientfd itself.*/
}