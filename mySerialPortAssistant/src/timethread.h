#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include "QThread"
#include "QObject"
#include <QDateTime>
#include <QTimer>

class TimeThread : public QThread
{
    Q_OBJECT
    
public:
    explicit TimeThread(QObject *parent = nullptr);

private slots:

    void run() override;

signals:

    void timeUpdateSignal();

};

#endif // TIMETHREAD_H
