#include "diagnosticwidget.h" //master
#include "ui_diagnosticwidget.h"
#include<QPixmap>
#include<QImageIOHandler>
#include<QIODevice>
#include<QImageWriter> //mas
normValuesType curTableValues;
#include<QTime>
void DiagnosticWidget::delay()
{    QTime dieTime = QTime::currentTime().addMSecs(5500);
     while( QTime::currentTime() < dieTime ){
            QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
     }// master
}

DiagnosticWidget::DiagnosticWidget(QWidget *parent, QSerialPort *sp)
    : QWidget(parent, Qt::WindowMinMaxButtonsHint)
    , ui(new Ui::DiagnosticWidget)
    , stackedWidget(qobject_cast<SlidingStackedWidget*>(parent))
    , ownerInfo(new ownerInfoType())
    , r3WorkaroundSendAgain(true)
    , r3WorkaroundTryNum(0)
    , printProcess(new QProcess(this))
{
  ui -> setupUi(this) ;
  ui->f->setStyleSheet("color: #0066FF; font-weight: bold; font-size: 15px;");
  ui->g->setStyleSheet("color: #0066FF; font-weight: bold; font-size: 15px;");
  ui->i->setStyleSheet("color: #0066FF; font-weight: bold; font-size: 15px;");
  ui->j->setStyleSheet("color: #0066FF; font-weight: bold; font-size: 15px;");
  ui->a->setStyleSheet("color: #0066FF; font-weight: bold; font-size: 15px;");
  ui->ll->setStyleSheet("color:#0066FF; font-weight: bold; font-size: 15px;");
  ui->t->setStyleSheet("color: #0066FF; font-weight: bold; font-size: 15px;");
  ui->w->setStyleSheet("color: #0066FF; font-weight: bold; font-size: 15px;");
  ui->rationBar1->setStyleSheet("QProgressBar {border: 2px solid #00CCFF; border-radius: 5px;background-color: #E8E8E8;} QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 cyan,stop: 0.5 #0066FF, stop: 1 cyan); }");
  ui->rationBar2->setStyleSheet("QProgressBar {border: 2px solid #00CCFF; border-radius: 5px;background-color: #E8E8E8;} QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 cyan,stop: 0.5 #0066FF, stop: 1 cyan); }");
  ui->rationBar3->setStyleSheet("QProgressBar {border: 2px solid #00CCFF; border-radius: 5px;background-color: #E8E8E8;} QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 cyan,stop: 0.5 #0066FF, stop: 1 cyan); }");
  ui->rationBar4->setStyleSheet("QProgressBar {border: 2px solid #00CCFF; border-radius: 5px;background-color: #E8E8E8;} QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 cyan,stop: 0.5 #0066FF, stop: 1 cyan); }");
  ui->overflowBar1->setStyleSheet("QProgressBar {border: 2px solid #00CCFF; border-radius: 5px;background-color: #E8E8E8;} QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 cyan,stop: 0.5 #0066FF, stop: 1 cyan); }");
  ui->overflowBar2->setStyleSheet("QProgressBar {border: 2px solid #00CCFF; border-radius: 5px;background-color: #E8E8E8;} QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 cyan,stop: 0.5 #0066FF, stop: 1 cyan); }");
  ui->overflowBar3->setStyleSheet("QProgressBar {border: 2px solid #00CCFF; border-radius: 5px;background-color: #E8E8E8;} QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 cyan,stop: 0.5 #0066FF, stop: 1 cyan); }");
  ui->overflowBar4->setStyleSheet("QProgressBar {border: 2px solid #00CCFF; border-radius: 5px;background-color: #E8E8E8;} QProgressBar::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,stop: 0 cyan,stop: 0.5 #0066FF, stop: 1 cyan); }");

  runFlag=false;
  sendFlag=false;
  ui->notFoundLabel->hide();

  noEntry=true;
  wFlag=false;
  end = false;
  //1st line
  ui->lowerTable->setSpan(0, 1, 1, 2);
  ui->lowerTable->setSpan(0, 3, 1, 8);
  ui->lowerTable->setSpan(0, 11, 1, 2);
  ui->lowerTable->setSpan(0, 13, 1, 8);
  //2nd line
  ui->lowerTable->setSpan(1, 1, 1, 2);
  ui->lowerTable->setSpan(1, 3, 1, 2);
  ui->lowerTable->setSpan(1, 5, 1, 2);
  ui->lowerTable->setSpan(1, 7, 1, 2);
  ui->lowerTable->setSpan(1, 9, 1, 2);
  ui->lowerTable->setSpan(1, 11, 1, 2);
  ui->lowerTable->setSpan(1, 13, 1, 2);
  ui->lowerTable->setSpan(1, 15, 1, 2);
  ui->lowerTable->setSpan(1, 17, 1, 2);
  ui->lowerTable->setSpan(1, 19, 1, 2);

  tableColumnBaseWidth.push_back(120);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);

  tableRowBaseHeight.push_back(25);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);

  for(size_t i = 0; i < tableColumnBaseWidth.size(); i++)
      ui->lowerTable->setColumnWidth(i, tableColumnBaseWidth[i]);
  for(size_t i = 0; i < tableRowBaseHeight.size(); i++)
      ui->lowerTable->setRowHeight(i, tableRowBaseHeight[i]);

  ui -> progressBar -> setVisible(false) ;
  ui -> infoLabel -> setVisible(false) ;

  ui -> pressureLabel -> setStyleSheet("background-color: rgb(0, 255, 0)") ;
  ui -> timeLabel -> setStyleSheet("background-color: rgb(0, 255, 0)") ;
  ui -> frequencyLabel -> setStyleSheet("background-color: rgb(0, 255, 0)") ;
  ui -> counterLabel -> setStyleSheet("background-color: rgb(0, 255, 0)") ;
  ui -> subgroupLabel -> setStyleSheet("background-color: rgb(100, 0, 255)") ;
  webview =new QWebView(this);
  webview->setVisible(false);
  port = sp ;
  comIO =new HwComIO(this, sp);

  w="";
  before="";
  reloadHwLists();

  emptyRows=false;
  actualNumber = 0;
  signalMapper =new QSignalMapper(this);
  connect(ui->closeDialogButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
  signalMapper->setMapping(ui->closeDialogButton, MENU_WIDGET);

  connect(signalMapper, SIGNAL(mapped(int) ), stackedWidget, SLOT(slideInIdx(int) ) ) ;
  connect(ui -> printResultButton, SIGNAL(clicked() ), this, SLOT(printResult() ) ) ;
  connect(ui -> showResultsButton, SIGNAL(clicked() ), this, SLOT(showResult() ) ) ;
  connect(ui -> readFromDatabaseButton, SIGNAL(clicked() ), this, SLOT(readFromDatabase() ) ) ;

  connect(ui -> sendData1Button, SIGNAL(clicked()), this, SLOT(sendData1ToDriver()));
  connect(ui -> sendData2Button, SIGNAL(clicked()), this, SLOT(sendData2ToDriver()));
  connect(ui -> sendData3Button, SIGNAL(clicked()), this, SLOT(sendData3ToDriver()));
  connect(ui -> sendData4Button, SIGNAL(clicked()), this, SLOT(sendData4ToDriver()));
  connect(ui -> sendData5Button, SIGNAL(clicked()), this, SLOT(sendData5ToDriver()));

  connect(ui -> runDriverButton, SIGNAL(clicked() ), this, SLOT(runDriver() ) ) ;
  connect(ui -> stopDriverButton, SIGNAL(clicked() ), this, SLOT(stopDriver() ) ) ;
  connect(ui -> companyBox, SIGNAL(currentIndexChanged(QString) ), this, SLOT(changeCompany(QString) ) ) ;
  connect(ui -> emRadioButton, SIGNAL(clicked() ), this, SLOT(changeTypeEM() ) ) ;
  connect(ui -> piezoRadioButton, SIGNAL(clicked() ), this, SLOT(changeTypePiezo() ) ) ;

  connect(comIO, SIGNAL(finished() ), this, SLOT(closeLoadingDialog() ) ) ;
  connect(comIO, SIGNAL(numberChanged(int) ), this, SLOT(upgradeDialog(int) ) ) ;
  connect(comIO, SIGNAL(readData(QByteArray) ), this, SLOT(closeWaitingDialog(QByteArray) ) ) ;

  QObject::connect(this, SIGNAL(changeNumDriver(int)), this, SLOT(changeNumDrivers(int)));
  QObject::connect(this, SIGNAL(changeNumDriver(int)), this, SLOT(changeNumDrivers(int)));

  QObject::connect(this,SIGNAL(changeR1(int)),ui->rationBar1, SLOT(setValue(int)));
  QObject::connect(this,SIGNAL(changeR2(int)),ui->rationBar2, SLOT(setValue(int)));
  QObject::connect(this,SIGNAL(changeR3(int)),ui->rationBar3, SLOT(setValue(int)));
  QObject::connect(this,SIGNAL(changeR4(int)),ui->rationBar4, SLOT(setValue(int)));

  QObject::connect(this,SIGNAL(changeO1(int)),ui->overflowBar1, SLOT(setValue(int)));
  QObject::connect(this,SIGNAL(changeO2(int)),ui->overflowBar2, SLOT(setValue(int)));
  QObject::connect(this,SIGNAL(changeO3(int)),ui->overflowBar3, SLOT(setValue(int)));
  QObject::connect(this,SIGNAL(changeO4(int)),ui->overflowBar4, SLOT(setValue(int)));
  temp="";
  at="";
  k=1;
  post=0;
  units = new double[4];
  for(int i = 0; i<4;i++)
      units[i]= 0;
  pass = new double[4];
  for(int i = 0; i<4;i++)
      pass[i]= 0;
}
bool DiagnosticWidget::isNumber(const QString &string) {
    return string.toDouble() != 0.0;
}

