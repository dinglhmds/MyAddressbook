#include "classmate.h"

using namespace std;
Classmate::Classmate(QString name, QString phone, QString email, QString birthday, QString school, int type)
    :Person(name, phone, email, birthday, type), school(school)
{
}

void Classmate::show()
{
    Person::show();
    qInfo() << school;
}
