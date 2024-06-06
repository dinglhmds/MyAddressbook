#include "addwindow.h"
#include "ui_addwindow.h"

#include <QMessageBox>
#include <QString>

#include <QFile>
#include <QDebug>

#include "classmate.h"
#include "relative.h"
#include "friend.h"
#include "colleague.h"

#include <ctime>

#define FILENAME "D:\\QtProject\\AddressBook_homework\\AddressBook.txt"

AddWindow::AddWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddWindow)
{
    ui->setupUi(this);
    connect(ui->birthdayEdit, SIGNAL(returnPressed()), ui->addButton, SLOT(click()), Qt::UniqueConnection);
    connect(ui->emailEdit, SIGNAL(returnPressed()), ui->addButton, SLOT(click()), Qt::UniqueConnection);
    connect(ui->nameEdit, SIGNAL(returnPressed()), ui->addButton, SLOT(click()), Qt::UniqueConnection);
    connect(ui->phoneEdit, SIGNAL(returnPressed()), ui->addButton, SLOT(click()), Qt::UniqueConnection);
    connect(ui->remarkEdit, SIGNAL(returnPressed()), ui->addButton, SLOT(click()), Qt::UniqueConnection);
}

AddWindow::~AddWindow()
{
    delete ui;
}

void AddWindow::on_cancelButton_clicked()
{
    emit goback();
}

void AddWindow::initData()
{
    this->ui->birthdayEdit->setPlaceholderText("yyyymmdd");
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

void AddWindow::addPerson()
{
    int currentIndex = this->ui->comboBox->currentIndex();
    QString currentItem = this->ui->comboBox->itemText(currentIndex);

    QString type = currentItem;
    QString name = ui->nameEdit->text();
    QString phone = ui->phoneEdit->text();
    QString birthday = ui->birthdayEdit->text();
    QString email = ui->emailEdit->text();
    QString remark = ui->remarkEdit->text();

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

void AddWindow::save(const QString &fileName)
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

void AddWindow::clearEdit()
{
    this->ui->nameEdit->clear();
    this->ui->phoneEdit->clear();
    this->ui->birthdayEdit->clear();
    this->ui->emailEdit->clear();
    this->ui->remarkEdit->clear();
}

bool AddWindow::birthdayIsValid(QString birthday)
{
    int inputBirthday = birthday.toInt();   //yyyymmdd
    int y = inputBirthday / 10000;
    int m = inputBirthday % 10000 / 100;
    int d = inputBirthday % 100;

    struct tm t;    //tm结构指针
    time_t now;     //声明time_t类型变量
    time(&now);     //获取系统日期和时间
    localtime_s(&t, &now);   //获取当地日期和时间

    int yearNow = t.tm_year + 1900;
    int monthNow = t.tm_mon + 1;
    int dayNow = t.tm_mday;

    if (m > 12 || d > this->getDays(m, y) || y > yearNow || (y == yearNow && m > monthNow)
        || (y == yearNow && m == monthNow && d > dayNow))
    {
        return false;
    }
    return true;
}


void AddWindow::on_addButton_clicked()
{


    //姓名生日不可为空，生日须合法
    if (ui->nameEdit->text().isEmpty())
    {
        QMessageBox::information(NULL, "警告", "姓名不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    /*else if (ui->phoneEdit->text().isEmpty())
    {
        QMessageBox::information(NULL, "警告", "手机号不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }*/
    else if (ui->birthdayEdit->text().isEmpty())
    {
        QMessageBox::information(NULL, "警告", "生日不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    else if (!this->birthdayIsValid(ui->birthdayEdit->text()))
    {
        QMessageBox::information(NULL, "警告", "请填写合适的生日", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    /*else if (ui->emailEdit->text().isEmpty())
    {
        QMessageBox::information(NULL, "警告", "邮箱不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }*/
    /*else if (ui->remarkEdit->text().isEmpty())
    {
        QMessageBox::information(NULL, "警告", "备注不能为空", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }*/
    else
    {
        this->addPerson();
        this->save(FILENAME);
        emit goback();
    }
}

