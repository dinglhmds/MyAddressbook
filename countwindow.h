#ifndef COUNTWINDOW_H
#define COUNTWINDOW_H

#include "person.h"
#include <QMainWindow>
#include <vector>

using namespace std;

namespace Ui {
class CountWindow;
}

class CountWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CountWindow(QWidget *parent = nullptr);
    ~CountWindow();

private:
    Ui::CountWindow *ui;

signals:
    void goback();

private slots:
    void on_pushButton_clicked();

public:
    void initData();
    void count();

public:
    vector<Person*> data;
    int months[12] = {0};
    vector<Person*> people[12];
};

#endif // COUNTWINDOW_H
