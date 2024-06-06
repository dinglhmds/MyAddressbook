#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include "person.h"
#include <QMainWindow>
#include <vector>

using namespace std;

namespace Ui {
class AddWindow;
}

class AddWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddWindow(QWidget *parent = nullptr);
    ~AddWindow();

private:
    Ui::AddWindow *ui;

signals:
    void goback();

private slots:
    void on_cancelButton_clicked();

    void on_addButton_clicked();

public:
    vector<Person*> data;
    vector<Person*>::iterator findIt;

public:
    void initData();
    void addPerson();

    void save(const QString& fileName);

    void clearEdit();

    bool birthdayIsValid(QString birthday);
    inline int getDays(int m, int y)
    {
        {
            if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
            {
                return 31;
            }
            else if (m != 2)
            {
                return 30;
            }
            else
            {
                if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)
                {
                    return 29;	//闰年29
                }
                else
                {
                    return 28;	//平年28
                }
            }
        }
    }
};

#endif // ADDWINDOW_H
