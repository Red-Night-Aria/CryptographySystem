#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->findall_btn, SIGNAL(clicked()), this, SLOT(on_findall_btn_clicked()));

    content = ui->content_widge;



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessageDialog(std::string msg){
    message_dialog = new MessageDialog(this);
    message_dialog->setMessage(msg);
    message_dialog->show();
    message_dialog->exec();
}

void MainWindow::on_findall_btn_clicked()
{

}
