#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDir>
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>

#include "classmate.h"
#include "relative.h"
#include "friend.h"
#include "colleague.h"

#include <algorithm>

#include <QTextCodec>   //需修改.pro兼容Qt5

#include <QFontMetrics>
#include <QModelIndex>

#define FILENAME "D:\\QtProject\\AddressBook_homework\\AddressBook.txt"
#define PINYIN_FILENAME ":/pinyin.txt"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //qInfo()<<QDir::currentPath();
    readFile(FILENAME);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //表格不可编辑
    ui->tableView->setStyleSheet("background-color:transparent");       //表格透明

    setFixedSize(760,540);

    //背景图片
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(":/pictures/background.png");
    QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);

    //表格适应窗体
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //setCentralWidget(ui->tableView);

    //调整列宽度
    this->refreshTabColWidth();

    //拼音文件
    this->pinyinData.GetPinYinMap(PINYIN_FILENAME, m_pinyin);
    //"D:\\QtProject\\AddressBook_homework\\pinyin.txt"

    //查看
    connect(ui->allAct, SIGNAL(triggered()), this, SLOT(allActTriggered()));
    connect(ui->relativeAct, SIGNAL(triggered()), this, SLOT(relativeActTriggered()));
    connect(ui->colleagueAct, SIGNAL(triggered()), this, SLOT(colleagueActTriggered()));
    connect(ui->friendAct, SIGNAL(triggered()), this, SLOT(friendActTriggered()));
    connect(ui->classmateAct, SIGNAL(triggered()), this, SLOT(classmateActTriggered()));


//编辑
    //修改
    modifyWindow = new EditWindow;
    modifyWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);    //隐藏右上方关闭按钮
    connect(ui->modifyAct, SIGNAL(triggered()), this, SLOT(editActTriggered()));    //转到编辑页面
    connect(modifyWindow, &EditWindow::goback, this, [=](){
        modifyWindow->close();
        this->readFile(FILENAME);
        this->refreshTabColWidth();
        this->show();
    });   //回到主界面

    //删除
    delWindow = new DeleteWindow;
    delWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    connect(ui->delAct, SIGNAL(triggered()), this, SLOT(delActTriggered()));
    connect(delWindow, &DeleteWindow::goback, this, [=](){
        delWindow->close();
        this->readFile(FILENAME);
        this->refreshTabColWidth();
        this->show();
    });

    //新建
    addWindow = new AddWindow;
    addWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    connect(ui->addAct, SIGNAL(triggered()), this, SLOT(addActTriggered()));
    connect(addWindow, &AddWindow::goback, this, [=](){
        addWindow->close();
        this->readFile(FILENAME);
        this->refreshTabColWidth();
        this->show();
    });
