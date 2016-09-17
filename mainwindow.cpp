#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "systeminfo.h"
#include "proc.h"
#include "resourceinfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timerId=startTimer(1000);
    X=QVector<double>(60);
    cpuY=QVector<double>(60,0);
    memY=QVector<double>(60,0);
    for(int i=0;i<60;i++){
        X[i]=i;
    }
    initial();
}

MainWindow::~MainWindow()
{
    if(timerId!=0){
        killTimer(timerId);
    }
    delete ui;
}

/**
 * @brief MainWindow::initial
 * 界面初始化
 */
void MainWindow::initial(){
    // mainwindow
    this->setWindowIcon(QIcon(":/icon"));
    this->setWindowTitle("系统监视器");
    ui->tabWidget->setCurrentIndex(0);
    // about

    // proc
    procModel=new QStandardItemModel();
    ui->tv_PROC->setModel(procModel);
    procModel->setHorizontalHeaderItem(0,new QStandardItem(QObject::tr("进程名")));
    procModel->setHorizontalHeaderItem(1,new QStandardItem(QObject::tr("ID")));
    procModel->setHorizontalHeaderItem(2,new QStandardItem(QObject::tr("状态")));
    procModel->setHorizontalHeaderItem(3,new QStandardItem(QObject::tr("内存")));
    procModel->setHorizontalHeaderItem(4,new QStandardItem(QObject::tr("优先级")));

    //mem
    ui->cp_CPU->xAxis->setRange(0,60);
    ui->cp_CPU->yAxis->setRange(0,100);
    ui->cp_CPU->addGraph();

    ui->cp_MEM->xAxis->setRange(0,60);
    ui->cp_MEM->yAxis->setRange(0,100);
    ui->cp_MEM->addGraph();

    updateSysInfo();
    updateProcInfo();
    updateMemInfo();

    connect(ui->bt_PROC_refresh,SIGNAL(clicked(bool)),this,SLOT(updateProcInfo()));
    connect(ui->bt_PROC_kill,SIGNAL(clicked(bool)),this,SLOT(onBtProcKillCliked()));
    qDebug()<<"Start Success.";
}

void MainWindow::updateMemInfo(){
    ResourceInfo resourceInfo;
    resourceInfo.refresh();

    double rate=((double)resourceInfo.getCPURate())/10;
    ui->lb_CPU_rate->setText(QString::number(rate,'g',2));
    cpuY.pop_back();
    cpuY.prepend(rate);
    rate=((double)resourceInfo.getMemRate())/10;
    ui->lb_MEM_rate->setText(QString::number(rate,'g',2));
    memY.pop_back();
    memY.prepend(rate);

    ui->cp_CPU->graph(0)->setData(X,cpuY);
    ui->cp_CPU->replot();
    ui->cp_MEM->graph(0)->setData(X,memY);
    ui->cp_MEM->replot();

    ui->pb_SWT->setValue(resourceInfo.getSwapRate()/10);
    ui->lb_MEM_sum->setText(QString::number(resourceInfo.getMemAll()));
    ui->lb_MEM_used->setText(QString::number(resourceInfo.getMemUsed()));
    ui->lb_MEM_unused->setText(QString::number(resourceInfo.getMemun()));
    ui->lb_MEM_buf->setText(QString::number(resourceInfo.getBuffer()));
    ui->lb_MEM_cached->setText(QString::number(resourceInfo.getCached()));
    ui->lb_SWT_sum->setText(QString::number(resourceInfo.getSwapAll()));
    ui->lb_SWT_unused->setText(QString::number(resourceInfo.getSwapUn()));
    ui->lb_SWT_used->setText(QString::number(resourceInfo.getSwapUsed()));
}

/**
 * @brief MainWindow::updateProcInfo
 * 更新进程界面
 */
void MainWindow::updateProcInfo(){
    Proc proc;

    QStandardItem *item= procModel->item(ui->tv_PROC->currentIndex().row(),1);
    if(item)
        selectedPid=item->data(Qt::DisplayRole).toString();
    else
        selectedPid="";
    proc.freshModel(procModel);
    boundTable(selectedPid);
    int sumProc=proc.getSumProc();
    int runProc=proc.getRunProc();
    ui->lb_PROC_sum->setText(QString::number(sumProc));
    ui->lb_PROC_act->setText(QString::number(runProc));
}

/**
 * @brief MainWindow::updateSysInfo
 * 更新系统信息界面
 */
void MainWindow::updateSysInfo(){
    SystemInfo sysInfo;
    QVector<QString> vcCPU=sysInfo.getCPUInfo();
    QVector<QString> vcVersion=sysInfo.getVersionInfo();
    ui->lb_CPU_model->setText(vcCPU.at(0));
    ui->lb_CPU_num->setText(vcCPU.at(1));
    ui->lb_CPU_freq->setText(vcCPU.at(2));
    ui->lb_CPU_buf->setText(vcCPU.at(3));
    ui->lb_V_kernel->setText(vcVersion.at(0));
    ui->lb_V_complier->setText(vcVersion.at(1));
    QFile file("/proc/cpuinfo");
    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        ui->tb_detail->setPlainText(in.readAll());
    }
    file.close();
}

/**
 * @brief MainWindow::timerEvent
 * @param event
 * 每秒更新一次进程画面
 */
void MainWindow::timerEvent( QTimerEvent *event ){
    if(ui->tabWidget->currentIndex()==1){
        updateProcInfo();
    }
    if(ui->tabWidget->currentIndex()==2){
        updateMemInfo();
    }
}

/**
 * @brief MainWindow::onBtProcKillCliked
 * 点击按钮,杀死进程
 */
void MainWindow::onBtProcKillCliked(){
    QMessageBox box;

    if(selectedPid!=""){
        box.setWindowTitle("删除进程");
        box.setText("是否删除进程"+selectedPid.toLatin1()+"?");
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        if(box.exec()==QMessageBox::Yes){
            int res=system("kill "+selectedPid.toLatin1());
            if(res<0){
                ui->statusBar->showMessage("杀死进程失败.",3000);
            }else{
                ui->statusBar->showMessage("进程"+selectedPid.toLatin1()+"已被终止.",3000);
            }
        }
    }else{
        ui->statusBar->showMessage("未选中任何进程.",3000);
    }
}

/**
 * @brief MainWindow::boundTable
 * @param item
 * @return
 *
 * 绑定选中行
 */
bool MainWindow::boundTable(QString item){
    QList<QStandardItem*> list= procModel->findItems(item,Qt::MatchExactly,1);
    if(list.isEmpty()){
        return false;
    }
    ui->tv_PROC->selectRow(list.at(0)->row());

    return true;
}
