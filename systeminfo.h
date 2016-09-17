#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QObject>
#include <QFile>
#include <QDebug>

class SystemInfo : public QObject
{
    Q_OBJECT
public:
    enum CPU{
        NAME=0,
        KERNEL_NUM=1,
        FREQ=2,
        BUF_SIZE=3
    };

    explicit SystemInfo(QObject *parent = 0);
    QVector<QString> getCPUInfo();
    QVector<QString> getVersionInfo();


signals:

public slots:

private:
    QFile file;
};

#endif // SYSTEMINFO_H
