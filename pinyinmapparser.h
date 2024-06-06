#ifndef PINYINMAPPARSER_H
#define PINYINMAPPARSER_H
#include <map>
#include <QString>

using namespace std;
class PinyinMapParser
{
public:
    PinyinMapParser(){}
    ~PinyinMapParser(){}

public:
    // 从文件中获取拼音字典
    static int GetPinYinMap(const QString& path, map<QString, QString>& pin_yin_map);
};

#endif // PINYINMAPPARSER_H
