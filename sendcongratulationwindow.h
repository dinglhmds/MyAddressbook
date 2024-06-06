#ifndef SENDCONGRATULATIONWINDOW_H
#define SENDCONGRATULATIONWINDOW_H

#include <QMainWindow>
#include "person.h"
#include <vector>

#include "zsmtp.h"

using namespace std;

namespace Ui {
class SendCongratulationWindow;
}

class SendCongratulationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SendCongratulationWindow(QWidget *parent = nullptr);
    ~SendCongratulationWindow();

private:
    Ui::SendCongratulationWindow *ui;

signals:
    void goback();

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

public:
    vector<Person*> data;
    vector<Person*> people;
    ZSmtp *smtp = NULL;

public:
    void initData();

    void clearBrowser();

    QString formText(Person* val);

    void birthdayIsNearby();
    inline int getDays(int m, int y)    //获取每个月天数
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

#endif // SENDCONGRATULATIONWINDOW_H
