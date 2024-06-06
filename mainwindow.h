#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "person.h"
#include "editwindow.h"
#include "deletewindow.h"
#include "addwindow.h"
#include "countwindow.h"
#include "sendcongratulationwindow.h"

#include "pinyinmapparser.h"
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refreshTabColWidth();

    void readFile(const QString& fileName);
    void saveFile(const QString& fileName);

    void search(QString &nameToSearch);

    void sortName();
    void sortBirthday();
    bool cmpName(Person* p1, Person* p2);
    bool isZh(const QChar &c);

    bool birthdayIsNearby();
    inline int getDays(int m, int y)
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

private:
    Ui::MainWindow *ui;

public:         //其他窗口
    EditWindow* modifyWindow = NULL;
    DeleteWindow* delWindow = NULL;
    AddWindow* addWindow = NULL;
    CountWindow* cntWindow = NULL;
    SendCongratulationWindow* congratulateWindow = NULL;

public:         //数据
    std::vector<Person*> data;
    QStringList m_headers;
    PinyinMapParser pinyinData;
    map<QString, QString> m_pinyin;

private slots:  //查看
    void allActTriggered();
    void relativeActTriggered();
    void colleagueActTriggered();
    void friendActTriggered();
    void classmateActTriggered();

private slots:  //编辑
    void editActTriggered();
    void delActTriggered();
    void addActTriggered();

    void on_searchButton_clicked();

    void sortNameActTriggered();
    void sortBirthdayActTriggered();

    void countBirthMonthActTriggered();
    void sendCongratulationActTriggered();
};
#endif // MAINWINDOW_H
