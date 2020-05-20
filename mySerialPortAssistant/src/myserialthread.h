#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QMutex>
#include <stringtohex.h>

class StringToHex;
class MySerialThread : public QThread
{
    Q_OBJECT
public:
    MySerialThread(QObject *parent = nullptr);

    void transaction(const QString &portName, const QString &request, const int baudrate, const int dataBit,
                     const int stopBit, const int parity, const bool isHex);
private:
    void run() override;
    QString portName;
    QSerialPort::DataBits dataBit;
    QSerialPort::StopBits stopBit;
    QSerialPort::BaudRate baudrate;
    QSerialPort::Parity parity;
    QString request;
    QMutex mutex;
    StringToHex strToHex;

signals:
        void error(const QString &s);
        void response(const QString &s);

private slots:
};

#endif // SERIALTHREAD_H
