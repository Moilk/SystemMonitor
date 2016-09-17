#include "systeminfo.h"

SystemInfo::SystemInfo(QObject *parent) : QObject(parent)
{

}

QVector<QString> SystemInfo::getCPUInfo(){
    QVector<QString> cpuInfo(4,"\0");
    QString tmpStr;
    char buf[1024];
    int f_pos;
    int res;
    int flag=0;

    file.setFileName("/proc/cpuinfo");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"error: can't open cpuinfo";
        return cpuInfo;
    }
    while(flag<4){
        res=file.readLine(buf,size_t(buf));
        if(res<=0){
            break;
        }
        tmpStr=QString(buf);
        if((f_pos=tmpStr.indexOf("model name"))!=-1){
            f_pos+=13;
            cpuInfo[0]=QString(tmpStr.mid(f_pos,tmpStr.length()-14));
            flag++;
        }else if((f_pos=tmpStr.indexOf("cpu cores"))!=-1){
            f_pos+=12;
            cpuInfo[1]=QString(tmpStr.mid(f_pos,tmpStr.length()-13));
            flag++;
        }else if((f_pos=tmpStr.indexOf("cpu MHz"))!=-1){
            f_pos+=11;
            cpuInfo[2]=QString(tmpStr.mid(f_pos,tmpStr.length()-12));
            flag++;
        }else if((f_pos=tmpStr.indexOf("cache size"))!=-1){
            f_pos+=13;
            cpuInfo[3]=QString(tmpStr.mid(f_pos,tmpStr.length()-14));
            flag++;
        }
    }
    file.close();

    return cpuInfo;
}

QVector<QString> SystemInfo::getVersionInfo(){
    QVector<QString> vInfo(2,"\0");
    QString tmpStr;
    char buf[1024];
    int res;

    file.setFileName("/proc/version");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"error: can't open version";
        return vInfo;
    }
    res=file.readLine(buf,sizeof(buf));
    if(res>0){
        tmpStr=QString(buf);
        int pos1=tmpStr.indexOf("Linux version");
        int pos2=tmpStr.indexOf(" (");
        vInfo[0]=QString(tmpStr.mid(pos1,pos2-pos1));

        pos1=tmpStr.indexOf("gcc version");
        pos2=tmpStr.indexOf(" )");
        vInfo[1]=QString(tmpStr.mid(pos1,pos2-pos1));
    }
    file.close();

    return vInfo;
}