//


    //主界面搜索
    connect(this->ui->searchEdit, SIGNAL(returnPressed()), ui->searchButton, SLOT(click()), Qt::UniqueConnection);
    ui->searchEdit->setPlaceholderText("search");

    //排序
    connect(ui->sortName, SIGNAL(triggered()), this, SLOT(sortNameActTriggered()));
    connect(ui->sortBirthday, SIGNAL(triggered()), this, SLOT(sortBirthdayActTriggered()));

    //更多

    //统计每月生日人数
    cntWindow = new CountWindow;
    cntWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    connect(ui->countBirthMonthAct, SIGNAL(triggered()), this, SLOT(countBirthMonthActTriggered()));
    connect(cntWindow, &CountWindow::goback, this, [=](){
        cntWindow->close();
        this->show();
    });

    //发送祝贺email
    congratulateWindow = new SendCongratulationWindow;
    congratulateWindow->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    connect(ui->sendCongratulationAct, SIGNAL(triggered()), this, SLOT(sendCongratulationActTriggered()));
    connect(congratulateWindow, &SendCongratulationWindow::goback, this, [=](){
        congratulateWindow->close();
        this->show();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTabColWidth()
{
    int tabWidth = ui->tableView->width() - 45;
    int sumWidth = 0;
    vector<double> width;

    qInfo()<<tabWidth;
    for (int x = 0; x < ui->tableView->model()->columnCount(); x++)
    {
        int maxWidth = 0;
        //身份 姓名 电话 生日 用最大值做宽度，邮箱备注用平均值做宽度
        if (x == 0 || x == 1 || x == 3 || x == 3)
        {
            for (int y = 0; y < ui->tableView->model()->rowCount(); y++)
            {
                QModelIndex index = ui->tableView->model()->index(y, x);
                QString text = index.data().toString();
                //qInfo() << "Row:" << y << "Column:" << x << "Text:" << text;
                maxWidth = qMax(maxWidth, QFontMetrics(ui->tableView->font()).horizontalAdvance(text));
            }
        }
        else
        {
            double sumw = 0;
            for (int y = 0; y < ui->tableView->model()->rowCount(); y++)
            {
                QModelIndex index = ui->tableView->model()->index(y, x);
                QString text = index.data().toString();
                //qInfo() << "Row:" << y << "Column:" << x << "Text:" << text;
                sumw += QFontMetrics(ui->tableView->font()).horizontalAdvance(text) * 1.0;
            }
            maxWidth = int(sumw / (ui->tableView->model()->rowCount() * 1.0));
        }

        sumWidth += maxWidth + 20;
        width.push_back((maxWidth + 20) * 1.0);
    }

    //归一化
    for (int x = 0; x < ui->tableView->model()->columnCount(); x++)
    {
        width[x] = width[x] / double(sumWidth);
        //qInfo()<<width[x];
    }

    for (int x = 0; x < ui->tableView->model()->columnCount(); x++)
    {
        ui->tableView->setColumnWidth(x, width[x] * tabWidth);
    }
}

void MainWindow::readFile(const QString &fileName)
{
    qInfo()<<fileName;
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qInfo()<<"open failed!";
    }
    else
    {
        qInfo()<<"open succceeded!";
    }

    QTextStream stream(&file);

    QStringList headers = stream.readLine().split(" ");
    m_headers = headers;

    QStandardItemModel* model = new QStandardItemModel(this);
    ui->tableView->setModel(model);

    model->setHorizontalHeaderLabels(headers);



    //如果data非空就清空data
    if (data.size() != 0)
    {
        vector<Person*> temp;
        data.swap(temp);
    }

    while (!stream.atEnd())
    {
        QStringList lineData = stream.readLine().split(" ");

        QList<QStandardItem*> item;

        QString type = lineData[0];
        QString name = lineData[1];
        QString phone = lineData[2];
        QString birthday = lineData[3];
        QString email = lineData[4];
        QString remark = lineData[5];

        if (type == "亲戚")
        {
            data.push_back(new Relative(name, phone, email, birthday, remark, 1));
        }
        else if (type == "朋友")
        {
            data.push_back(new Friend(name, phone, email, birthday, remark, 2));
        }
        else if (type == "同事")
        {
            data.push_back(new Colleague(name, phone, email, birthday, remark, 3));
        }
        else if (type == "同学")
        {
            data.push_back(new Classmate(name, phone, email, birthday, remark, 4));
        }

        for (auto &val : lineData)
        {
            item.push_back(new QStandardItem(val));
        }

        model->appendRow(item);
    }
}

void MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qInfo()<<"open failed";
        return;
    }

    QTextStream stream(&file);
    stream<<"分类 姓名 手机号 生日 邮箱 备注\n";

    for (auto &val : data)
    {

        QString str = val->getType() + ' ' + val->name + ' ' + val->phone
                      + ' ' + val->birthday + ' ' + val->email + ' ' + val->getRemark() + '\n';

        stream << str;
    }

    file.close();
}

inline double jaroSimilarity(const QString &s1, const QString &s2) {
    //const double matchingThreshold = 0.7;

    int m = 0; // Number of matching characters
    int s1Length = s1.length();
    int s2Length = s2.length();

    if (s1Length == 0 || s2Length == 0) {
        return 0.0;
    }

    int range = std::max(0, std::max(s1Length, s2Length) / 2 - 1);

    bool* s1Matched = new bool[s1Length]{false};
    bool* s2Matched = new bool[s2Length]{false};

    for (int i = 0; i < s1Length; i++) {
        int start = std::max(0, i - range);
        int end = std::min(i + range + 1, s2Length);

        for (int j = start; j < end; j++) {
            if (!s2Matched[j] && s1.at(i) == s2.at(j)) {
                s1Matched[i] = true;
                s2Matched[j] = true;
                m++;
                break;
            }
        }
    }

    if (m == 0) {
        delete[] s1Matched;
        delete[] s2Matched;
        return 0.0;
    }

    int k = 0; // Number of transpositions
    int pos = 0;
    for (int i = 0; i < s1Length; i++) {
        if (s1Matched[i]) {
            while (!s2Matched[pos]) {
                pos++;
            }
            if (s1.at(i) != s2.at(pos)) {
                k++;
            }
            pos++;
        }
    }

    delete[] s1Matched;
    delete[] s2Matched;

    return (m / static_cast<double>(s1Length) +
            m / static_cast<double>(s2Length) +
            (m - k/2) / static_cast<double>(m)) / 3.0;
}


