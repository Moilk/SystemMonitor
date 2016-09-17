#ifndef RESOURCEINFO_H
#define RESOURCEINFO_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

class ResourceInfo : public QObject
{
    Q_OBJECT
public:
    explicit ResourceInfo(QObject *parent = 0);
    int getCPURate();
    int getMemRate();
    int getMemAll();
    int getMemUsed();
    int getMemun();
    int getBuffer();
    int getCached();
    int getSwapRate();
    int getSwapAll();
    int getSwapUsed();
    int getSwapUn();
    bool refresh();
    bool refreshCPURate();
    void sleep(unsigned int msec);
    bool sampling(int *idle,int *total);

signals:

public slots:

private:
    QFile file;
    int cpuRate;
    int memRate;
    int memAll;
    int memUsed;
    int memUn;
    int buffer;
    int cached;

    int swapRate;
    int swapAll;
    int swapUsed;
    int swapUn;
};

#endif // RESOURCEINFO_H
