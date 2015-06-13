#include "portconfigwidget.h"
#include "ui_portconfigwidget.h"

PortConfigWidget::PortConfigWidget(QWidget *parent, QStringList SL)
    : QWidget(parent, Qt::WindowTitleHint)
    , ui(new Ui::PortConfigWidget)
    , stackedWidget(qobject_cast<SlidingStackedWidget*>(parent))
{
  ui -> setupUi(this) ;

  ui -> portBox -> addItems(SL) ;

  canceled = false ;

  connect(ui -> okButton, SIGNAL(clicked() ), this, SLOT(ok() ) ) ;
  connect(ui -> cancelButton, SIGNAL(clicked() ), this, SLOT(cancel() ) ) ;
}

PortConfigWidget::~PortConfigWidget()
{
  delete ui ;
}

portConfigType PortConfigWidget::configFileToPortConfigType(QStringList confFile)
{
    portConfigType tempConfig;
    tempConfig.mPortName = confFile[0];
    tempConfig.mBaudRate = (QSerialPort::BaudRate)confFile[1].toInt();

    if (confFile[2] == "Brak")
      tempConfig.mParity = QSerialPort::NoParity;
    else if (confFile[2] == "Odd")
      tempConfig.mParity = QSerialPort::OddParity;
    else if (confFile[2] == "Even")
      tempConfig.mParity = QSerialPort::EvenParity;
    else if (confFile[2] == "Mark")
      tempConfig.mParity = QSerialPort::MarkParity;
    else if (confFile[2] == "Space")
      tempConfig.mParity = QSerialPort::SpaceParity;

    if (confFile[3] == "5")
      tempConfig.mDataBits = QSerialPort::Data5;
    else if (confFile[3] == "6")
      tempConfig.mDataBits = QSerialPort::Data6;
    else if (confFile[3] == "7")
      tempConfig.mDataBits = QSerialPort::Data7;
    else if (confFile[3] == "8")
      tempConfig.mDataBits = QSerialPort::Data8;

    if (confFile[4] == "1")
      tempConfig.mStopBits = QSerialPort::OneStop;
    else if (confFile[4] == "1.5")
      tempConfig.mStopBits = QSerialPort::OneAndHalfStop;
    else if (confFile[4] == "2")
      tempConfig.mStopBits = QSerialPort::TwoStop;

    if (confFile[5] == "Brak")
      tempConfig.mFlowControl = QSerialPort::NoFlowControl;
    else if (confFile[5] == "Hardware")
      tempConfig.mFlowControl = QSerialPort::HardwareControl;
    else if (confFile[5] == "Software")
      tempConfig.mFlowControl = QSerialPort::SoftwareControl;

    return tempConfig;
}

QStringList PortConfigWidget::portConfigTypeToconfigFile(portConfigType portConf)
{
    QStringList abcd;
    abcd.append(portConf.mPortName);
    abcd.append(QString::number(portConf.mBaudRate));

    if (portConf.mParity == QSerialPort::NoParity)
      abcd.append("Brak");
    else if (portConf.mParity == QSerialPort::OddParity)
      abcd.append("Odd");
    else if (portConf.mParity == QSerialPort::EvenParity)
      abcd.append("Even");
    else if (portConf.mParity == QSerialPort::MarkParity)
      abcd.append("Mark");
    else if (portConf.mParity == QSerialPort::SpaceParity)
      abcd.append("Space");

    if (portConf.mDataBits == QSerialPort::Data5)
      abcd.append("5");
    else if (portConf.mDataBits == QSerialPort::Data6)
      abcd.append("6");
    else if (portConf.mDataBits == QSerialPort::Data7)
      abcd.append("7");
    else if (portConf.mDataBits == QSerialPort::Data8)
      abcd.append("8");

    if (portConf.mStopBits == QSerialPort::OneStop)
      abcd.append("1");
    else if (portConf.mStopBits == QSerialPort::OneAndHalfStop)
      abcd.append("1.5");
    else if (portConf.mStopBits == QSerialPort::TwoStop)
      abcd.append("2");

    if (portConf.mFlowControl == QSerialPort::NoFlowControl)
      abcd.append("Brak");
    else if (portConf.mFlowControl == QSerialPort::HardwareControl)
      abcd.append("Hardware");
    else if (portConf.mFlowControl == QSerialPort::SoftwareControl)
      abcd.append("Software");

    return abcd;
}

void PortConfigWidget::updateUiToCurConf(QStringList _portConf)
{
  int portIdx = ui->portBox->findText(_portConf[0], Qt::MatchFixedString);
  ui->portBox->setCurrentIndex(portIdx);
  int baudIdx = ui->baudRateBox->findText(_portConf[1], Qt::MatchFixedString);
  ui->baudRateBox->setCurrentIndex(baudIdx);
  int parityIdx = ui->parityBox->findText(_portConf[2], Qt::MatchFixedString);
  ui->parityBox->setCurrentIndex(parityIdx);
  int dBitsIdx = ui->dataBitsBox->findText(_portConf[3], Qt::MatchFixedString);
  ui->dataBitsBox->setCurrentIndex(dBitsIdx);
  int sBitsIdx = ui->stopBitsBox->findText(_portConf[4], Qt::MatchFixedString);
  ui->stopBitsBox->setCurrentIndex(sBitsIdx);
  int flowIdx = ui->flowControlBox->findText(_portConf[5], Qt::MatchFixedString);
  ui->flowControlBox->setCurrentIndex(flowIdx);
}

void PortConfigWidget::ok()
{
  curUiConf.clear();
  curUiConf.append(ui -> portBox -> currentText());
  curUiConf.append(ui -> baudRateBox -> currentText());
  curUiConf.append(ui -> parityBox -> currentText());
  curUiConf.append(ui -> dataBitsBox -> currentText());
  curUiConf.append(ui -> stopBitsBox -> currentText());
  curUiConf.append(ui -> flowControlBox -> currentText());

  canceled = false;
  stackedWidget->slideInIdx(MENU_WIDGET);
  emit widgetClosed();
}

void PortConfigWidget::cancel()
{
  canceled = true;
  stackedWidget->slideInIdx(MENU_WIDGET);
  emit widgetClosed();
}
