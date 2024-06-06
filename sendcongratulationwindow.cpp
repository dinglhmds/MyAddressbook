#include "sendcongratulationwindow.h"
#include "ui_sendcongratulationwindow.h"

#include <ctime>

#include <QMessageBox>
#include <QString>

#include <QFile>
#include <QDebug>

#include "classmate.h"
#include "relative.h"
#include "friend.h"
#include "colleague.h"

#define FILENAME "D:\\QtProject\\AddressBook_homework\\AddressBook.txt"

SendCongratulationWindow::SendCongratulationWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SendCongratulationWindow)
{
    ui->setupUi(this);
}

SendCongratulationWindow::~SendCongratulationWindow()
{
    delete ui;
}

void SendCongratulationWindow::on_cancelButton_clicked()
{
    emit goback();
}

void SendCongratulationWindow::initData()
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
}

void SendCongratulationWindow::clearBrowser()
{
    this->ui->peopleBrowser->clear();
}

QString SendCongratulationWindow::formText(Person* val)
{
    /*
    被祝贺人姓名:
        祝生日快乐，健康幸福。
                    祝贺人姓名
    */
    QString temp = "";
    temp += val->name + ":\n\t祝生日快乐，健康幸福。\n\t\t\t\t";
    temp += "LHMDS";
    return temp;
}

void SendCongratulationWindow::birthdayIsNearby()
{
    struct tm t;    //tm结构体
    time_t now;     //声明time_t类型变量
    time(&now);     //获取系统日期和时间
    localtime_s(&t, &now);   //获取当地日期和时间

    //格式化输出本地时间
    int yearNow = t.tm_year + 1900;
    int monthNow = t.tm_mon + 1;
    int dayNow = t.tm_mday;
    //int weekdayNow = (t.tm_wday + 5) % 7;
    QString birthMin = QString::number(dayNow + monthNow * 100);

    //printf_s("目前时间：%d 年 %d 月 %d 日\n", yearNow, monthNow, dayNow, weekdayNow);

    dayNow = dayNow + 5;

    if (dayNow > getDays(monthNow, yearNow))
    {
        dayNow = dayNow % getDays(monthNow, yearNow);
        monthNow++;
        if (monthNow > 12)
        {
            monthNow = 1;
            yearNow++;
        }
    }

    QString birthMax = QString::number(dayNow + monthNow * 100);

    bool flag = false;

    //people非空则清空
    if (people.size())
    {
        vector<Person*> temp;
        people.swap(temp);
    }

    for (auto& val : data)
    {
        QString birth = QString::number((val->birthday.toInt()) % 1000);
        if (birth >= birthMin && birth <= birthMax)
        {
            people.push_back(val);
            flag = true;
        }
    }

    if (!flag)
    {
        QMessageBox::information(NULL, "提示", "未查询到五日内生日的联系人", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        this->on_cancelButton_clicked();
    }
    else
    {
        this->ui->peopleBrowser->append("以下联系人五天内生日，点击确定发送生日祝福\n\n");
        for (auto &val : people)
        {
            QString temp = val->name + " " + val->birthday + "\n";
            this->ui->peopleBrowser->append(temp);
        }
    }
}


void SendCongratulationWindow::on_okButton_clicked()
{
    bool flag = false;
    for (auto &val : people)
    {
        if (val->email.right(7) == "@qq.com")
        {
            QString emailText = formText(val);
            this->smtp = new ZSmtp; //默认是qq邮箱服务器，想用别的服务器就 new ZSmtp(Smtp服务器地址)
            connect(smtp, SIGNAL(disconnected()), smtp, SLOT(deleteLater()));	//发送完毕自行销毁
            smtp->sendEmail("your email address", "password", val->email, "生日祝福", emailText);    //参数都不需要转换为base64
        }
        else
        {
            flag = true;
        }
    }
    if (flag)
    {
        QMessageBox::information(NULL, "提示", "抱歉，目前仅支持QQ邮箱，未能全部发送", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

    emit goback();
}

