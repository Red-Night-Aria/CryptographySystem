#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "proxy.h"

namespace Ui {
class MessageDialog;
class SignDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(QWidget *parent = 0);
    void setMessage(std::string msg);
    ~MessageDialog();

private:
    Ui::MessageDialog *ui;
};


class SignDialog : public QDialog
{
    Q_OBJECT

public:
    Proxy* proxy;

    explicit SignDialog(Proxy* proxy);
    ~SignDialog();

private slots:
    void signup();
    void signin();

private:
    Ui::SignDialog *ui;
    string salt;
};
#endif // DIALOG_H
