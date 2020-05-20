#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <timethread.h>
#include <qpixmap.h>
#include <myserialthread.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


private:
    Ui::Widget *ui;

    QDateTime dateTime;
    QString messages;
    TimeThread *timethread;
    MySerialThread *myserialthread;
    QPixmap *background;

    bool isStart = false;

signals:

private slots:

    void on_sendBtn_clicked();

    void on_clearBtn_clicked();

    void on_beginBtn_clicked();

    void timeUpdate();

    void sendMessages();

    void showMessages(const QString &s);

    void showError(const QString &s);
};

#endif // WIDGET_H
