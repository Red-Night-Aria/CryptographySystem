#include "mainwindow.h"
#include "dialog.h"
#include "ui_msgdialog.h"
#include "ui_signdialog.h"

MessageDialog::MessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
}

void MessageDialog::setMessage(std::string msg){
    ui->label->setText(QString::fromStdString(msg));
}

MessageDialog::~MessageDialog()
{
    delete ui;
}


SignDialog::SignDialog(Proxy* proxy) :
    QDialog(NULL),
    ui(new Ui::SignDialog),
    proxy(proxy)
{
    ui->setupUi(this);

    connect(ui->signin_btn, SIGNAL(clicked()), this, SLOT(signin()));
    connect(ui->signup_btn, SIGNAL(clicked()), this, SLOT(signup()));
}

void SignDialog::signin(){
    if(salt.empty())
        salt = proxy->getsalt();

    const QString Qusername = ui->username_lineedit->text();
    const string username = Qusername.toStdString();
    const QString Qpassword = ui->password_lineedit->text();
    const string password = Qpassword.toStdString();

    if(proxy->signin_client(username, password, salt)){
        MainWindow* w = new MainWindow();
        w->show();
        this->~SignDialog();
    }else{
        ui->result_label->setText(QString::fromStdString("登录失败，帐号或密码错误"));
    }
}

void SignDialog::signup()
{
    const QString Qusername = ui->username_lineedit->text();
    const string username = Qusername.toStdString();
    const QString Qpassword = ui->password_lineedit->text();
    const string password = Qpassword.toStdString();

    if(proxy->signup_client(username, password)){
        MainWindow w;
        w.show();
        this->~SignDialog();
    }else{
        ui->result_label->setText(QString::fromStdString("注册失败"));
    }
}

SignDialog::~SignDialog()
{
    delete ui;
}
