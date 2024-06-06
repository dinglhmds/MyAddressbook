#include "relative.h"
using namespace std;
Relative::Relative(QString name, QString phone, QString email, QString birthday, QString relation, int type)
    :Person(name, phone, email, birthday, type), relation(relation)
{
}

void Relative::show()
{
    Person::show();
    qInfo() << relation;
}
