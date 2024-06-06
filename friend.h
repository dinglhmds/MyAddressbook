#ifndef FRIEND_H
#define FRIEND_H

#include "person.h"

class Friend : public Person
{
public:
    Friend(QString name, QString phone, QString email, QString birthday,
           QString meetPlace, int type);
    virtual void show();
    virtual QString getRemark() { return meetPlace; }
    virtual void setRemark(QString str) { meetPlace = str; }
    QString meetPlace;
};

#endif // FRIEND_H
