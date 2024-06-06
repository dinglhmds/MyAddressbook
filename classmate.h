#ifndef CLASSMATE_H
#define CLASSMATE_H

#include "person.h"

class Classmate : public Person
{
public:
    Classmate(QString name, QString phone, QString email, QString birthday,
              QString school, int type);
    virtual void show();
    virtual QString getRemark() { return school; }
    virtual void setRemark(QString str) { school = str; }
    QString school;
};

#endif // CLASSMATE_H
