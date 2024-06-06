#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QDebug>

class Person
{
public:
    Person(QString name, QString phone, QString email, QString birthday, int type);
    Person() {};
    QString name;
    QString phone;
    QString email;
    QString birthday;
    int type;

    virtual void show();
    virtual QString getRemark() { return ""; };
    virtual void setRemark(QString str) { str = ""; };
    virtual QString getType() {
        if (type == 1) return "亲戚";
        if (type == 2) return "朋友";
        if (type == 3) return "同事";
        if (type == 4) return "同学";
        else return "error";
    }
};
#endif //PERSON_H
