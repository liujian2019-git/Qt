#include "stringtohex.h"

StringToHex::StringToHex()
{

}

char StringToHex::charToHex(char c) {
    if(c >= 0x30 && c <= 0x39) {
        c -= 0x30;
    }

    //c为'a' ~ 'f'
    else if(c >= 0x41 && c <= 0x46) {
        c -= 0x37;
    }

    //c为'A' ~ 'F'
    else if(c >= 0x61 && c <= 0x66) {
        c -= 0x57;
    }

    else {
        c = 0xff;
    }
    return c;
}

QByteArray StringToHex::strToHex(QString str) {
    QByteArray temp;
    int len = str.length();
    int index = 0;
    int lowHex;
    int upperHex;
    for(int i = 0; i < len ; i++) {
        if(str[i].toLatin1()!=' ') {
            temp[index] = str[i].toLatin1();
            index ++;
        }
    }
    QByteArray hexcode;
    for(int i = 0; i < index / 2 ; i++) {
        upperHex = charToHex(temp[i * 2 ])<<4;
        lowHex = charToHex(temp[i * 2 + 1]);
        hexcode[i] = upperHex + lowHex;
    }

    return hexcode;
}