void MainWindow::search(QString &nameToSearch)
{
    //qInfo()<<jaroSimilarity("farmville", "faremivel");
    vector <pair<Person*, double>> relativity;
    for (auto val : data)
    {
        pair<Person*, double> temp;
        temp.first = val;
        temp.second = jaroSimilarity(nameToSearch, val->name) * 1.7
                      + jaroSimilarity(nameToSearch, val->birthday) * 0.4
                      + jaroSimilarity(nameToSearch, val->email) * 0.4
                      + jaroSimilarity(nameToSearch, val->getRemark()) * 1.5;
        //相似度，生日和邮箱占比较低，姓名占比较高

        relativity.push_back(temp);
    }

    std::sort(std::begin(relativity), std::end(relativity),
              [](pair<Person*, double> p1, pair<Person*, double> p2)
              {return p1.second > p2.second; }
    );

    QStandardItemModel* searchModel = new QStandardItemModel(this);
    ui->tableView->setModel(searchModel);
    searchModel->setHorizontalHeaderLabels(m_headers);

    for (auto& val : relativity)
    {
        QList<QStandardItem*> item;

        if (val.first->type == 1)
        {
            item.push_back(new QStandardItem("亲戚"));
        }
        else if (val.first->type == 2)
        {
            item.push_back(new QStandardItem("朋友"));
        }
        else if (val.first->type == 3)
        {
            item.push_back(new QStandardItem("同事"));
        }
        else if (val.first->type == 4)
        {
            item.push_back(new QStandardItem("同学"));
        }
        item.push_back(new QStandardItem(val.first->name));
        item.push_back(new QStandardItem(val.first->phone));
        item.push_back(new QStandardItem(val.first->birthday));
        item.push_back(new QStandardItem(val.first->email));
        item.push_back(new QStandardItem(val.first->getRemark()));

        searchModel->appendRow(item);

        //if (val.second < 1.2) break;
    }

    this->refreshTabColWidth();
}

void MainWindow::sortName()
{
    std::sort(std::begin(data), std::end(data),
              [this](Person* p1, Person* p2) {return this->cmpName(p1, p2); });
}

bool MainWindow::cmpName(Person* p1, Person* p2)
{
    QString name1 = p1->name;
    QString name2 = p2->name;

    auto lastName1 = name1[0];
    auto lastName2 = name2[0];

    if (isZh(lastName1) && isZh(lastName2)) //中中
    {
        return this->m_pinyin[p1->name[0]][0] < this->m_pinyin[p2->name[0]][0];
    }
    else if (isZh(lastName1) && !isZh(lastName2))    //中>英
    {
        return false;
    }
    else if (!isZh(lastName1) && isZh(lastName2))    //英<中
    {
        return true;
    }
    else if (!isZh(lastName1) && !isZh(lastName2))    //英英
    {
        return name1 < name2;
    }
    return true;
}

void MainWindow::sortBirthday()
{
    std::sort(std::begin(data), std::end(data),
              [](Person* p1, Person* p2) {return p1->birthday < p2->birthday; });
}



bool MainWindow::isZh(const QChar &c)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray data = codec->fromUnicode(c);

    // 中文unicode编码范围是0X4E00~0X9FA5,即utf8范围0xe4b880 ~ 0xe9baa5
    // 使用utf8编码占3个字节
    if (data.size() == 3 && (data.at(0) & 0xF0) == 0xE0 && (data.at(1) & 0xC0) == 0x80 && (data.at(2) & 0xC0) == 0x80)
    {
        return true;
    }

    return false;
}

void MainWindow::allActTriggered()
{
    QStandardItemModel* allModel = new QStandardItemModel(this);
    ui->tableView->setModel(allModel);
    allModel->setHorizontalHeaderLabels(m_headers);

    for (auto& val : data)
    {
        QList<QStandardItem*> item;
        if (val->type == 1)
        {
            item.push_back(new QStandardItem("亲戚"));
        }
        else if (val->type == 2)
        {
            item.push_back(new QStandardItem("朋友"));
        }
        else if (val->type == 3)
        {
            item.push_back(new QStandardItem("同事"));
        }
        else if (val->type == 4)
        {
            item.push_back(new QStandardItem("同学"));
        }
        item.push_back(new QStandardItem(val->name));
        item.push_back(new QStandardItem(val->phone));
        item.push_back(new QStandardItem(val->birthday));
        item.push_back(new QStandardItem(val->email));
        item.push_back(new QStandardItem(val->getRemark()));

        allModel->appendRow(item);

        //qInfo()<<"all";
    }
    this->refreshTabColWidth();
}