bool DiagnosticWidget::checkValue(QString value, double min, double max)
{
    double result = value.toDouble();
    if(result < min || result > max)
        return true;
    return false;
}
bool DiagnosticWidget::isNumber(const std::string &kod) {
    std::string::const_iterator it = kod.begin();
    while (it != kod.end() && isdigit(*it)) ++it;

    return !kod.empty() && it == kod.end();
}
DiagnosticWidget::~DiagnosticWidget()
{
  if(ui)
  {
      delete ui;
      ui = nullptr;
  }
  if(ownerInfo)
  {
      delete ownerInfo;
      ownerInfo = nullptr;
  }
}
void DiagnosticWidget::changeNumDrivers(int numDriver)
{
    ui->subgroupLabel->setText( QString::number(numDriver) );
}
void DiagnosticWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeLowerTable();
}

void DiagnosticWidget::resizeLowerTable()
{
    int maxSizeX = ui->lowerTable->width() - 20;//leave some space
    int maxSizeY = ui->lowerTable->height() - 20;
    int sizex = 0;
    for(size_t i = 0; i < tableColumnBaseWidth.size(); i++)
        sizex += tableColumnBaseWidth[i];
    int sizey = 0;
     for(size_t i = 0; i < tableRowBaseHeight.size(); i++)
        sizey += tableRowBaseHeight[i];

     if(maxSizeX < sizex)
         maxSizeX = sizex;
     if(maxSizeY < sizey)
         maxSizeY = sizey;

    const int sizeXLeft =  maxSizeX - sizex;
    const int eachColumnAdd = sizeXLeft / tableColumnBaseWidth.size();
    for(size_t i = 0; i < tableColumnBaseWidth.size(); i++)
       ui->lowerTable->setColumnWidth(i, tableColumnBaseWidth[i] + eachColumnAdd);

    const int sizeYLeft = maxSizeY - sizey;
    const int eachRowAdd = sizeYLeft / tableRowBaseHeight.size();
    for(size_t i = 0; i < tableRowBaseHeight.size(); i++)
       ui->lowerTable->setRowHeight(i, tableRowBaseHeight[i] + eachRowAdd);
}

void DiagnosticWidget::reloadHwLists()
{
    ui -> numbersBox->clear();
    QDir dir ;
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/EM") ;
    SL = dir.entryList(QDir::Files) ;

    if (SL.isEmpty() )
    {
      ui -> readFromDatabaseButton -> setDisabled(true) ;
    }
    else
    {
      for (int i = 0 ; i < SL.size() ; i++)
      {
        SL[i].chop(4) ;
      }

      ui -> numbersBox -> addItems(SL) ;
    }

    SL.clear() ;
}

