#ifndef STRINGTOHEX_H
#define STRINGTOHEX_H

#include<QString>

class StringToHex
{
public:
    StringToHex();
    QByteArray strToHex(QString str);

private:
    char charToHex(char c);
};

#endif // STRINGTOHEX_H
