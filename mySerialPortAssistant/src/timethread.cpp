#include "timethread.h"

TimeThread::TimeThread(QObject *parent)
    : QThread(parent)
{

}

void TimeThread::run() {
    while(true) {
        sleep(1);
        emit timeUpdateSignal();
    }
}