void DiagnosticWidget::printResult()
{
  PrintingFormDialog pDialog(this, number.isEmpty() ? PrintingFormDialog::NoNumber : PrintingFormDialog::RememberedNumber) ;
  pDialog.exec() ;

  if (pDialog.canceled) return ;

  //QString filepath = QFileDialog::getOpenFileName(this,tr("Otworz Plik"),"C://","Wszystkie pliki(*.*);;Mapy bitowe(*.bmp);;Obrazki JPG(*.jpg)");

  QString strStream;
  QTextStream out(&strStream);

  out << "<html>\n";

  out << "<head>\n";

  out << "<style type=\"text/css\">\n";
  out << "#title {\n";
  out << "position: relative;\n";
  out << "width: 100%;\n";
  out << "top: 0px;\n";
  out << "height: 70px;\n";
  out << "text-align: center;\n";
  out << "font-size: 35px;\n";
  out << "}\n";
  out << "#infoBar {\n";
  out << "height: 240px;\n";
  out << "width: 100%;\n";
  out << "}\n";
  out << "#infoLeft {\n";
  out << "float: left;\n";
  out << "align: left;\n";
  out << "text-align: left;\n";
  out << "font-size: 18px;\n";
  out << "}\n";
  out << "img.infoRightLogo {\n";
  out << "height: 90%;\n";
  out << "align: right;\n";
  out << "float: right;\n";
  out << "}\n";
  out << "#tend {\n";
  out << "position: relative;\n";
  out << "width: 100%;\n";
  out << "top: 0px;\n";
  out << "height: 70px;\n";
  out << "text-align: left;\n";
  out << "font-size: 25px;\n";
  out << "}\n";
  out << "#footer {\n";
  out << "position: relative;\n";
  out << "width: 100%;\n";
//  out << "bottom: 0px;\n";
  out << "height: 70px;\n";
  out << "border-top: 5px solid #000000;\n";
  out << "text-align: left;\n";
  out << "font-size: 23px;\n";
  out << "}\n";
  out << "</style>\n";

  out << "</head>\n";

  out << "<body>\n";

  QStringList mResDbmaxValues;
  double minValue = 0;
  double maxValue = 0;
  {
      out << "<div id=\"title\">\n";
      out << QString("<b>Raport Diagnostyczny</b>\n");
      out << "</div>\n";
  }

  {
      out << "<div id=\"infoBar\">\n";
      {
          out << "<div id=\"infoLeft\">\n";
          out << "<br><br><br>\n";
          out << QString("<b>Imię i nazwisko / nazwa: </b>%1<br>\n").arg(pDialog.name);
          out << QString("<b>Data: </b>%1<br>\n").arg(pDialog.date);
          out << QString("<b>Firma: </b>%1<br>\n").arg(pDialog.company);
          out << QString("<b>Typ: </b>%1<br>\n").arg(pDialog.type);
          if(number.isEmpty())
              out << QString("<b>Numer wtryskiwacza: </b>%1<br>\n").arg(pDialog.number);
          else
              out << QString("<b>Numer wtryskiwacza: </b>%1<br>\n").arg(number);
          out << "</div>\n";
      }
        if(pDialog.filepath != ""){
            QString newLogoPath ("<img class=\"infoRightLogo\" src=");
                newLogoPath += pDialog.filepath;
                newLogoPath += ">\n";
        out << newLogoPath;
        }
        else
             out << "<img class=\"infoRightLogo\" src=\"logosx.png\">\n";
      out << "</div>\n";
  }

  out << "<br><br>\n";
  {
      bool isNG[10];
      for(int i = 0; i < 10; i++){
          isNG[i] = false;
      }
      out << "<div name=\"dawki_przelewy\">\n";
      out << "<table border = 1 cellspacing = 0 align = center style=\"font-size: 20px;\">";

      out << "<tr>" ;
      out << QString("<th width = 160 height = 50 align = center><b>Test</b></th>");
      out << QString("<th width = 115 height = 50 align = center ><b>Norma<br>od&nbsp;-&nbsp;do</b></th>");

      out << QString("<th width = 115 height = 50 align = center ><b>Wtrysk.<br>1</b></th>");
      out << QString("<th width = 115  height = 50 align = center ><b>Wtrysk.<br>2</b></th>");
      out << QString("<th width = 115  height = 50 align = center ><b>Wtrysk.<br>3</b></th>");
      out << QString("<th width = 115  height = 50 align = center ><b>Wtrysk.<br>4</b></th>");
      out << "</tr>" ;

      int tItemCounter = 0;
      int curRowMaxIdx = 0;

      out << "<tr>" ;
      out << QString("<th style=\"background-color: red; \" width = 160 height = 50 align = center><b>Max.moc</b></th>");

      curRowMaxIdx = tItemCounter + 10;

      out << QString("<th width = 115 height = 50 align = center><b>%1 - %2</b></th>").arg(mResDbFileSL[tItemCounter]).arg(mResDbFileSL[tItemCounter+1]);
      minValue = mResDbFileSL[tItemCounter++].toDouble();
      maxValue = mResDbFileSL[tItemCounter++].toDouble();

      for(int frameIndex = 0; tItemCounter < curRowMaxIdx; tItemCounter++){
          if(frameIndex==4){
              isNG[9]=true;
              tItemCounter += 4;
              break;
          }
          if( isNumber( mResDbFileSL[tItemCounter]) ){
              checkValue(mResDbFileSL[tItemCounter], minValue, maxValue) ? out << QString("<th width = 115 height = 50 align = center ><b><font color= #CC0000>%1</font></b></th>").arg(mResDbFileSL[tItemCounter])
                                        : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(mResDbFileSL[tItemCounter]);
          }
          else if(mResDbFileSL[tItemCounter] == "NG")
              isNG[tItemCounter % 10]=true;
          else if(mResDbFileSL[tItemCounter] == ""){
              frameIndex++;
                    out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg("&nbsp;");
          }
      }
      tItemCounter = curRowMaxIdx;
      out << "</tr>" ;

      int curRowIdxMax = tItemCounter + 10;
  {
      if( numberic(mResDbFileSL[tItemCounter]) )
          mResDbmaxValues.append( mResDbFileSL[tItemCounter]);
      else if(mResDbFileSL[tItemCounter] == "NG")
          isNG[tItemCounter % 10]=true;
      else if(mResDbFileSL[tItemCounter] == ""){
              mResDbmaxValues.append(QString("&nbsp;"));
      }
      tItemCounter++;
  }
      {
          if( numberic(mResDbFileSL[tItemCounter]) )
              mResDbmaxValues.append( mResDbFileSL[tItemCounter]);
          else if(mResDbFileSL[tItemCounter] == "NG")
              isNG[tItemCounter % 10]=true;
          else if(mResDbFileSL[tItemCounter] == ""){
                  mResDbmaxValues.append(QString("&nbsp;"));
          }
          tItemCounter++;
      }
      for(int frameIndex=0; tItemCounter < curRowIdxMax; tItemCounter++){
          if(frameIndex==4) {
              tItemCounter += 4;
              break;
          }
          if( isNumber( mResDbFileSL[tItemCounter]) )
              mResDbmaxValues.append( mResDbFileSL[tItemCounter]);
          else if(mResDbFileSL[tItemCounter] == "NG")
              isNG[tItemCounter % 10]=true;
          else if(mResDbFileSL[tItemCounter] == ""){
              frameIndex++;
              mResDbmaxValues.append(QString("&nbsp;"));
          }
      }
      tItemCounter = curRowIdxMax;
      out << "<tr>" ;
      out << QString("<th width = 160 height = 50 align = center><b>Średnia moc</b></th>");
      curRowMaxIdx = tItemCounter + 10;
      out << QString("<th width = 115 height = 50 align = center><b>%1 - %2</b></th>").arg(mResDbFileSL[tItemCounter]).arg(mResDbFileSL[tItemCounter+1]);
      minValue = mResDbFileSL[tItemCounter++].toDouble();
      maxValue = mResDbFileSL[tItemCounter++].toDouble();
      for(int frameIndex = 0; tItemCounter < curRowMaxIdx; tItemCounter++){
          if(frameIndex==4){
              tItemCounter += 4;
              break;
          }
          if( isNumber( mResDbFileSL[tItemCounter] ) ){
              checkValue(mResDbFileSL[tItemCounter], minValue, maxValue) ? out << QString("<th width = 115 height = 50 align = center ><b><font color= #CC0000>%1</font></b></th>").arg(mResDbFileSL[tItemCounter])
                                        : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(mResDbFileSL[tItemCounter]);
          }
          else if(mResDbFileSL[tItemCounter] == "NG")
              isNG[tItemCounter % 10]=true;
          else if(mResDbFileSL[tItemCounter] == ""){
              frameIndex++;
              //if(!(frameIndex%2))
                    out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg("&nbsp;");
          }
      }
      tItemCounter += 10;
      out << "</tr>" ;
      out << "<tr>" ;
      out << QString("<th background-color: #CC0000 width = 160 height = 50 align = center><b>Min. moc</b></th>");
      curRowMaxIdx = tItemCounter + 10;

      out << QString("<th width = 115 height = 50 align = center><b>%1 - %2</b></th>").arg(mResDbFileSL[tItemCounter]).arg(mResDbFileSL[tItemCounter+1]);
      minValue = mResDbFileSL[tItemCounter++].toDouble();
      maxValue = mResDbFileSL[tItemCounter++].toDouble();

      for(int frameIndex = 0; tItemCounter < curRowMaxIdx; tItemCounter++){
          if(frameIndex==4){
              tItemCounter += 4;
              break;
          }
          if( isNumber( mResDbFileSL[tItemCounter] ) ){
              checkValue(mResDbFileSL[tItemCounter], minValue, maxValue) ? out << QString("<th width = 115 height = 50 align = center ><b><font color= #CC0000>%1</font></b></th>").arg(mResDbFileSL[tItemCounter])
                                        : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(mResDbFileSL[tItemCounter]);
          }
          else if(mResDbFileSL[tItemCounter] == "NG")
              isNG[tItemCounter % 10]=true;
          else if(mResDbFileSL[tItemCounter] == ""){
              frameIndex++;
                    out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg("&nbsp;");
          }
      }
      tItemCounter += 10;
      out << "</tr>" ;

      out << "<tr>" ;
      out << QString("<th  width = 160 height = 50 align = center ><b>Przedwtrysk</b></th>");
      curRowMaxIdx = tItemCounter + 10;
      out << QString("<th width = 115 height = 50 align = center ><b>%1 - %2</b></th>").arg(mResDbFileSL[tItemCounter]).arg(mResDbFileSL[tItemCounter+1]);
      minValue = mResDbFileSL[tItemCounter++].toDouble();
      maxValue = mResDbFileSL[tItemCounter++].toDouble();
      for(int frameIndex = 0; tItemCounter < curRowMaxIdx; tItemCounter++){
          if(frameIndex==4){
              tItemCounter += 4;
              break;
          }
          if( isNumber( mResDbFileSL[tItemCounter] ) ){
              checkValue(mResDbFileSL[tItemCounter], minValue, maxValue) ? out << QString("<th width = 115 height = 50 align = center ><b><font color= #CC0000>%1</font></b></th>").arg(mResDbFileSL[tItemCounter])
                                        : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(mResDbFileSL[tItemCounter]);
          }
          else if(mResDbFileSL[tItemCounter] == "NG")
              isNG[tItemCounter % 10]=true;
          else if(mResDbFileSL[tItemCounter] == ""){
              frameIndex++;
                    out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg("&nbsp;");
          }
      }
      tItemCounter += 10;
      out << "</tr>" ;

        out << "<tr>" ;
        out << QString("<th width = 160 height = 50 align = center><b>Szczelność</b></th>");
        tItemCounter += 10;
        curRowMaxIdx = tItemCounter + 10;

        out << QString("<th width = 115 height = 50 align = center><b>%1 - %2</b></th>").arg(mResDbFileSL[tItemCounter]).arg(mResDbFileSL[tItemCounter+1]);
        minValue = mResDbFileSL[tItemCounter++].toDouble();
        maxValue = mResDbFileSL[tItemCounter++].toDouble();
        for(int frameIndex = 0; tItemCounter < curRowMaxIdx; tItemCounter++){
            if(frameIndex==4) {
                tItemCounter += 4;
                break;
            }
            if( isNumber( mResDbFileSL[tItemCounter] ) ){
                checkValue(mResDbFileSL[tItemCounter], minValue, maxValue) ? out << QString("<th width = 115 height = 50 align = center ><b><font color= #CC0000>%1</font></b></th>").arg(mResDbFileSL[tItemCounter])
                                          : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(mResDbFileSL[tItemCounter]);
            }
            else if(mResDbFileSL[tItemCounter] == "NG")
                isNG[tItemCounter % 10]=true;
            else if(mResDbFileSL[tItemCounter] == ""){
                frameIndex++;
                out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg("&nbsp;");
            }
        }
        tItemCounter += 10;
        out << "</tr>" ;

      out << "<tr>" ;
      out << QString("<th width = 160 height = 50 align = center><b>Przelew</b></th>");

      QStringList::iterator it = mResDbmaxValues.begin();
      minValue = (*it).toDouble();
      out << QString("<th width = 115 height = 50 align = center><b>%1 - %2</b></th>").arg(*it).arg(*(it+1));
      it++;
      maxValue = (*it).toDouble();
      it++;
      for(it; it != mResDbmaxValues.end(); ++it ) {
          checkValue((*it), minValue, maxValue) ? out << QString("<th border = 5 width = 115 height = 50 align = center ><b><font color= #CC0000>%1</font></b></th>").arg(*(it))
                             : out << QString("<th border = 5 width = 115 height = 50 align = center ><b>%1</b></th>").arg(*(it));
      }
      tItemCounter += 10;
      out << "</tr>" ;

      out << "<tr>" ;
      out << QString("<th width = 160 height = 50 align = center><b>Wynik</b></th>");
      out << QString("<th width = 115 height = 50 align = center><b>%1</b></th>").arg("&nbsp;");

      isNG[3] ? out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(QString("<font color= #C00000>NG</font>"))
                             : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(QString("<font color= #00CC00>OK</font>"));

      isNG[5] ? out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(QString("<font color= #C00000>NG</font>"))
                             : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(QString("<font color= #00CC00>OK</font>"));

      isNG[7] ? out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(QString("<font color= #C00000>NG</font>"))
                             : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(QString("<font color= #00CC00>OK</font>"));

      isNG[9] ? out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(QString("<font color= #C00000>NG</font>"))
                             : out << QString("<th width = 115 height = 50 align = center ><b>%1</b></th>").arg(QString("<font color= #00CC00>OK</font>"));
      out << "</tr>" ;
      out << "</table>" ;
      out << "</div>\n";
  }
  {
      out << "<div id=\"tend\">\n";
      out << "<br><br>\n";
      out << QString("1.......................................................................................................................\n");
      out << QString("2.......................................................................................................................\n");
      out << QString("3.......................................................................................................................\n");
      out << QString("4.......................................................................................................................\n");
      out << QString("5.......................................................................................................................\n");
      out << QString("6.......................................................................................................................\n");
      out << "<br><br>\n";
      out << "</div>\n";
  }
  {
      out << "<div id=\"footer\">\n";
out << "<br>\n";
      out << "<table border = 0 cellspacing = 0 align = center style=\"font-size: 20px;\">";
      out << "<tr>" ;
      out << QString("<th width = 550 height = 50 align = left ><b>Nazwa&nbsp;Zakładu:&nbsp;</b>%1</th>").arg(ownerInfo->ownerName);
      out << QString("<th width = 300 height = 50 align = left ><b>Telefon:&nbsp;</b>%1</th>").arg(ownerInfo->ownerPhone);
      out << "</tr>" ;
      out << "<tr>" ;
      out << QString("<th width = 550 height = 50 align = left ><b>Ulica:&nbsp;</b>%1\n</b></th>").arg(ownerInfo->ownerStreet);
      out << QString("<th width = 300 height = 50 align = left ><b>Miejscowość:&nbsp;</b>%1\n</th>").arg(ownerInfo->ownerCity);
      out << "</tr>" ;

      out << "</div>\n";
  }
  out << "</body>\n";
  out << "</html>\n";

/*//QT PRINTING BUG - WORKAROUND - NASTY BUT WORKS
  webview->setHtml(strStream);
  connect(webview, SIGNAL(loadFinished(bool)), this, SLOT(docReady

ToPrint(bool)));
*/
  //WORKAROUND

  QString htmlFileName(QDir::currentPath() + "/printsx.html");
  QString htmlLogoFileName(QDir::currentPath() + "/logosx.png");
  QFile htmlFile(htmlFileName);
  //clean
  if(htmlFile.exists())
  {
    htmlFile.setPermissions(QFile::ReadOther | QFile::WriteOther);
    htmlFile.remove();
  }
  QFile htmlLogoFile(htmlLogoFileName);
  if(htmlLogoFile.exists())
  {
    htmlLogoFile.setPermissions(QFile::ReadOther | QFile::WriteOther);
    htmlLogoFile.remove();
  }
  //write files to disk
  htmlFile.open(QFile::Text | QFile::WriteOnly);
  QTextStream str(&htmlFile);
  str << strStream;
  htmlFile.close();

  QFile::copy(":/icons/diagn_report_logo.png", htmlLogoFileName);

  //print
  QString exec("RUNDLL32.exe");
#ifdef Q_OS_WIN
  printProcess->setNativeArguments("MSHTML.DLL,PrintHTML \"" + QDir::toNativeSeparators(htmlFileName)+ "\"");
  printProcess->start(exec);
  printProcess->waitForFinished(-1);
#else
  QStringList args;
  args.append(QString("MSHTML.DLL,PrintHTML"));
  args.append(QDir::toNativeSeparators(htmlFileName));
  printProcess->start(exec, args);
  printProcess->waitForFinished(-1);
#endif

  //clean
  if(htmlFile.exists())
  {
    htmlFile.setPermissions(QFile::ReadOther | QFile::WriteOther);
    htmlFile.remove();
  }
  if(htmlLogoFile.exists())
  {
    htmlLogoFile.setPermissions(QFile::ReadOther | QFile::WriteOther);
    htmlLogoFile.remove();
  }
}
void DiagnosticWidget::docReadyToPrint(bool loadingStatus)
{
  QPrinter printer ;
  QPrintPreviewDialog dialog(&printer, this) ;
  dialog.setWindowTitle(QString("Podgląd wydruku")) ;
  connect(&dialog, SIGNAL(paintRequested(QPrinter*) ), this, SLOT(print(QPrinter*) ) ) ;
  dialog.exec() ;
}

