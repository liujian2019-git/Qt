#include "widget.h"
#include "ui_widget.h"
#include "timethread.h"
#include <QSerialPortInfo>
#include <QPixmap>
#include <QDir>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    dateTime = QDateTime::currentDateTime();
    ui->receiveText->setText("");

    QTransform matrix;
    setAutoFillBackground(true);
    QString path = QDir::currentPath() + "/image/Dva.jpg";
    background = new QPixmap(path);
    *background = background->scaled(800, 600, Qt::KeepAspectRatio);
    *background = background->transformed(matrix);
    QPalette *palette = new QPalette;
    palette->setBrush(QPalette::Background, QBrush(*background));
    setPalette(*palette);
    myserialthread = new MySerialThread;

    timethread = new TimeThread;
    connect(timethread, SIGNAL(timeUpdateSignal()), this, SLOT(timeUpdate()));
    timethread->start();

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        ui->portNameBox->addItem(info.portName());

    ui->receiveText->setVerticalScrollBar(ui->receiveRoll);
    ui->receiveText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->receiveText->document()->setMaximumBlockCount(100);
    ui->beginBtn->setEnabled(true);
    connect(myserialthread, MySerialThread::error, this, &Widget::showError);
    connect(myserialthread, MySerialThread::response, this, &Widget::showMessages);
    connect(ui->baudrateBox, QComboBox::currentTextChanged, this, &Widget::on_sendBtn_clicked);
}

Widget::~Widget()
{
    delete ui;

}

void Widget::on_clearBtn_clicked() {
    ui->receiveText->setText("");
}

//开始按键槽函数
void Widget::on_beginBtn_clicked() {
    if(ui->beginBtn->text() == "打开串口") {
        ui->beginBtn->setText("关闭串口");
        this->isStart = true;
        sendMessages();
    } else if(ui->beginBtn->text() == "关闭串口") {
        ui->beginBtn->setText("打开串口");
        this->isStart = false;
    }
}

void Widget::on_sendBtn_clicked() {
    if(isStart)
        sendMessages();
}

void Widget::timeUpdate() {
    dateTime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(dateTime);
}

void Widget::sendMessages() {
    myserialthread->transaction(ui->portNameBox->currentText(), ui->sendLine->text(),ui->baudrateBox->currentIndex(),
                                ui->databitBox->currentIndex(), ui->stopbitBox->currentIndex(), ui->paritybitBox->currentIndex(),
                                ui->sendHex->isChecked());
}

void Widget::showMessages(const QString &s) {
    messages.append(s);
    if(ui->receIveHex->isChecked()) {
        messages = QString::fromUtf8(messages.toLatin1().toHex());
    }
    if(ui->beginBtn->text() == "关闭串口") {
        ui->receiveText->append(messages);
    }
    messages = "";
}

void Widget::showError(const QString &s) {
    ui->receiveText->append(s);
}

