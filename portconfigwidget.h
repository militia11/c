#ifndef PORTCONFIGWIDGET_H
#define PORTCONFIGWIDGET_H

#include <QWidget>
#include <QString>

#include "slidingstackedwidget.h"

#include <QSerialPort>
struct portConfigType
{
  QString mPortName;
  QSerialPort::BaudRate mBaudRate;
  QSerialPort::DataBits mDataBits;
  QSerialPort::Parity mParity;
  QSerialPort::StopBits mStopBits;
  QSerialPort::FlowControl mFlowControl;
};

namespace Ui
{
  class PortConfigWidget ;
}

class PortConfigWidget : public QWidget
{
  Q_OBJECT

  public:
    explicit PortConfigWidget(QWidget *parent, QStringList SL) ;
    ~PortConfigWidget() ;
    void updateUiToCurConf(QStringList _portConf);
    QStringList portConfigTypeToconfigFile(portConfigType portConf);
    portConfigType configFileToPortConfigType(QStringList confFile);
    bool canceled ;
    QStringList curUiConf;
    QString port ;
    QString parity ;
    QString baudRate ;
    QString dataBits ;
    QString stopBits ;
    QString flowControl ;

  public slots:
    void ok();
    void cancel();

  signals:
    void widgetClosed();

  private:
    Ui::PortConfigWidget *ui;
    SlidingStackedWidget* stackedWidget;
} ;

#endif
