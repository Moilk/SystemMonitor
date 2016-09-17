#ifndef PROC_H
#define PROC_H

#include <QObject>
#include <QStandardItemModel>
#include <QDir>
#include <QDebug>

class Proc : public QObject
{
    Q_OBJECT
public:
    explicit Proc(QObject *parent = 0);
    void freshModel(QStandardItemModel *model);
    int getSumProc();
    int getRunProc();

signals:

public slots:

private:
    int sumProc;
    int runProc;
    QFile file;
    QString idProc;
    QString nameProc;
    QString timeProc;
    QString statusProc;
    QString memProc;
    QString priProc;
    void clearModel(QStandardItemModel *model);
    void setStatusProc(QString status);
    int isDigitStr(QString src);
    bool reloadProc(QString idProc);
};

#endif // PROC_H
