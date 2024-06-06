#include "Person.h"
using namespace std;

Person::Person(QString name, QString phone, QString email, QString birthday, int type)
    :name(name), phone(phone), email(email), birthday(birthday), type(type)
{
}

void Person::show()
{
    qInfo() << this->name << ' ' << this->phone << ' ' << this->birthday << ' ' << this->email << ' ';
}
