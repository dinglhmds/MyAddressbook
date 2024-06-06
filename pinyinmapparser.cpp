#include "pinyinmapparser.h"

#include <QDebug>
#include <QFile>
#include <QString>

using namespace std;

// 获取拼音映射文件
int PinyinMapParser::GetPinYinMap(const QString& path, map<QString, QString>& pin_yin_map)
{
    QFile file(path);
    //路径非空
    if (path.isEmpty())
    {
        qInfo() << "pinyin path emtpy, invalid param";
        return -1;
    }
    //文件打开成功
    if (!file.open(QIODevice::ReadOnly))
    {
        qInfo() << "pinyin open failed!";
    }
    else
    {
        qInfo() << "pinyin open succceeded!";
    }

    QTextStream stream(&file);

    while (!stream.atEnd())
    {
        QString tmp_pinyin = stream.readLine();

        if (tmp_pinyin.contains("="))
        {
            QString zh, pinyin;
            int i = tmp_pinyin.indexOf('=');
            if (i != -1 && i != tmp_pinyin.size()-1)
            {
                zh = tmp_pinyin.left(i);
                pinyin = tmp_pinyin.mid(i+1);

                // 去掉拼音末尾声调
                if (pinyin.contains(","))
                {
                    int j = pinyin.indexOf(",");
                    pinyin = pinyin.left(j-1);
                }
                else
                {
                    pinyin = pinyin.left(pinyin.length()-1);
                }
                // 取出来后放到字典里
                //qDebug() << "zh:" << zh << ", pinyin:" << pinyin;
                pin_yin_map[zh] = pinyin;
            }
        }
    }

    file.close();
    return 0;
}
