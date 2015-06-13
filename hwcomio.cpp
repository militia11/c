#include "hwcomio.h"

#include <QTimer>

HwComIO::HwComIO(QObject *parent, QSerialPort* sp)
    : QObject(parent)
    , port(sp)
{
    connect(port, SIGNAL(readyRead()), this, SLOT(incomingDataReady()));
    connect(port, SIGNAL(bytesWritten(qint64)), this, SLOT(dataSent(qint64)));
}

void HwComIO::incomingDataReady()
{
  QByteArray array = port -> readAll() ;

  emit readData(array) ;
}

void HwComIO::dataSent(qint64 numBytesWritten)
{
    SL[0].bytesWritten += numBytesWritten;
    if(SL[0].bytesWritten == SL[0].textToSend.size())
    {
        const int nextIn = SL[0].delayAfterSend;
        SL.pop_front();

        if(!SL.empty())
        {
            QTimer::singleShot(nextIn, this, SLOT(sendData()));
            emit numberChanged(SL.size());
        }
        else
            emit finished();
    }
}

void HwComIO::sendData()
{
    if(!SL.empty())
        port->write(SL[0].textToSend.toUtf8());
}

void HwComIO::enqueueOutgoing(QVector<packetType> sList)
{
    bool startSending = false;
    if(SL.empty())
        startSending = true;

    for(size_t i = 0; i < sList.size(); i++)
      SL.push_back(sList[i]);

    if(startSending)
        sendData();
}
