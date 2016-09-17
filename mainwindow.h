#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void updateProcInfo();
    void onBtProcKillCliked();
protected:
    void timerEvent( QTimerEvent *event );
    int timerId;

private:
    Ui::MainWindow *ui;
    QStandardItemModel *procModel;
    QString selectedPid;
    int timeCounter;
    QVector<double> X;
    QVector<double> cpuY;
    QVector<double> memX;
    QVector<double> memY;
    void initial();
    void updateSysInfo();
    void updateMemInfo();
    bool boundTable(QString item);
};

#endif // MAINWINDOW_H
