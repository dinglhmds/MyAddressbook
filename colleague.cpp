#include "colleague.h"
using namespace std;
Colleague::Colleague(QString name, QString phone, QString email, QString birthday, QString company, int type)
    :Person(name, phone, email, birthday, type), company(company)
{
}

void Colleague::show()
{
    Person::show();
    qInfo() << company;
}
