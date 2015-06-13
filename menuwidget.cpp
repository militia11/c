#include "menuwidget.h"
#include "ui_menuwidget.h"
#include "datawidget.h"

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent, Qt::WindowMinMaxButtonsHint)
    , shouldOpenSettingsOnStart(false)
    , ui(new Ui::MenuWidget)
    , stackedWidget(qobject_cast<SlidingStackedWidget*>(parent))
{
  ui -> setupUi(this);
  ui->notFoundLabel->hide();

  foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts() )
  {
    portsList.append(info.portName() );
  }

  reloadHwLists();

  if (portsList.size() == 0)
  {
    QMessageBox::critical(this, "Brak portów", "Nie odnaleziono żadnego portu szeregowego RS-232.");
    ui -> diagnosticButton -> setDisabled(true);
  }
  configWidget =new PortConfigWidget(stackedWidget, portsList);
  stackedWidget->addWidget(CONFIG_WIDGET, configWidget);

  QFile file(QDir::currentPath() + "/portsettings.ini");
  QStringList SL;
  portConfigType config;
  if(file.exists())
  {
    file.open(QFile::Text | QFile::ReadOnly);
    QTextStream str(&file);
    while(!str.atEnd() )
    {
      SL.append(str.readLine() );
    }
    file.close();

    config = configWidget->configFileToPortConfigType(SL);
  }
  else
  {
      shouldOpenSettingsOnStart = true;
  }

  SL.clear();
  port = new QSerialPort(this);

  const ownerInfoType ownerInfo = readOwnerInfoFile();
  ui->ownerName->setText(ownerInfo.ownerName);
  ui->ownerPhone->setText(ownerInfo.ownerPhone);
  ui->ownerCity->setText(ownerInfo.ownerCity);
  ui->ownerStreet->setText(ownerInfo.ownerStreet);
  connect(ui->ownerName, SIGNAL(editingFinished()), this, SLOT(ownerInfoChanged()));
  connect(ui->ownerPhone, SIGNAL(editingFinished()), this, SLOT(ownerInfoChanged()));
  connect(ui->ownerCity, SIGNAL(editingFinished()), this, SLOT(ownerInfoChanged()));
  connect(ui->ownerStreet, SIGNAL(editingFinished()), this, SLOT(ownerInfoChanged()));


  diagnostic =new DiagnosticWidget(stackedWidget, port);
  diagnostic->setOwnerInfo(ownerInfo);
  stackedWidget->addWidget(DIAGNOSTIC_WIDGET, diagnostic);
  dbWidget =new DatabaseWidget(stackedWidget);
  stackedWidget->addWidget(DATABASE_WIDGET, dbWidget);


  signalMapper =new QSignalMapper(this);
  connect(ui->diagnosticButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
  signalMapper->setMapping(ui->diagnosticButton, DIAGNOSTIC_WIDGET);
  connect(this, SIGNAL(hwListsChanged()), diagnostic, SLOT(reloadHwLists()));

  connect(ui->databaseButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
  signalMapper->setMapping(ui->databaseButton, DATABASE_WIDGET);
  connect(dbWidget, SIGNAL(dialogClosed()), this, SLOT(onDbWidgetClosed()));
  connect(dbWidget, SIGNAL(hwListsChanged()), this, SLOT(reloadHwLists()));


  connect(ui->settingsButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
  signalMapper->setMapping(ui->settingsButton, CONFIG_WIDGET);
  connect(configWidget, SIGNAL(widgetClosed()), this, SLOT(onConfigWidgetClosed()));


  connect(ui -> exitButton, SIGNAL(clicked() ), qApp, SLOT(quit() ) );
  connect(signalMapper, SIGNAL(mapped(int)), stackedWidget, SLOT(slideInIdx(int)));
  connect(signalMapper, SIGNAL(mapped(int)), stackedWidget, SLOT(slideInIdx(int)));
  connect(signalMapper, SIGNAL(mapped(int)), stackedWidget, SLOT(slideInIdx(int)));
  connect(ui -> searchButton, SIGNAL(clicked() ), this, SLOT(search() ) );
  connect(ui -> companyBox, SIGNAL(currentIndexChanged(QString) ), this, SLOT(changeCompany(QString) ) );
  connect(ui -> typeBox, SIGNAL(currentIndexChanged(QString) ), this, SLOT(changeType(QString) ) );

  if(!shouldOpenSettingsOnStart)
  {
      if(!port->isOpen())
      {
        port->setPortName(config.mPortName);
        port->open(QIODevice::ReadWrite);
        port->setBaudRate(config.mBaudRate);
        port->setDataBits(config.mDataBits);
        port->setFlowControl(config.mFlowControl);
        port->setParity(config.mParity);
        port->setStopBits(config.mStopBits);
      }
      QStringList aa = configWidget->portConfigTypeToconfigFile(config);
      configWidget->updateUiToCurConf(aa);
  }
}

MenuWidget::~MenuWidget()
{
  delete ui;
}

void MenuWidget::onDbWidgetClosed()
{
  QDir dir;
  QStringList SL;

  ui -> numbersBox -> clear();

  dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/" + ui -> typeBox -> currentText() );
  SL = dir.entryList(QDir::Files);

  for (int i = 0; i < SL.size(); i++)
  {
    SL[i].chop(4);
  }

  if (SL.isEmpty() ) ui -> searchButton -> setDisabled(true);
  else
  {
    ui -> numbersBox -> addItems(SL);
    ui -> searchButton -> setEnabled(true);
  }
}

ownerInfoType MenuWidget::readOwnerInfoFile()
{
  ownerInfoType tempInfo;
  QFile file(QDir::currentPath() + "/ownerinfo.ini");
  if(file.exists())
  {
    file.open(QFile::Text | QFile::ReadOnly);
    QTextStream str(&file);

    tempInfo.ownerName = str.readLine();
    tempInfo.ownerPhone = str.readLine();
    tempInfo.ownerCity = str.readLine();
    tempInfo.ownerStreet = str.readLine();

    file.close();
  }
  return tempInfo;
}

void MenuWidget::writeOwnerInfoFile(ownerInfoType info)
{
    QFile file(QDir::currentPath() + "/ownerinfo.ini");
    file.open(QFile::Text | QFile::WriteOnly);
    QTextStream str(&file);

    str << info.ownerName << "\n";
    str << info.ownerPhone << "\n";
    str << info.ownerCity << "\n";
    str << info.ownerStreet << "\n";

    file.close();
}

void MenuWidget::ownerInfoChanged()
{
    ownerInfoType ownerInfo;
    ownerInfo.ownerName = ui->ownerName->text();
    ownerInfo.ownerPhone = ui->ownerPhone->text();
    ownerInfo.ownerCity = ui->ownerCity->text();
    ownerInfo.ownerStreet = ui->ownerStreet->text();
    diagnostic->setOwnerInfo(ownerInfo);
    writeOwnerInfoFile(ownerInfo);
}

void MenuWidget::reloadHwLists()
{
    ui->numbersBox->clear();

    QDir dir;
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/" + ui -> typeBox -> currentText() );
    QStringList SL = dir.entryList(QDir::Files);

    if (SL.isEmpty() )
      ui -> searchButton -> setDisabled(true);
    else
    {
      for (int i = 0; i < SL.size(); i++)
      {
        SL[i].chop(4);
      }
      ui->numbersBox->addItems(SL);
    }
    emit hwListsChanged();
}

void MenuWidget::onConfigWidgetClosed()
{
  if(configWidget->canceled)
    return;
  if(shouldOpenSettingsOnStart)
    shouldOpenSettingsOnStart = false;

  QFile file(QDir::currentPath() + "/portsettings.ini");
  file.open(QFile::Text | QFile::WriteOnly);
  QTextStream str(&file);

  for(size_t i = 0; i < configWidget->curUiConf.size(); i++)
  {
      str << configWidget->curUiConf[i];
      if(i !=configWidget->curUiConf.size() -1)
        str << "\n";
  }

  file.close();

  portConfigType curConf = configWidget->configFileToPortConfigType(configWidget->curUiConf);

  port->setPortName(curConf.mPortName);
  if(!port->isOpen())
    port->open(QIODevice::ReadWrite);
  port->setPortName(curConf.mPortName);
  port->setBaudRate(curConf.mBaudRate);
  port->setDataBits(curConf.mDataBits);
  port->setFlowControl(curConf.mFlowControl);
  port->setParity(curConf.mParity);
  port->setStopBits(curConf.mStopBits);
}

void MenuWidget::search()
{
    QFile file;
    QString fileName = ui -> numbersBox -> currentText();
    file.setFileName(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText()
                     + '/' + ui -> typeBox -> currentText() + '/' + fileName + ".txt" );
    if(file.exists())
    {
        if(ui->notFoundLabel->isVisible())
            ui->notFoundLabel->hide();
        stackedWidget->slideInIdx(DIAGNOSTIC_WIDGET);
        diagnostic->selectProduct(
                    ui->companyBox->currentText(), ui->typeBox->currentText()
                    , ui->numbersBox->currentText());
    }
    else
    {
        if(!ui->notFoundLabel->isVisible())
            ui->notFoundLabel->show();
    }
}

void MenuWidget::changeCompany(QString company)
{
  ui -> numbersBox -> clear();

  QDir dir;
  QStringList SL;

  dir.setPath(QDir::currentPath() + "/DatabaseSx/" + company + "/" + ui -> typeBox -> currentText() );
  SL = dir.entryList(QDir::Files);

  for (int i = 0; i < SL.size(); i++)
  {
    SL[i].chop(4);
  }

  if (SL.isEmpty() )
  {
    ui -> searchButton -> setDisabled(true);
    return;
  }
  else ui -> searchButton -> setEnabled(true);

  ui -> numbersBox -> addItems(SL);
}

void MenuWidget::changeType(QString type)
{
  ui -> numbersBox -> clear();

  QDir dir;
  QStringList SL;

  dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/" + type);
  SL = dir.entryList(QDir::Files);

  for (int i = 0; i < SL.size(); i++)
  {
    SL[i].chop(4);
  }

  if (SL.isEmpty() )
  {
    ui -> searchButton -> setDisabled(true);
    return;
  }
  else ui -> searchButton -> setEnabled(true);

  ui -> numbersBox -> addItems(SL);
}
