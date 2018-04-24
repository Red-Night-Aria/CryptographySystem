#include <QApplication>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "dialog.h"
#include "proxy.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    char hostname[20];
    strcpy(hostname, "127.0.0.1");
    char port[6];
    strcpy(port, "2345");
    Proxy* proxy = new Proxy{hostname, port};
    if(proxy->clientfd == -1){
        MessageDialog* msg = new MessageDialog(NULL);
        msg->setMessage("连接至索引服务器失败");
        msg->show();
    }
    else{
        SignDialog* sign_dialog = new SignDialog(proxy);
        sign_dialog->show();
    }
    return a.exec();
}
