#ifndef DIAGNOSTICDIALOG_H
#define DIAGNOSTICDIALOG_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QStringList>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QIODevice>
#include <QTableWidgetItem>
#include <QString>
#include <QPainter>
#include <QSignalMapper>
#include <QImage>
#include <QtWebKitWidgets/QWebView>
#include <QWebFrame>
#include <QProcess>
#include <QTimer>
#include <vector>

#include "formdialog.h"
#include "hwcomio.h"
#include "portconfigwidget.h"
#include "normdialog.h"
#include "printingformdialog.h"
#include "ownerInfoType.h"
#include "slidingstackedwidget.h"
#include <unistd.h>
#include<time.h>
#include<ctime>
#include <chrono>
#include <thread>
namespace Ui
{
  class DiagnosticWidget ;
}

class DiagnosticWidget : public QWidget
{
  Q_OBJECT

  public:


    void delay();
    explicit DiagnosticWidget(QWidget *parent, QSerialPort *sp) ;
    ~DiagnosticWidget() ;
    void resizeLowerTable();
    bool isNumber(const std::string &kod);
    int actualNumber;

signals:
  void changeNumDriver(int numDriver);

  void changeR1(int value);
  void changeR2(int value);
  void changeR3(int value);
  void changeR4(int value);
  void changeO1(int value);
  void changeO2(int value);
  void changeO3(int value);
  void changeO4(int value);

  public slots:
    void printResult() ;
    void showResult() ;
    void readFromDatabase() ;
    void runDriver() ;
    void stopDriver() ;
    void sendData1ToDriver();
    void sendData2ToDriver();
    void sendData3ToDriver();
    void sendData4ToDriver();
    void sendData5ToDriver();
    void selectProduct(QString company, QString type, QString model);
    void setOwnerInfo(ownerInfoType info);
    void reloadHwLists();

    void changeNumDrivers(int numDriver);

  private slots:
    void closeLoadingDialog() ;
    void upgradeDialog(int i) ;
    void closeWaitingDialog(QByteArray array) ;
    void changeCompany(QString company) ;
    void changeTypeEM() ;
    void changeTypePiezo() ;
    void docReadyToPrint(bool loadingStatus);
    void print(QPrinter *printer) ;


    void on_toolButton_clicked();

protected:
    void resizeEvent(QResizeEvent *event);

  private:
    bool noEntry;
    int sendfirst;
    bool emptyRows, runFlag, sendFlag, end;
    bool numberic(const QString code);
    bool isNumber(const QString &string);
    bool checkValue(QString value, double min, double max);
    Ui::DiagnosticWidget *ui ;
    HwComIO* comIO;
    QSerialPort *port ;
    QStringList SL2 ;
    QStringList SL ;
    QByteArray packet ;
    QList<QByteArray> aList ;
    QWebView* webview;
    SlidingStackedWidget* stackedWidget;
    QSignalMapper* signalMapper;
    ownerInfoType* ownerInfo;
    void sendDataToDriver(QVector<packetType> paramData);
    void fillInTable(int startIdx, QStringList data);
    void clearTableDataContents();
    QStringList mDbFileSL;
    QStringList mResDbFileSL;
    QStringList printRes;
    std::vector<int> tableColumnBaseWidth;
    std::vector<int> tableRowBaseHeight;
    bool r3WorkaroundSendAgain;
    int r3WorkaroundTryNum , chosenRatioNormGroup, post,k;
    QProcess* printProcess;
    double* units;
    double* pass;
    QString f,g,i,j,a,l,t,w,temp;
    QString at, before, number;
    bool wFlag;

} ;

#endif