void DiagnosticWidget::print(QPrinter *printer)
{
  webview->print(printer);
}

void DiagnosticWidget::showResult()
{
  int idxToAdd = 0;
  if(!mResDbFileSL.empty())
  {
    idxToAdd = chosenRatioNormGroup * 20;
    curTableValues.rationMin = mResDbFileSL[0 + idxToAdd].toInt();
    curTableValues.rationMax = mResDbFileSL[1 + idxToAdd].toInt();
    curTableValues.rationValue1 = mResDbFileSL[2 + idxToAdd];
    curTableValues.rationValue2 = mResDbFileSL[4 + idxToAdd];
    curTableValues.rationValue3 = mResDbFileSL[6 + idxToAdd];
    curTableValues.rationValue4 = mResDbFileSL[8 + idxToAdd];
    curTableValues.overflowMin = mResDbFileSL[10 + idxToAdd].toInt();
    curTableValues.overflowMax = mResDbFileSL[11 + idxToAdd].toInt();
    curTableValues.overflowValue1 = mResDbFileSL[12 + idxToAdd];
    curTableValues.overflowValue2 = mResDbFileSL[14 + idxToAdd];
    curTableValues.overflowValue3 = mResDbFileSL[16 + idxToAdd];
    curTableValues.overflowValue4 = mResDbFileSL[18 + idxToAdd];
  }
  else
  {
    for(int i = 0; i < 100; i++)
    mResDbFileSL.append(QString(""));
    idxToAdd = chosenRatioNormGroup * 8;
    curTableValues.rationMin = mDbFileSL [4 + idxToAdd].toInt();
    curTableValues.rationMax = mDbFileSL[5 + idxToAdd].toInt();
    curTableValues.overflowMin = mDbFileSL[6 + idxToAdd].toInt();
    curTableValues.overflowMax = mDbFileSL[7 + idxToAdd].toInt();
  }
   // lub chosenRatioNormGroup
  if(actualNumber>1 && actualNumber < 5)
      emptyRows=true;
  else
      emptyRows=false;
  NormDialog dialog(this, curTableValues, emptyRows);
  dialog.setNormsVisible(false);
  dialog.exec() ;

  idxToAdd = chosenRatioNormGroup * 20;
  if(emptyRows){
      mResDbFileSL[0 + idxToAdd] = QString::number(dialog.mData.rationMin);
      mResDbFileSL[1 + idxToAdd] = QString::number(dialog.mData.rationMax);
      mResDbFileSL[2 + idxToAdd] = dialog.mData.rationValue1;
      mResDbFileSL[4 + idxToAdd] = dialog.mData.rationValue2;
      mResDbFileSL[6 + idxToAdd] = dialog.mData.rationValue3;
      mResDbFileSL[8 + idxToAdd] = dialog.mData.rationValue4;
      mResDbFileSL[10 + idxToAdd] = QString::number(dialog.mData.overflowMin);
      mResDbFileSL[11 + idxToAdd] = QString::number(dialog.mData.overflowMax);
      mResDbFileSL[12 + idxToAdd] = "";
      mResDbFileSL[14 + idxToAdd] = "";
      mResDbFileSL[16 + idxToAdd] = "";
      mResDbFileSL[18 + idxToAdd] = "";
  }
  else{
  mResDbFileSL[0 + idxToAdd] = QString::number(dialog.mData.rationMin);
  mResDbFileSL[1 + idxToAdd] = QString::number(dialog.mData.rationMax);
  mResDbFileSL[2 + idxToAdd] = dialog.mData.rationValue1;
  mResDbFileSL[4 + idxToAdd] = dialog.mData.rationValue2;
  mResDbFileSL[6 + idxToAdd] = dialog.mData.rationValue3;
  mResDbFileSL[8 + idxToAdd] = dialog.mData.rationValue4;
  mResDbFileSL[10 + idxToAdd] = QString::number(dialog.mData.overflowMin);
  mResDbFileSL[11 + idxToAdd] = QString::number(dialog.mData.overflowMax);
  mResDbFileSL[12 + idxToAdd] = dialog.mData.overflowValue1;
  mResDbFileSL[14 + idxToAdd] = dialog.mData.overflowValue2;
  mResDbFileSL[16 + idxToAdd] = dialog.mData.overflowValue3;
  mResDbFileSL[18 + idxToAdd] = dialog.mData.overflowValue4;
}
  ui->f->setText(dialog.mData.rationValue1);
  ui->g->setText(dialog.mData.rationValue2);
  ui->i->setText(dialog.mData.rationValue3);
  ui->j->setText(dialog.mData.rationValue4);

  ui->a->setText(dialog.mData.overflowValue1);
  ui->ll->setText(dialog.mData.overflowValue2);
  ui->t->setText(dialog.mData.overflowValue3);
  ui->w->setText(dialog.mData.overflowValue4);

  curTableValues.rationValue1=dialog.mData.rationValue1;
  curTableValues.rationValue2=dialog.mData.rationValue2;
  curTableValues.rationValue3=dialog.mData.rationValue3;
  curTableValues.rationValue4=dialog.mData.rationValue4;

  curTableValues.overflowValue1=dialog.mData.overflowValue1;
  curTableValues.overflowValue2=dialog.mData.overflowValue2;
  curTableValues.overflowValue3=dialog.mData.overflowValue3;
  curTableValues.overflowValue4=dialog.mData.overflowValue4;

  emit changeR1(dialog.mData.rationValue1.toDouble());
  emit changeR2(dialog.mData.rationValue2.toDouble());
  emit changeR3(dialog.mData.rationValue3.toDouble());
  emit changeR4(dialog.mData.rationValue4.toDouble());

  emit changeO1(dialog.mData.overflowValue1.toDouble());
  emit changeO2(dialog.mData.overflowValue2.toDouble());
  emit changeO3(dialog.mData.overflowValue3.toDouble());
  emit changeO4(dialog.mData.overflowValue4.toDouble());

  //recalc if results(changed) are between norms
  auto recalcResult = [&](int minNormIdx, int maxNormIdx
          , int resultIdx, int testIdx)
  {
      if(mResDbFileSL[resultIdx].toDouble() >= mResDbFileSL[minNormIdx].toDouble()
              && mResDbFileSL[resultIdx].toDouble() <= mResDbFileSL[maxNormIdx].toDouble() )
          mResDbFileSL[testIdx] = "OK";
      else
          mResDbFileSL[testIdx] = "NG";
  };
  if(emptyRows){
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 2 + idxToAdd, 3 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 4 + idxToAdd, 5 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 6 + idxToAdd, 7 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 8 + idxToAdd, 9 + idxToAdd);
  }
  else{
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 2 + idxToAdd, 3 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 4 + idxToAdd, 5 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 6 + idxToAdd, 7 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 8 + idxToAdd, 9 + idxToAdd);
  recalcResult(10 + idxToAdd, 11 + idxToAdd, 12 + idxToAdd, 13 + idxToAdd);
  recalcResult(10 + idxToAdd, 11 + idxToAdd, 14 + idxToAdd, 15 + idxToAdd);
  recalcResult(10 + idxToAdd, 11 + idxToAdd, 16 + idxToAdd, 17 + idxToAdd);
  recalcResult(10 + idxToAdd, 11 + idxToAdd, 18 + idxToAdd, 19 + idxToAdd);
  }
  clearTableDataContents();
  fillInTable(0, mResDbFileSL);//update table

  {//save file
      const QString tempProdType =
              ui->emRadioButton->isChecked() ? "/EM/" : "/Piezo/";
      QFile file;
      file.setFileName(QDir::currentPath() + "/DatabaseSx/"
                       + ui -> companyBox -> currentText() + tempProdType
                       + "Results/" + ui -> numbersBox -> currentText() + ".txt") ;
      file.open(QFile::WriteOnly | QFile::Text) ;
      QTextStream in(&file) ;
      for(int i = 0; i < 100; i++)
      {
        in << mResDbFileSL[i];
        if(i != 99)//last line dont add newline
          in << '\n' ;
      }
      file.close() ;
  }

  ui -> printResultButton -> setEnabled(true) ;
}

