#ifndef RELATIVE_H
#define RELATIVE_H

#include "person.h"

class Relative : public Person
{
public:
    Relative(QString name, QString phone, QString email, QString birthday,
             QString relation, int type);
    virtual void show();
    virtual QString getRemark() { return relation; }
    virtual void setRemark(QString str) { relation = str; }
    QString relation;
};

#endif // RELATIVE_H
