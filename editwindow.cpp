#include "editwindow.h"
#include "ui_editwindow.h"
#include <QMessageBox>
#include <QString>

#include <QFile>
#include <QDebug>

#include "classmate.h"
#include "relative.h"
#include "friend.h"
#include "colleague.h"

#define FILENAME "D:\\QtProject\\AddressBook_homework\\AddressBook.txt"

EditWindow::EditWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EditWindow)
{
    ui->setupUi(this);

    findIt = data.end();

    connect(this->ui->nameEdit, SIGNAL(returnPressed()), ui->searchButton, SLOT(click()), Qt::UniqueConnection);

}

EditWindow::~EditWindow()
{
    delete ui;
}

void EditWindow::on_cancelButton_clicked()
{
    emit goback();
}

void EditWindow::on_searchButton_clicked()
{
    if (this->ui->nameEdit->text().isEmpty())
    {
        QMessageBox::information(NULL, "警告", "姓名不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else
    {
        QString str = this->ui->nameEdit->text();
        search(str);
    }
}

void EditWindow::search(QString &nameToSearch)
{
    //如果data非空就清空data
    if (data.size() != 0)
    {
        vector<Person*> temp;
        data.swap(temp);
    }

    QString fileName =  FILENAME;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qInfo()<<"open failed!";
    }
    else
    {
        qInfo()<<"open succceeded!";
    }

    QTextStream stream(&file);

    QStringList header = stream.readLine().split(" ");

    while (!stream.atEnd())
    {
        QStringList lineData = stream.readLine().split(" ");

        QString type = lineData[0];
        QString name = lineData[1];
        QString phone = lineData[2];
        QString birthday = lineData[3];
        QString email = lineData[4];
        QString remark = lineData[5];

        if (type == "亲戚")
        {
            data.push_back(new Relative(name, phone, email, birthday, remark, 1));
        }
        else if (type == "朋友")
        {
            data.push_back(new Friend(name, phone, email, birthday, remark, 2));
        }
        else if (type == "同事")
        {
            data.push_back(new Colleague(name, phone, email, birthday, remark, 3));
        }
        else if (type == "同学")
        {
            data.push_back(new Classmate(name, phone, email, birthday, remark, 4));
        }
    }
    bool flag = false;
    findIt = data.end();
    for (auto it = data.begin(); it != data.end();)
    {
        if ((*it)->name == nameToSearch)
        {
            findIt = it;
            flag = true;
            break;
        }
        else
        {
            ++it;
        }
    }
    if (!flag)
    {
        this->ui->phoneEdit->clear();
        this->ui->birthdayEdit->clear();
        this->ui->emailEdit->clear();
        this->ui->remarkEdit->clear();
        QMessageBox::information(NULL, "警告", "未找到此人", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else
    {
        this->ui->birthdayEdit->setReadOnly(true);
        this->ui->phoneEdit->setText((*findIt)->phone);
        this->ui->emailEdit->setText((*findIt)->email);
        this->ui->birthdayEdit->setText((*findIt)->birthday);
        this->ui->remarkEdit->setText((*findIt)->getRemark());
    }

    file.close();
}

void EditWindow::modify()
{
    QString newPhone = this->ui->phoneEdit->text();
    QString newEmail = this->ui->emailEdit->text();
    QString newRemark = this->ui->remarkEdit->text();

    (*findIt)->phone = newPhone;
    (*findIt)->email = newEmail;
    (*findIt)->setRemark(newRemark);

    //(*findIt)->show();
}

void EditWindow::save(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qInfo()<<"open failed";
        return;
    }

    QTextStream stream(&file);
    stream<<"分类 姓名 手机号 生日 邮箱 备注\n";

    for (auto &val : data)
    {

        QString str = val->getType() + ' ' + val->name + ' ' + val->phone
                      + ' ' + val->birthday + ' ' + val->email + ' ' + val->getRemark() + '\n';

        stream << str;
    }

    file.close();
}

void EditWindow::clearEdit()
{
    this->ui->nameEdit->clear();
    this->ui->phoneEdit->clear();
    this->ui->birthdayEdit->clear();
    this->ui->emailEdit->clear();
    this->ui->remarkEdit->clear();
}

void EditWindow::on_okButton_clicked()
{
    if (this->findIt!=data.end())
    {
        modify();
        this->save(FILENAME);
        emit goback();
    }
    else
    {
        QMessageBox::information(NULL, "警告", "姓名不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