void DiagnosticWidget::selectProduct(QString company, QString type, QString model)
{
    ui->companyBox->setCurrentText(company);
    type == "EM" ? ui -> emRadioButton->setChecked(true)
               : ui->piezoRadioButton->setChecked(true);
    ui->numbersBox->setCurrentText(model);
    ui->readFromDatabaseButton->click();
}

void DiagnosticWidget::clearTableDataContents()
{
  for(int i = 2; i <= 6; i++)
  {
    for(int j = 1; j <= 20; j++)
    {
      QTableWidgetItem* tItem = ui->lowerTable->item(i, j);
      if(tItem)
        tItem->setText("");
    }
  }
}

void DiagnosticWidget::fillInTable(int startIdx, QStringList data)
{
    auto safeSetTableText = [&](int _row, int _col, QString _text)
    {
        QTableWidgetItem* tItem = ui->lowerTable->item(_row, _col);
        if(tItem == nullptr)
        {
            tItem =new QTableWidgetItem();
            ui->lowerTable->setItem(_row, _col, tItem);
        }
        tItem->setText(_text);
        tItem->setTextAlignment(Qt::AlignCenter);
        if(_text == "OK")
            tItem->setTextColor(Qt::darkGreen);
        else if(_text == "NG")
            tItem->setTextColor(Qt::red);
    };

    int itemCounter = startIdx;
    for(int i = 2; i <= 6; i++)
    {
      for(int j = 1; j <= 20; j++)
      {
          safeSetTableText(i, j, data[itemCounter]);
          itemCounter++;
      }
    }
}

void DiagnosticWidget::readFromDatabase()
{
  mDbFileSL.clear();
  mResDbFileSL.clear();
  QFile dbFile;
  QFile dbResFile;

  const QString tempProdType =
          ui->emRadioButton->isChecked() ? "/EM/" : "/Piezo/";
  dbFile.setFileName(QDir::currentPath() + "/DatabaseSx/"
                     + ui -> companyBox -> currentText() + tempProdType
                     + ui -> numbersBox -> currentText() + ".txt") ;
  dbResFile.setFileName(QDir::currentPath() + "/DatabaseSx/"
                     + ui -> companyBox -> currentText() + tempProdType
                     + "Results/" + ui -> numbersBox -> currentText() + ".txt") ;

  if(!dbFile.exists())
  {
      if(!ui->notFoundLabel->isVisible())
        ui->notFoundLabel->show();
      return;
  }
  else
  {
      if(ui->notFoundLabel->isVisible())
          ui->notFoundLabel->hide();
  }

  {//read from file
      dbFile.open(QFile::Text | QFile::ReadOnly) ;
      QTextStream str(&dbFile) ;
      for (int i = 0 ; i < 40 ; i++)
        mDbFileSL.append(str.readLine() ) ;
      dbFile.close() ;
  }

  ui -> editl1u1 -> setText(mDbFileSL[0]) ;
  ui -> editl1u2 -> setText(mDbFileSL[1]) ;
  ui -> editl1u3 -> setText(mDbFileSL[2]) ;
  ui -> editl1u4 -> setText(mDbFileSL[3]) ;

  ui -> editl2u1 -> setText(mDbFileSL[8]) ;
  ui -> editl2u2 -> setText(mDbFileSL[9]) ;
  ui -> editl2u3 -> setText(mDbFileSL[10]) ;
  ui -> editl2u4 -> setText(mDbFileSL[11]) ;

  ui -> editl3u1 -> setText(mDbFileSL[16]) ;
  ui -> editl3u2 -> setText(mDbFileSL[17]) ;
  ui -> editl3u3 -> setText(mDbFileSL[18]) ;
  ui -> editl3u4 -> setText(mDbFileSL[19]) ;

  ui -> editl4u1 -> setText(mDbFileSL[24]) ;
  ui -> editl4u2 -> setText(mDbFileSL[25]) ;
  ui -> editl4u3 -> setText(mDbFileSL[26]) ;
  ui -> editl4u4 -> setText(mDbFileSL[27]) ;

  ui -> editl5u1 -> setText(mDbFileSL[32]) ;
  ui -> editl5u2 -> setText(mDbFileSL[33]) ;
  ui -> editl5u3 -> setText(mDbFileSL[34]) ;
  ui -> editl5u4 -> setText(mDbFileSL[35]) ;

  if(dbResFile.exists())
    dbResFile.remove();

  for(int i = 0; i < 100; i++)
    mResDbFileSL.append(QString(""));

  int idxToAdd = 0;
  mResDbFileSL[0 + idxToAdd] = mDbFileSL[4];
  mResDbFileSL[1 + idxToAdd] = mDbFileSL[5];
  mResDbFileSL[10 + idxToAdd] = mDbFileSL[6];
  mResDbFileSL[11 + idxToAdd] = mDbFileSL[7];

  idxToAdd += 20;
  mResDbFileSL[0 + idxToAdd] = mDbFileSL[12];
  mResDbFileSL[1 + idxToAdd] = mDbFileSL[13];
  mResDbFileSL[10 + idxToAdd] = mDbFileSL[14];
  mResDbFileSL[11 + idxToAdd] = mDbFileSL[15];

  idxToAdd += 20;
  mResDbFileSL[0 + idxToAdd] = mDbFileSL[20];
  mResDbFileSL[1 + idxToAdd] = mDbFileSL[21];
  mResDbFileSL[10 + idxToAdd] = mDbFileSL[22];
  mResDbFileSL[11 + idxToAdd] = mDbFileSL[23];

  idxToAdd += 20;
  mResDbFileSL[0 + idxToAdd] = mDbFileSL[28];
  mResDbFileSL[1 + idxToAdd] = mDbFileSL[29];
  mResDbFileSL[10 + idxToAdd] = mDbFileSL[30];
  mResDbFileSL[11 + idxToAdd] = mDbFileSL[31];

  idxToAdd += 20;
  mResDbFileSL[0 + idxToAdd] = mDbFileSL[36];
  mResDbFileSL[1 + idxToAdd] = mDbFileSL[37];
  mResDbFileSL[10 + idxToAdd] = mDbFileSL[38];
  mResDbFileSL[11 + idxToAdd] = mDbFileSL[39];

  clearTableDataContents();
  fillInTable(0, mResDbFileSL);
}

