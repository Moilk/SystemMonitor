#include "proc.h"

Proc::Proc(QObject *parent) : QObject(parent)
{

}

void Proc::freshModel(QStandardItemModel *model){
    sumProc=0;
    runProc=0;

   QDir dir("/proc");
   QStringList list=dir.entryList();
   for (int i = 0; i < list.size(); ++i){
       idProc=list.at(i);
        if(isDigitStr(idProc)){
            continue;
        }
        sumProc++;
        if(!reloadProc(idProc))
            break;

        model->setItem(sumProc-1,0,new QStandardItem(nameProc));
        model->setItem(sumProc-1,1,new QStandardItem(idProc));
        model->setItem(sumProc-1,2,new QStandardItem(statusProc));
        model->setItem(sumProc-1,3,new QStandardItem(memProc));
        model->setItem(sumProc-1,4,new QStandardItem(priProc));

        file.close();
   }

   model->sort(2,Qt::DescendingOrder);
}

bool Proc::reloadProc(QString idProc){
    QFile file;
    QString tmpStr;
    char buf[1024];
    int heard;
    int tail;
    int res;

    file.setFileName("/proc/"+idProc+"/stat");
     if(!file.open(QIODevice::ReadOnly)){
         qDebug()<<"error: can't open "<<"/proc/"<<idProc<<"/stat";
         return false;
     }
     res=file.readLine(buf,sizeof(buf));
     if(res<=0){
         return false;
     }
     tmpStr=QString(buf);
     if(tmpStr.length()==0){
         return false;
     }
     heard=tmpStr.indexOf("(");
     tail=tmpStr.indexOf(")");
     nameProc=tmpStr.mid(heard+1,tail-heard-1);
     priProc=tmpStr.section(" ",17,17);
     memProc=tmpStr.section(" ",22,22);
     QString status=tmpStr.section(" ",2,2);
     setStatusProc(status);

     return true;
}

int Proc::isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
     const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    { //不是纯数字
        return -1;
    }
    else
    { //纯数字
        return 0;
    }
}

void Proc::setStatusProc(QString status){
    switch(status.at(0).toLatin1()){
    case 'S':
        statusProc="睡眠";
        break;
    case 'R':
        runProc++;
        statusProc="运行";
        break;
    case 'Z':
        statusProc="退出";
        break;
    case 'T':
        statusProc="暂停";
        break;
    case 'D':
        statusProc="不可中断睡眠";
        break;
    default:
        break;
    }
}


int Proc::getSumProc(){
    return sumProc;
}

int Proc::getRunProc(){
    return runProc;
}
