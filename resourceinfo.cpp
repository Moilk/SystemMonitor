#include "resourceinfo.h"

ResourceInfo::ResourceInfo(QObject *parent) : QObject(parent)
{
    cpuRate=0;
    memRate=0;
    memAll=0;
    memUsed=0;
    memUn=0;
    buffer=0;
    cached=0;

    swapRate=0;
    swapAll=0;
    swapUsed=0;
    swapUn=0;
}

bool ResourceInfo::refresh(){
    QString tmpStr;
    QString tmp;
    char buf[1024];
    int res;
    int flag=0;
    int heard=-1;

    file.setFileName("/proc/meminfo");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"error: open /proc/meminfo failed!";
        return false;
    }
    while(flag<6){
        res=file.readLine(buf,sizeof(buf));
        if(res<=0){
            qDebug()<<"error: read /proc/meminfo failed!";
            break;
        }
        tmpStr=QString(buf);

        if((heard=tmpStr.indexOf("MemTotal"))!=-1){
            tmp=tmpStr.mid(heard+10,tmpStr.length()-13);
            tmp=tmp.trimmed();
            memAll=tmp.toInt()/1024;
            flag++;
        }else if((heard=tmpStr.indexOf("MemAvailable"))!=-1){
            tmp=tmpStr.mid(heard+14,tmpStr.length()-17);
            tmp=tmp.trimmed();
            memUn=tmp.toInt()/1024;
            flag++;
        }else if((heard=tmpStr.indexOf("Buffers"))!=-1){
            tmp=tmpStr.mid(heard+9,tmpStr.length()-12);
            tmp=tmp.trimmed();
            buffer=tmp.toInt()/1024;
            flag++;
        }else if((heard=tmpStr.indexOf("Cached"))!=-1){
            tmp=tmpStr.mid(heard+8,tmpStr.length()-11);
            tmp=tmp.trimmed();
            cached=tmp.toInt()/1024;
            flag++;
        }else if((heard=tmpStr.indexOf("SwapTotal"))!=-1){
            tmp=tmpStr.mid(heard+11,tmpStr.length()-14);
            tmp=tmp.trimmed();
            swapAll=tmp.toInt()/1024;
            flag++;
        }else if((heard=tmpStr.indexOf("SwapFree"))!=-1){
            tmp=tmpStr.mid(heard+10,tmpStr.length()-13);
            tmp=tmp.trimmed();
            swapUn=tmp.toInt()/1024;
            flag++;
        }
    }
    file.close();

    memUsed=memAll-memUn;
    swapUsed=swapAll-swapUn;
    memRate=(memUsed*1000)/memAll;
    if(swapUsed!=0){
        swapRate=(swapUsed*1000)/swapAll;
    }else{
        swapRate=0;
    }

    if(!refreshCPURate())
        return false;

    return true;
}

bool ResourceInfo::refreshCPURate(){
    // cpu利用率
    int idle1=0;
    int idle2=0;
    int total1=0;
    int total2=0;

    sampling(&idle1,&total1);
    sleep(70);
    sampling(&idle2,&total2);
    if(idle1==idle2)
        return false;
    cpuRate=1000-(idle2-idle1)*1000/(total2-total1);

    return true;
}

bool ResourceInfo::sampling(int *idle,int *total){
    int res;
    QString tmpStr;
    char buf[1024];
    int tmpNum;

    *idle=0;
    *total=0;

    file.setFileName("/proc/stat");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"error: open /proc/stat failed!";
        return false;
    }
    res=file.readLine(buf,sizeof(buf));
    if(res<=0){
        qDebug()<<"read /proc/stat failed!";
        return false;
    }
    tmpStr=QString(buf);
    for(int i=1;i<=7;i++){
        tmpNum=tmpStr.section(" ",i,i).toInt();
        *total+=tmpNum;
        if(i==5){
            *idle=tmpNum;
        }
    }
    file.close();

    return true;
}

int ResourceInfo::getCPURate(){
    return cpuRate;
}

int ResourceInfo::getMemRate(){
    return memRate;
}

int ResourceInfo::getMemAll(){
    return memAll;
}

int ResourceInfo::getMemUsed(){
    return memUsed;
}

int ResourceInfo::getMemun(){
    return memUn;
}

int ResourceInfo::getBuffer(){
    return buffer;
}

int ResourceInfo::getCached(){
    return cached;
}

int ResourceInfo::getSwapRate(){
    return swapRate;
}

int ResourceInfo::getSwapAll(){
    return swapAll;
}

int ResourceInfo::getSwapUsed(){
    return swapUsed;
}

int ResourceInfo::getSwapUn(){
    return swapUn;
}

void ResourceInfo::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