void DiagnosticWidget::runDriver()
{
  ui -> tabWidget -> setCurrentIndex(1) ;
  ui -> progressBar -> setVisible(true) ;
  ui -> infoLabel -> setVisible(true) ;
  ui -> progressBar -> setRange(0, 0) ;

  r3WorkaroundSendAgain = true;// m
  r3WorkaroundTryNum = 0;
  QVector<packetType> tmp{packetType("r3", 100)};
  comIO->enqueueOutgoing(tmp);

  QVector<packetType> tmpx{packetType("p", 100)};
  comIO->enqueueOutgoing(tmpx);

  packet = "" ;

  ui -> runDriverButton -> setDisabled(true) ;
  ui -> stopDriverButton -> setEnabled(true) ;

  ui -> infoLabel -> setText("Oczekiwanie na reakcję ze strony sterownika...") ;
}

void DiagnosticWidget::stopDriver()
{
  runFlag=false;
  QVector<packetType> tmp{packetType("e", 100)};
  comIO->enqueueOutgoing(tmp);

  ui -> runDriverButton -> setDisabled(true) ;
  ui -> stopDriverButton -> setDisabled(true) ;

  ui -> progressBar -> setVisible(false) ;
  ui -> infoLabel -> setText("") ;
  ui -> subgroupLabel -> setText(QString::number(actualNumber)) ;
  ui -> pressureLabel -> setText("0 bar") ;
  ui -> timeLabel -> setText("0 µs") ;
  ui -> frequencyLabel -> setText("0 Hz") ;
  ui -> counterLabel -> setText("0 cykli") ;

  ui->sendData1Button->setEnabled(true);
  ui->sendData2Button->setEnabled(true);
  ui->sendData3Button->setEnabled(true);
  ui->sendData4Button->setEnabled(true);
  ui->sendData5Button->setEnabled(true);
}

void DiagnosticWidget::sendDataToDriver(QVector<packetType> paramData)
{
    ui->rationBar1->setValue(0);
    ui->rationBar2->setValue(0);
    ui->rationBar3->setValue(0);
    ui->rationBar4->setValue(0);
    ui->overflowBar1->setValue(0);
    ui->overflowBar2->setValue(0);
    ui->overflowBar3->setValue(0);
    ui->overflowBar4->setValue(0);
    ui->f->setText("");
    ui->g->setText("");
    ui->i->setText("");
    ui->j->setText("");
    ui->a->setText("");
    ui->ll->setText("");
    ui->t->setText("");
    ui->w->setText("");
    units[0]=0;
    units[1]=0;
    units[2]=0;
    units[3]=0;
    pass[0]=0;
    pass[1]=0;
    pass[2]=0;
    pass[3]=0;

curTableValues.rationValue1 = "";
curTableValues.rationValue2 = "";
curTableValues.rationValue3 = "";
curTableValues.rationValue4 = "";

curTableValues.overflowValue1 = "";
curTableValues.overflowValue2 = "";
curTableValues.overflowValue3 = "";
curTableValues.overflowValue4 = "";
    sendFlag=false;
    if (ui -> numbersBox -> count() != 0)
      number = ui -> numbersBox -> currentText() ;

    QVector<packetType> dataPacket;
    dataPacket.push_back(packetType("o", 125));
    for(size_t i = 0; i < paramData.size(); i++)
        dataPacket.push_back(paramData[i]);//n1

    for(int i = 2; i <= 14; i++)
    {//n2-14
        dataPacket.push_back(packetType("n" + QString::number(i), 75));
        dataPacket.push_back(packetType("b0", 75));
        dataPacket.push_back(packetType("u0", 75));
        dataPacket.push_back(packetType("h0", 75));
        dataPacket.push_back(packetType("c0", 125));
    }

    dataPacket.push_back(packetType("o", 125));

    if(ui->piezoRadioButton->isChecked() == true)
        dataPacket.push_back(packetType("p", 25));

    if (!port -> isOpen() )
    {
      if (!port -> open(QIODevice::ReadWrite) )
      {
        QString msg = QString("[Port]:%1 - Nie udało się połączyć się ze sterownikiem. Sprawdź ustawienia i spróbuj ponownie. err:%2").arg(port->portName(), port->errorString());
        QMessageBox::warning(this, "Błąd", msg);
        return ;
      }
    }

  ui -> progressBar -> setMaximum(dataPacket.size());
  ui->progressBar->setValue(0);

//  for(int i = 0; i < dataPacket.size(); i++)
//      QMessageBox::information(this, QString::number(i), dataPacket[i]);
  comIO -> enqueueOutgoing(dataPacket) ;

  ui -> progressBar -> setVisible(true) ;
  ui -> infoLabel -> setVisible(true) ;
  ui -> infoLabel -> setText("Przesyłanie danych. Proszę czekać...") ;
  ui->sendData1Button->setDisabled(true);
  ui->sendData2Button->setDisabled(true);
  ui->sendData3Button->setDisabled(true);
  ui->sendData4Button->setDisabled(true);
  ui->sendData5Button->setDisabled(true);
}

void DiagnosticWidget::sendData1ToDriver()
{
    ui->showResultsButton->setEnabled(true);
    chosenRatioNormGroup = 0;
    QVector<packetType> tmp;
    tmp.push_back(packetType("n1", 75));
    tmp.push_back(packetType("b" + ui->editl1u1->text(), 75));
    tmp.push_back(packetType("u" + ui->editl1u2->text(), 75));
    tmp.push_back(packetType("h" + ui->editl1u3->text(), 75));
    tmp.push_back(packetType("c" + ui->editl1u4->text(), 125));

    sendDataToDriver(tmp);
    actualNumber = 1;
    emit changeNumDriver(actualNumber);
}

void DiagnosticWidget::sendData2ToDriver()
{
    ui->showResultsButton->setEnabled(true);
    chosenRatioNormGroup = 1;
    QVector<packetType> tmp;
    tmp.push_back(packetType("n1", 75));
    tmp.push_back(packetType("b" + ui->editl2u1->text(), 75));
    tmp.push_back(packetType("u" + ui->editl2u2->text(), 75));
    tmp.push_back(packetType("h" + ui->editl2u3->text(), 75));
    tmp.push_back(packetType("c" + ui->editl2u4->text(), 125));

    sendDataToDriver(tmp);

    actualNumber = 2;
    emit changeNumDriver(actualNumber);
}

void DiagnosticWidget::sendData3ToDriver()
{
    ui->showResultsButton->setEnabled(true);
    chosenRatioNormGroup = 2;
    QVector<packetType> tmp;
    tmp.push_back(packetType("n1", 75));
    tmp.push_back(packetType("b" + ui->editl3u1->text(), 75));
    tmp.push_back(packetType("u" + ui->editl3u2->text(), 75));
    tmp.push_back(packetType("h" + ui->editl3u3->text(), 75));
    tmp.push_back(packetType("c" + ui->editl3u4->text(), 125));

    sendDataToDriver(tmp);

    actualNumber = 3;
    emit changeNumDriver(actualNumber);
}

