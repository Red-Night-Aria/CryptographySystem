#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "dialog.h"
#include "proxy.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void showMessageDialog(std::string msg);
    ~MainWindow();

private slots:
    void on_findall_btn_clicked();

private:
    Ui::MainWindow* ui;
    MessageDialog* message_dialog;
    SignDialog* sign_dialog;

    QWidget* content;
};

#endif // MAINWINDOW_H
