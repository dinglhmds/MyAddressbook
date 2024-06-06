#ifndef COLLEAGUE_H
#define COLLEAGUE_H

#include "person.h"

class Colleague : public Person
{
public:
    Colleague(QString name, QString phone, QString email, QString birthday,
              QString company, int type);
    virtual void show();
    virtual QString getRemark() { return company; }
    virtual void setRemark(QString str) { company = str; }
    QString company;
};

#endif // COLLEAGUE_H
