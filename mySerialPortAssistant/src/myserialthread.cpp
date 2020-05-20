#include "myserialthread.h"
#include <QThread>
#include <QObject>
#include <stringtohex.h>
MySerialThread::MySerialThread(QObject *parent) : QThread(parent)
{

}

void MySerialThread::transaction(const QString &portName, const QString &request, const int baudrate, const int dataBit, const int stopBit, const int parity, const bool isHex) {

    QMutexLocker locker(&mutex);

    switch(baudrate)  {
        case 0 : this->baudrate = QSerialPort::Baud4800;   break;
        case 1 : this->baudrate = QSerialPort::Baud9600;   break;
        case 2 : this->baudrate = QSerialPort::Baud19200;   break;
        case 3 : this->baudrate = QSerialPort::Baud38400;   break;
        case 4 : this->baudrate = QSerialPort::Baud57600;   break;
        case 5 : this->baudrate = QSerialPort::Baud115200;   break;
        default: this->baudrate = QSerialPort::Baud9600;
    }

    switch(dataBit) {
        case 0 : this->dataBit = QSerialPort::Data8; break;
        case 1 : this->dataBit = QSerialPort::Data5; break;
        case 2 : this->dataBit = QSerialPort::Data6; break;
        case 3 : this->dataBit = QSerialPort::Data7; break;
        default: this->dataBit = QSerialPort::Data8; break;
    }

    switch (stopBit) {
        case 0 : this->stopBit = QSerialPort::OneStop; break;
        case 1 : this->stopBit = QSerialPort::OneAndHalfStop; break;
        case 2 : this->stopBit = QSerialPort::TwoStop; break;
        default: this->stopBit = QSerialPort::OneStop; break;
    }
    switch (parity) {
        case 0 : this->parity = QSerialPort::NoParity; break;
        default: this->parity = QSerialPort::NoParity; break;
    }

    if(isHex) {
        this->request = QString::fromUtf8(strToHex.strToHex(request));
    } else {
        this->request = request;
    }
    this->portName = portName;
    if(!isRunning())
        start();
}

void MySerialThread::run() {

    QSerialPort serial;
    serial.setPortName(portName);
    serial.setBaudRate(baudrate);
    serial.setDataBits(dataBit);
    serial.setStopBits(stopBit);
    serial.setParity(parity);
    serial.open(QIODevice::ReadWrite);

    if(serial.portName().isEmpty()) {
        emit error("portName is empty!");
        return;
    }

    while(true) {
        const QByteArray requestData = request.toUtf8();
        if(request != "") {
            serial.write(requestData);
            if(!serial.waitForBytesWritten()) {
                emit error("write time out!");
            }
            request = "";
        }
        if (serial.waitForReadyRead(100)) {
            QByteArray responseData = serial.readAll();
            while (serial.waitForReadyRead(10))
                responseData += serial.readAll();
            const QString response = QString::fromUtf8(responseData);
            emit this->response(response);
        }

        if(serial.portName().isEmpty()) {
            emit error("portName is empty!");
            return;
        }

        if(serial.portName() != portName || serial.baudRate() != baudrate || serial.dataBits() != dataBit || serial.stopBits() != stopBit) {
            serial.close();
            mutex.lock();
            serial.setPortName(portName);
            serial.setBaudRate(baudrate);
            serial.setDataBits(dataBit);
            serial.setStopBits(stopBit);
            serial.setParity(parity);
            mutex.unlock();
            printf("%d", baudrate);
            serial.open(QIODevice::ReadWrite);
        }
    }
}