void DiagnosticWidget::sendData4ToDriver()
{
    ui->showResultsButton->setEnabled(true);
    chosenRatioNormGroup = 3;
    QVector<packetType> tmp;
    tmp.push_back(packetType("n1", 75));
    tmp.push_back(packetType("b" + ui->editl4u1->text(), 75));
    tmp.push_back(packetType("u" + ui->editl4u2->text(), 75));
    tmp.push_back(packetType("h" + ui->editl4u3->text(), 75));
    tmp.push_back(packetType("c" + ui->editl4u4->text(), 125));

    sendDataToDriver(tmp);

    actualNumber = 4;
    emit changeNumDriver(actualNumber);
}

void DiagnosticWidget::sendData5ToDriver()
{
    ui->showResultsButton->setEnabled(true);
    chosenRatioNormGroup = 4;
    QVector<packetType> tmp;
    tmp.push_back(packetType("n1", 75));
    tmp.push_back(packetType("b" + ui->editl5u1->text(), 75));
    tmp.push_back(packetType("u" + ui->editl5u2->text(), 75));
    tmp.push_back(packetType("h" + ui->editl5u3->text(), 75));
    tmp.push_back(packetType("c" + ui->editl5u4->text(), 125));

    sendDataToDriver(tmp);
    actualNumber = 5;
    emit changeNumDriver(actualNumber);
}

void DiagnosticWidget::closeLoadingDialog()
{
  ui -> progressBar -> setVisible(false) ;
  ui -> infoLabel -> setVisible(false) ;
  ui -> infoLabel -> setText("") ;

  ui->sendData1Button->setDisabled(true);
  ui->sendData2Button->setDisabled(true);
  ui->sendData3Button->setDisabled(true);
  ui->sendData4Button->setDisabled(true);
  ui->sendData5Button->setDisabled(true);
  ui -> runDriverButton -> setEnabled(true) ;
  sendFlag = true;
}

void DiagnosticWidget::upgradeDialog(int i)
{
  ui -> progressBar -> setMaximum(i) ;
  if(i > 1)
      ui->progressBar->setValue(1);
  else
      ui->progressBar->setValue(0);
}