void MainWindow::relativeActTriggered()
{
    QStandardItemModel* relativeModel = new QStandardItemModel(this);
    ui->tableView->setModel(relativeModel);
    relativeModel->setHorizontalHeaderLabels(m_headers);

    for (auto& val : data)
    {
        QList<QStandardItem*> item;
        if (val->type == 1)
        {
            item.push_back(new QStandardItem("亲戚"));
            item.push_back(new QStandardItem(val->name));
            item.push_back(new QStandardItem(val->phone));
            item.push_back(new QStandardItem(val->birthday));
            item.push_back(new QStandardItem(val->email));
            item.push_back(new QStandardItem(val->getRemark()));
        }
        else
        {
            continue;
        }

        relativeModel->appendRow(item);

        //qInfo()<<"relative";
    }
    this->refreshTabColWidth();
}

void MainWindow::friendActTriggered()
{
    QStandardItemModel* friendModel = new QStandardItemModel(this);
    ui->tableView->setModel(friendModel);
    friendModel->setHorizontalHeaderLabels(m_headers);

    for (auto& val : data)
    {
        QList<QStandardItem*> item;
        if (val->type == 2)
        {
            item.push_back(new QStandardItem("朋友"));
            item.push_back(new QStandardItem(val->name));
            item.push_back(new QStandardItem(val->phone));
            item.push_back(new QStandardItem(val->birthday));
            item.push_back(new QStandardItem(val->email));
            item.push_back(new QStandardItem(val->getRemark()));
        }
        else
        {
            continue;
        }

        friendModel->appendRow(item);

        //qInfo()<<"friend";
    }
    this->refreshTabColWidth();
}

void MainWindow::colleagueActTriggered()
{
    QStandardItemModel* colleagueModel = new QStandardItemModel(this);
    ui->tableView->setModel(colleagueModel);
    colleagueModel->setHorizontalHeaderLabels(m_headers);

    for (auto& val : data)
    {
        QList<QStandardItem*> item;
        if (val->type == 3)
        {
            item.push_back(new QStandardItem("同事"));
            item.push_back(new QStandardItem(val->name));
            item.push_back(new QStandardItem(val->phone));
            item.push_back(new QStandardItem(val->birthday));
            item.push_back(new QStandardItem(val->email));
            item.push_back(new QStandardItem(val->getRemark()));
        }
        else
        {
            continue;
        }

        colleagueModel->appendRow(item);

        //qInfo()<<"colleague";
    }
    this->refreshTabColWidth();
}

void MainWindow::classmateActTriggered()
{
    QStandardItemModel* classmateModel = new QStandardItemModel(this);
    ui->tableView->setModel(classmateModel);
    classmateModel->setHorizontalHeaderLabels(m_headers);

    for (auto& val : data)
    {
        QList<QStandardItem*> item;
        if (val->type == 4)
        {
            item.push_back(new QStandardItem("同学"));
            item.push_back(new QStandardItem(val->name));
            item.push_back(new QStandardItem(val->phone));
            item.push_back(new QStandardItem(val->birthday));
            item.push_back(new QStandardItem(val->email));
            item.push_back(new QStandardItem(val->getRemark()));
        }
        else
        {
            continue;
        }

        classmateModel->appendRow(item);

        //qInfo()<<"classmate";
    }
    this->refreshTabColWidth();
}

void MainWindow::editActTriggered()
{
    this->hide();
    modifyWindow->clearEdit();
    modifyWindow->show();
}

void MainWindow::delActTriggered()
{
    this->hide();
    delWindow->clearEdit();
    delWindow->show();
}

void MainWindow::addActTriggered()
{
    this->hide();
    addWindow->initData();
    addWindow->clearEdit();
    addWindow->show();
}

void MainWindow::on_searchButton_clicked()
{
    QString str = ui->searchEdit->text();
    if (!ui->searchEdit->text().isEmpty())
    {
        this->search(str);
    }
    else
    {
        return;
    }
}

void MainWindow::sortNameActTriggered()
{
    this->sortName();
    this->allActTriggered();
}

void MainWindow::sortBirthdayActTriggered()
{
    this->sortBirthday();
    this->allActTriggered();
}

void MainWindow::countBirthMonthActTriggered()
{
    this->hide();
    cntWindow->initData();
    cntWindow->count();
    cntWindow->show();
}

void MainWindow::sendCongratulationActTriggered()
{
    this->hide();
    congratulateWindow->clearBrowser();
    congratulateWindow->initData();
    this->congratulateWindow->show();
    congratulateWindow->birthdayIsNearby();
}
