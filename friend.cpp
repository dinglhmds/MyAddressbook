#include "friend.h"

using namespace std;
Friend::Friend(QString name, QString phone, QString email, QString birthday, QString meetPlace, int type)
    :Person(name, phone, email, birthday, type), meetPlace(meetPlace)
{
}

void Friend::show()
{
    Person::show();
    qInfo() << meetPlace;
}
