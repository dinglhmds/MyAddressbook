#ifndef DELETEWINDOW_H
#define DELETEWINDOW_H

#include "person.h"
#include <QMainWindow>
#include <vector>

using namespace std;

namespace Ui {
class DeleteWindow;
}

class DeleteWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeleteWindow(QWidget *parent = nullptr);
    ~DeleteWindow();

signals:
    void goback();

private slots:
    void on_cancelButton_clicked();

    void on_searchButton_clicked();

    void on_delButton_clicked();

private:
    Ui::DeleteWindow *ui;

public:
    vector<Person*> data;
    vector<Person*>::iterator findIt;

public:
    void search(QString &nameToSearch);

    void deletePerson();

    void save(const QString& fileName);

    void clearEdit();
};

#endif // DELETEWINDOW_H
