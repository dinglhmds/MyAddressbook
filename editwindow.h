#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include "person.h"
#include <QMainWindow>
#include <vector>
using namespace std;

namespace Ui {
class EditWindow;
}

class EditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = nullptr);
    ~EditWindow();

public:
    void search(QString &nameToSearch);
    void modify();

    void save(const QString& fileName);

    void clearEdit();

private:
    Ui::EditWindow *ui;

signals:
    void goback();

private slots:
    void on_cancelButton_clicked();
    void on_searchButton_clicked();
    void on_okButton_clicked();

public:
    vector<Person*> data;
    vector<Person*>::iterator findIt;

};

#endif // EDITWINDOW_H
