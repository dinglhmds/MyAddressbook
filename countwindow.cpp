#include "countwindow.h"
#include "ui_countwindow.h"

#include <QMessageBox>
#include <QString>

#include <QFile>
#include <QDebug>

#include "classmate.h"
#include "relative.h"
#include "friend.h"
#include "colleague.h"

#define FILENAME "D:\\QtProject\\AddressBook_homework\\AddressBook.txt"

CountWindow::CountWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CountWindow)
{
    ui->setupUi(this);
}

CountWindow::~CountWindow()
{
    delete ui;
}

void CountWindow::on_pushButton_clicked()
{
    emit goback();
}

void CountWindow::initData()
{
    this->ui->dataBrowser->clear();
    memset(months, 0, sizeof(months));

    //如果data非空就清空data
    if (data.size() != 0)
    {
        vector<Person*> temp;
        data.swap(temp);
    }

    for (int i = 0; i < 12; i++)
    {
        vector<Person*> temp;
        people[i].swap(temp);
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
}

void CountWindow::count()
{
    for (auto& val : data)
    {
        int m = val->birthday.toInt() / 100 % 100;

        months[m-1]++;
        people[m-1].push_back(val);

    }

    for (int i = 0; i < 12; i++)
    {
        if (!months[i])
        {
            continue;
        }

        QString str = " " + QString::number(i+1) + " 月份出生人数：" + QString::number(months[i]) + " 人\n";
        QString names = "姓名：";

        for (auto& val : people[i])
        {
            names = names + val->name + " 、 ";
        }

        names = names.left(names.size() - 3);
        names += "\n\n";

        this->ui->dataBrowser->append(str);
        this->ui->dataBrowser->append(names);
    }
}