void DiagnosticWidget::closeWaitingDialog(QByteArray array)
{
  packet += array ;

  int idxToAdd = chosenRatioNormGroup * 20;

if (packet.contains("1") )
  {
    r3WorkaroundSendAgain = false;//its ok hw started
    if(ui->subgroupLabel->text().length() != 2){
        ui -> subgroupLabel -> setText(QString::number(actualNumber) +"P" ) ;
    }

    int pos = packet.indexOf("1");
    packet.remove(pos,1);
    const int idxToAdd = chosenRatioNormGroup * 8;
    ui -> pressureLabel -> setText(mDbFileSL[0 + idxToAdd] + " bar") ;
    ui -> timeLabel -> setText(mDbFileSL[1 + idxToAdd] + " µs") ;
    ui -> frequencyLabel -> setText(mDbFileSL[2 + idxToAdd] + " Hz") ;
    ui -> counterLabel -> setText(mDbFileSL[3 + idxToAdd] + " cykli") ;
    ui -> infoLabel -> setText("Aktualnie wykonywana podgrupa: 1") ;


  if(r3WorkaroundSendAgain//seems like hw didnt started - try again
          && r3WorkaroundTryNum < 1)
  {
      r3WorkaroundTryNum ++;
      QVector<packetType> tmp{packetType("r3", 100)};
      comIO->enqueueOutgoing(tmp);
  }
  else
  {
      ui -> progressBar -> setVisible(false) ;
      ui -> infoLabel -> setVisible(false) ;
      ui -> showResultsButton -> setEnabled(true);
      ui -> sendData1Button -> setEnabled(true);
      ui -> sendData2Button -> setEnabled(true);
      ui -> sendData3Button -> setEnabled(true);
      ui -> sendData4Button -> setEnabled(true);
      ui -> sendData5Button -> setEnabled(true);
      ui -> runDriverButton -> setDisabled(true) ;
      ui -> stopDriverButton -> setDisabled(true) ;

      ui -> runDriverButton -> setEnabled(true) ;\
      ui -> stopDriverButton -> setEnabled(true) ;
      ui -> subgroupLabel -> setText("") ;
      noEntry=true;
      ui -> pressureLabel -> setText("0 bar") ;
      ui -> timeLabel -> setText("0 µs") ;
      ui -> frequencyLabel -> setText("0 Hz") ;
      ui -> counterLabel -> setText("0 cykli") ;

      ui -> infoLabel -> setText("") ;
  }
}
  if (packet.contains("\n") || packet.contains("k") )
    {
      packet = packet.simplified() ;
      if (packet.contains("k") )
      {
          noEntry=true;
          int pos = packet.indexOf("k");
          packet.remove(pos,1);
          wFlag=false;
          ///
      }
      ///



      /////

//      else if(packet.contains("2")){
//          noEntry=true;
//          int pos = packet.indexOf("2");
//          packet.remove(pos,1);
//      }
    }

if(noEntry==false){
  if(packet.contains("f")){
   w.clear();
      int p = packet.indexOf("f");
      QString check;
      int i = p-1;
      do{
          w.push_front(packet.at(i));
          packet.remove(i,1);
          i--;
      }
      while(numberic(check.append(packet.at(i))));
      w.insert(w.length()-1,".");
      pass[3]= w.toDouble();
      mResDbFileSL[18 + idxToAdd] = w;
      curTableValues.overflowValue4 = w;
      ui->w->setText(QString::number(pass[3]));
      emit changeO4(pass[3]);
      int pos = packet.indexOf("f");
      packet.remove(pos,1);
   int post = packet.indexOf("f");
   packet.remove(post,1);
 }
 if(packet.contains("g")){
     f.clear();
     int p = packet.indexOf("g");
     QString check;
     int i = p-1;
     do{
         f.push_front(packet.at(i));
         packet.remove(i,1);

         i--;
     }
     while(numberic(check.append(packet.at(i))));
     f.insert(f.length()-1,".");

     units[0]= f.toDouble();
     mResDbFileSL[2 + idxToAdd] = f;
     curTableValues.rationValue1 = f;
     ui->f->setText(QString::number(units[0]));
     emit changeR1(units[0]);

     int pos = packet.indexOf("g");
     packet.remove(pos,1);
  }
  if(packet.contains("i")){
      g.clear();
      int p = packet.indexOf("i");
      QString check;
      int i = p-1;
      do{
          g.push_front(packet.at(i));
          packet.remove(i,1);
          i--;
      }
      while(numberic(check.append(packet.at(i))));
      g.insert(g.length()-1,".");
      units[1]= g.toDouble();
      mResDbFileSL[4 + idxToAdd] = g;
      curTableValues.rationValue2 = g;
      ui->g->setText(QString::number(units[1]));
      emit changeR2(units[1]);
      int pos = packet.indexOf("i");
      packet.remove(pos,1);
}
  if(packet.contains("j")){
      i.clear();
      int p = packet.indexOf("j");
      QString check;
      int is = p-1;
      do{
          i.push_front(packet.at(is));
          packet.remove(is,1);
          is--;
      }
      while(numberic(check.append(packet.at(is))));
      i.insert(i.length()-1,".");
      units[2]= i.toDouble();
      mResDbFileSL[6 + idxToAdd] = i;
      curTableValues.rationValue3 = i;
      ui->i->setText(QString::number(units[2]));
      emit changeR3(units[2]);
      int pos = packet.indexOf("j");
      packet.remove(pos,1);

  }
  if(packet.contains("a")){
      j.clear();
      int p = packet.indexOf("a");
      QString check;
      int i = p-1;
      do{
          j.push_front(packet.at(i));
          packet.remove(i,1);
          i--;
      }
      while(numberic(check.append(packet.at(i))));
      j.insert(j.length()-1,".");
      units[3]= j.toDouble();
      mResDbFileSL[8 + idxToAdd] = j;
      curTableValues.rationValue4 = j;
      ui->j->setText(QString::number(units[3]));
      emit changeR4(units[3]);
      int pos = packet.indexOf("a");
      packet.remove(pos,1);
  }
  if(packet.contains("l")){
      a.clear();
      int p = packet.indexOf("l");
      QString check;
      int i = p-1;
      do{
          a.push_front(packet.at(i));
          packet.remove(i,1);
          i--;
      }
      while(numberic(check.append(packet.at(i))));
      a.insert(a.length()-1,".");

      pass[0]= a.toDouble();
      mResDbFileSL[12 + idxToAdd] = a;
      curTableValues.overflowValue1 = a;
      ui->a->setText(QString::number(pass[0]));
      emit changeO1(pass[0]);
      int pos = packet.indexOf("l");
      packet.remove(pos,1);
  }
  if(packet.contains("t")){
      l.clear();
      int p = packet.indexOf("t");
      QString check;
      int i = p-1;
      do{
          l.push_front(packet.at(i));
          packet.remove(i,1);
          i--;
      }
      while(numberic(check.append(packet.at(i))));

      l.insert(l.length()-1,".");
      pass[1]= l.toDouble();
      mResDbFileSL[14 + idxToAdd] = l;
      curTableValues.overflowValue2 = l;
      ui->ll->setText(QString::number(pass[1]));
      emit changeO2(pass[1]);
      int pos = packet.indexOf("t");
      packet.remove(pos,1);
  }
  if(packet.contains("w")){
      t.clear();
      wFlag=true; // wyzerować po sygnale k
      int p = packet.indexOf("w");
      QString check;
      int i = p-1;
      do{
          t.push_front(packet.at(i));
          packet.remove(i,1);
          i--;
      }
      while(numberic(check.append(packet.at(i))));

      t.insert(t.length()-1,".");
      int pos = packet.indexOf("w");
      pass[2]= t.toDouble();
      mResDbFileSL[16 + idxToAdd] = t;
      curTableValues.overflowValue3 = t;
      ui->t->setText(QString::number(pass[2]));
      emit changeO3(pass[2]);
      packet.remove(pos,1);
  }
      curTableValues.rationMin = mResDbFileSL[0 + idxToAdd].toDouble();
      curTableValues.rationMax = mResDbFileSL[1 + idxToAdd].toDouble();
      curTableValues.overflowMin = mResDbFileSL[10 + idxToAdd].toDouble();
      curTableValues.overflowMax = mResDbFileSL[11 + idxToAdd].toDouble();
      idxToAdd = chosenRatioNormGroup * 20;
      if(actualNumber>1 && actualNumber < 5){
          mResDbFileSL[0 + idxToAdd] = QString::number(curTableValues.rationMin);
          mResDbFileSL[1 + idxToAdd] = QString::number(curTableValues.rationMax);
          mResDbFileSL[2 + idxToAdd] = curTableValues.rationValue1;
          mResDbFileSL[4 + idxToAdd] = curTableValues.rationValue2;
          mResDbFileSL[6 + idxToAdd] = curTableValues.rationValue3;
          mResDbFileSL[8 + idxToAdd] = curTableValues.rationValue4;
          mResDbFileSL[10 + idxToAdd] = QString::number(curTableValues.overflowMin);
          mResDbFileSL[11 + idxToAdd] = QString::number(curTableValues.overflowMax);
          mResDbFileSL[12 + idxToAdd] = "";
          mResDbFileSL[14 + idxToAdd] = "";
          mResDbFileSL[16 + idxToAdd] = "";
          mResDbFileSL[18 + idxToAdd] = "";
      }
      else{
          mResDbFileSL[0 + idxToAdd] = QString::number(curTableValues.rationMin);
          mResDbFileSL[1 + idxToAdd] = QString::number(curTableValues.rationMax);
          mResDbFileSL[2 + idxToAdd] = curTableValues.rationValue1;
          mResDbFileSL[4 + idxToAdd] = curTableValues.rationValue2;
          mResDbFileSL[6 + idxToAdd] = curTableValues.rationValue3;
          mResDbFileSL[8 + idxToAdd] = curTableValues.rationValue4;
          mResDbFileSL[10 + idxToAdd] = QString::number(curTableValues.overflowMin);
          mResDbFileSL[11 + idxToAdd] = QString::number(curTableValues.overflowMax);
          mResDbFileSL[12 + idxToAdd] = curTableValues.overflowValue1;
          mResDbFileSL[14 + idxToAdd] = curTableValues.overflowValue2;
          mResDbFileSL[16 + idxToAdd] = curTableValues.overflowValue3;
          mResDbFileSL[18 + idxToAdd] = curTableValues.overflowValue4;
      }
      auto recalcResult = [&](int minNormIdx, int maxNormIdx
              , int resultIdx, int testIdx)
      {
          if(mResDbFileSL[resultIdx].toDouble() >= mResDbFileSL[minNormIdx].toDouble()
                  && mResDbFileSL[resultIdx].toDouble() <= mResDbFileSL[maxNormIdx].toDouble() )
              mResDbFileSL[testIdx] = "OK";
          else
              mResDbFileSL[testIdx] = "NG";
      };
      if(actualNumber>1 && actualNumber < 5){
          recalcResult(0 + idxToAdd, 1 + idxToAdd, 2 + idxToAdd, 3 + idxToAdd);
          recalcResult(0 + idxToAdd, 1 + idxToAdd, 4 + idxToAdd, 5 + idxToAdd);
          recalcResult(0 + idxToAdd, 1 + idxToAdd, 6 + idxToAdd, 7 + idxToAdd);
          recalcResult(0 + idxToAdd, 1 + idxToAdd, 8 + idxToAdd, 9 + idxToAdd);
      }
      else{
          recalcResult(0 + idxToAdd, 1 + idxToAdd, 2 + idxToAdd, 3 + idxToAdd);
          recalcResult(0 + idxToAdd, 1 + idxToAdd, 4 + idxToAdd, 5 + idxToAdd);
          recalcResult(0 + idxToAdd, 1 + idxToAdd, 6 + idxToAdd, 7 + idxToAdd);
          recalcResult(0 + idxToAdd, 1 + idxToAdd, 8 + idxToAdd, 9 + idxToAdd);
          recalcResult(10 + idxToAdd, 11 + idxToAdd, 12 + idxToAdd, 13 + idxToAdd);
          recalcResult(10 + idxToAdd, 11 + idxToAdd, 14 + idxToAdd, 15 + idxToAdd);
          recalcResult(10 + idxToAdd, 11 + idxToAdd, 16 + idxToAdd, 17 + idxToAdd);
          recalcResult(10 + idxToAdd, 11 + idxToAdd, 18 + idxToAdd, 19 + idxToAdd);
      }
      clearTableDataContents();
      fillInTable(0, mResDbFileSL);//update table
}// !k
}

void DiagnosticWidget::changeCompany(QString company)
{
  QDir dir ;

  ui -> numbersBox -> clear() ;

  if (ui -> emRadioButton -> isChecked() )
  {
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + company + "/EM") ;
  }
  else
  {
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + company + "/Piezo") ;
  }

  SL = dir.entryList(QDir::Files) ;

  if (SL.isEmpty() )
  {
    ui -> readFromDatabaseButton -> setDisabled(true) ;
    return ;
  }
  else ui -> readFromDatabaseButton -> setEnabled(true) ;

  for (int i = 0 ; i < SL.size() ; i++)
  {
    SL[i].chop(4) ;
  }

  ui -> numbersBox -> addItems(SL) ;
}

void DiagnosticWidget::changeTypeEM()
{
  QDir dir ;

  ui -> numbersBox -> clear() ;

  dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/EM") ;

  SL = dir.entryList(QDir::Files) ;

  if (SL.isEmpty() )
  {
    ui -> readFromDatabaseButton -> setDisabled(true) ;
    return ;
  }
  else ui -> readFromDatabaseButton -> setEnabled(true) ;

  for (int i = 0 ; i < SL.size() ; i++)
  {
    SL[i].chop(4) ;
  }

  ui -> numbersBox -> addItems(SL) ;
}

void DiagnosticWidget::changeTypePiezo()
{

  QDir dir ;

  ui -> numbersBox -> clear() ;

  dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/Piezo") ;

  SL = dir.entryList(QDir::Files) ;

  if (SL.isEmpty() )
  {
    ui -> readFromDatabaseButton -> setDisabled(true) ;
    return ;
  }
  else ui -> readFromDatabaseButton -> setEnabled(true) ;

  for (int i = 0 ; i < SL.size() ; i++)
  {
    SL[i].chop(4) ;
  }

  ui -> numbersBox -> addItems(SL) ;
}

void DiagnosticWidget::setOwnerInfo(ownerInfoType info)
{
    *ownerInfo = info;
}
bool DiagnosticWidget::numberic(const QString code) {
    std::string kod= code.toStdString();
    std::string::const_iterator it = kod.begin();
    while (it != kod.end() && isdigit(*it)) ++it;
    return !kod.empty() && it == kod.end();
}


void DiagnosticWidget::on_toolButton_clicked()
{
    sendData5ToDriver();
    runDriver();
    //QTimer timer;
    sendData4ToDriver();
}

