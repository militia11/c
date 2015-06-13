#ifndef HWCOMIO_H
#define HWCOMIO_H

#include <QSerialPort>
#include <QStringList>
#include <QVector>
#include <QObject>

struct packetType
{
    packetType()
        : delayAfterSend(0)
        , textToSend("")
        , bytesWritten(0)
    {

    }

    packetType(QString _textToSend, int _delayAfterSend)
        : delayAfterSend(_delayAfterSend)
        , textToSend(_textToSend)
        , bytesWritten(0)
    {

    }
    int delayAfterSend;
    QString textToSend;

    qint64 bytesWritten;
};

class HwComIO : public QObject
{
  Q_OBJECT

  public:
    explicit HwComIO(QObject *parent, QSerialPort *sp);
    void enqueueOutgoing(QVector<packetType> sList) ;

  signals:
    void numberChanged(int i) ;
    void readData(QByteArray data) ;
    void finished();

  private slots:
    void dataSent(qint64 numBytesWritten);
    void sendData();
    void incomingDataReady();

  private:
    QSerialPort *port ;
    QVector<packetType> SL;
} ;

#endif
