#include "databasewidget.h"
#include "ui_databasewidget.h"

DatabaseWidget::DatabaseWidget(QWidget *parent)
    : QWidget(parent, Qt::WindowMinMaxButtonsHint)
    , ui(new Ui::DatabaseWidget)
    , stackedWidget(qobject_cast<SlidingStackedWidget*>(parent))
    , dataWidget(nullptr)
    , showResultWiget(nullptr)
{
  ui -> setupUi(this) ;

  connect(ui -> closeDialogButton, SIGNAL(clicked() ), this, SLOT(closeDialog() ) ) ;
  connect(ui -> addDataButton, SIGNAL(clicked() ), this, SLOT(addDataToDatabaseInit() ) ) ;
  connect(ui -> editDataButton, SIGNAL(clicked() ), this, SLOT(editDataInit() ) ) ;
  connect(ui -> deleteDataButton, SIGNAL(clicked() ), this, SLOT(deleteDataFromDatabase() ) ) ;
  connect(ui -> showResultButton, SIGNAL(clicked() ), this, SLOT(showResult() ) ) ;
  connect(ui -> editResultButton, SIGNAL(clicked() ), this, SLOT(editResultInit() ) ) ;
  connect(ui -> deleteResultButton, SIGNAL(clicked() ), this, SLOT(deleteResult() ) ) ;
}

DatabaseWidget::~DatabaseWidget()
{
  delete ui ;
}

void DatabaseWidget::closeDialog()
{
  stackedWidget->slideInIdx(MENU_WIDGET);
  emit dialogClosed();
}

void DatabaseWidget::addDataToDatabaseInit()
{
  FormDialog fDialog(this, FormDialog::AddingRecord, "Numer wtryskiwacza:") ;

  fDialog.exec() ;

  if (fDialog.canceled) return ;

  QString filePathName(QDir::currentPath() + "/DatabaseSx/" + fDialog.company
                       + '/' + fDialog.type + '/' + fDialog.number + ".txt");
  QFile file(filePathName);

  if (file.exists() )
  {
    QMessageBox::warning(this, "Błąd", "Nastawy do tego wtryskiwacza już istnieją w bazie.") ;
    return ;
  }

  if(dataWidget)
  {
      stackedWidget->removeWidget(dataWidget);
      delete dataWidget;
      dataWidget = nullptr;
  }
  const QString title(fDialog.company + " - " + fDialog.type + " - " + fDialog.number);
  dataWidget =new DataWidget(stackedWidget, title, QStringList(), filePathName);
  stackedWidget->addWidget(DATA_WIDGET, dataWidget);
  stackedWidget->slideInIdx(DATA_WIDGET);
  connect(dataWidget, SIGNAL(dialogClosed()), this, SLOT(addDataToDatabaseClosed()));
}

void DatabaseWidget::addDataToDatabaseClosed()
{
    if(!dataWidget || dataWidget->canceled)
        return;

    QFile file(dataWidget->filename);
    file.open(QFile::Text | QFile::WriteOnly) ;
    QTextStream str(&file) ;
    for (int i = 0 ; i < 40 ; i++)
    {
      str << dataWidget->data[i];
      if(i != 39)
        str << '\n';
    }
    file.close() ;

    QMessageBox::information(this, "Sukces", "Nastawy zostały dodane do bazy danych.") ;

    emit hwListsChanged();
}

void DatabaseWidget::editDataInit()
{
  FormDialog fDialog(this, FormDialog::SelectingRecord, "Numer wtryskiwacza:") ;

  fDialog.exec() ;

  if (fDialog.canceled) return ;

  const QString filePathName(QDir::currentPath() + "/DatabaseSx/" + fDialog.company
                            + '/' + fDialog.type + '/' + fDialog.number + ".txt");
  QFile file(filePathName);

  if (!file.exists() )
  {
    QMessageBox::critical(this, "Błąd", "Nie udało się wczytać nastaw z bazy. Może nastawy do tego wtryskiwacza nie istnieją?") ;
    return ;
  }

  file.open(QFile::Text | QFile::ReadOnly) ;

  QTextStream str(&file) ;
  QStringList SL ;

  for (int i = 0 ; i < 40 ; i++)
  {
    SL.append(str.readLine() ) ;
  }

  file.close() ;
  str.flush() ;


  if(dataWidget)
  {
      stackedWidget->removeWidget(dataWidget);
      delete dataWidget;
      dataWidget = nullptr;
  }
  const QString title(fDialog.company + " - " + fDialog.type + " - " + fDialog.number);
  dataWidget =new DataWidget(stackedWidget, title, SL, filePathName);
  stackedWidget->addWidget(DATA_WIDGET, dataWidget);
  stackedWidget->slideInIdx(DATA_WIDGET);
  connect(dataWidget, SIGNAL(dialogClosed()), this, SLOT(editDataClosed()));
}

void DatabaseWidget::editDataClosed()
{
    if(!dataWidget || dataWidget->canceled)
        return;

    QFile file(dataWidget->filename);
    file.open(QFile::Text | QFile::WriteOnly) ;
    QTextStream str(&file) ;
    for (int i = 0 ; i < 40 ; i++)
    {
      str << dataWidget->data[i];
      if(i != 39)
        str << '\n';
    }
    file.close() ;
    QMessageBox::information(this, "Sukces", "Nastawy zostały pomyślnie zmienione.") ;
}

void DatabaseWidget::deleteDataFromDatabase()
{
  FormDialog fDialog(this, FormDialog::SelectingRecord, "Numer wtryskiwacza:") ;

  fDialog.exec() ;

  if (fDialog.canceled) return ;

  QFile file(QDir::currentPath() + "/DatabaseSx/" + fDialog.company + '/' + fDialog.type + '/' + fDialog.number + ".txt") ;

  if(file.remove())
    QMessageBox::information(this, "Sukces",
                             "Nastawy zostały pomyślnie usunięte z bazy danych.") ;
  else
      QMessageBox::critical(this, "Błąd",
                            "Nie udało się usunąć nastaw z bazy. Może nastawy \
                            do tego wtryskiwacza nigdy nie istniały?") ;
}

void DatabaseWidget::showResult()
{
  FormDialog dialog(this, FormDialog::SelectingRecord, "Nazwa rekordu:") ;

  dialog.exec() ;

  if (dialog.canceled) return ;


  QFile dbFile;
  QFile dbResFile;
  dbFile.setFileName(QDir::currentPath() + "/DatabaseSx/" + dialog.company
                     + "/" + dialog.type + "/" + dialog.number + ".txt");
  dbResFile.setFileName(QDir::currentPath() + "/DatabaseSx/" + dialog.company
                        + "/" + dialog.type + "/Results/" + dialog.number + ".txt");

  if(!dbFile.exists() || !dbResFile.exists())
  {
    QMessageBox::warning(this, "Wynik"
                         , "Wynik o podanej nazwie nie istnieje w bazie danych.");
    return;
  }

  QStringList dbFileSL ;
  {//read from file
    dbFile.open(QFile::Text | QFile::ReadOnly) ;
    QTextStream str(&dbFile) ;
    for (int i = 0 ; i < 40 ; i++)
      dbFileSL.append(str.readLine() ) ;
    dbFile.close() ;
  }
  QStringList dbResFileSL;
  {//read from file
      dbResFile.open(QFile::Text | QFile::ReadOnly) ;
      QTextStream str(&dbResFile) ;
      for (int i = 0 ; i < 100 ; i++)
        dbResFileSL.append(str.readLine() ) ;
      dbResFile.close() ;
  }

  if(showResultWiget)
  {
     stackedWidget->removeWidget(showResultWiget);
     delete showResultWiget;
     showResultWiget = nullptr;
  }
  const QString hwName = dialog.company + "-" + dialog.type + "-" + dialog.number;
  showResultWiget =new ResultWidget(stackedWidget, dbFileSL, dbResFileSL, hwName, SHOW_ONLY);
  stackedWidget->addWidget(SHOWRESULT_WIDGET, showResultWiget);
  stackedWidget->slideInIdx(SHOWRESULT_WIDGET);
}

void DatabaseWidget::editResultInit()
{
  FormDialog dialog(this, FormDialog::SelectingRecord, "Nazwa rekordu:") ;
  dialog.exec() ;
  if (dialog.canceled)
      return ;


  QFile dbFile;
  QFile dbResFile;
  dbFile.setFileName(QDir::currentPath() + "/DatabaseSx/" + dialog.company
                     + "/" + dialog.type + "/" + dialog.number + ".txt");
  dbResFile.setFileName(QDir::currentPath() + "/DatabaseSx/" + dialog.company
                        + "/" + dialog.type + "/Results/" + dialog.number + ".txt");

  if(!dbFile.exists() || !dbResFile.exists())
  {
    QMessageBox::warning(this, "Wynik"
                         , "Wynik o podanej nazwie nie istnieje w bazie danych.");
    return;
  }

  QStringList dbFileSL ;
  {//read from file
    dbFile.open(QFile::Text | QFile::ReadOnly) ;
    QTextStream str(&dbFile) ;
    for (int i = 0 ; i < 40 ; i++)
      dbFileSL.append(str.readLine() ) ;
    dbFile.close() ;
  }
  QStringList dbResFileSL;
  {//read from file
      dbResFile.open(QFile::Text | QFile::ReadOnly) ;
      QTextStream str(&dbResFile) ;
      for (int i = 0 ; i < 100 ; i++)
        dbResFileSL.append(str.readLine() ) ;
      dbResFile.close() ;
  }
  if(showResultWiget)
  {
    stackedWidget->removeWidget(showResultWiget);
    delete showResultWiget;
    showResultWiget = nullptr;
  }
  const QString hwName = dialog.company + "-" + dialog.type + "-" + dialog.number;
  showResultWiget =new ResultWidget(stackedWidget, dbFileSL, dbResFileSL, hwName, EDIT);
  stackedWidget->addWidget(SHOWRESULT_WIDGET, showResultWiget);
  stackedWidget->slideInIdx(SHOWRESULT_WIDGET);
  connect(showResultWiget, SIGNAL(dialogClosed()), this, SLOT(editResultClosed()));
}

void DatabaseWidget::editResultClosed()
{
  if(!showResultWiget || showResultWiget->canceled)
    return;

  QStringList hwSL = showResultWiget->hwName.split("-");
  const QString filePathName(QDir::currentPath() + "/DatabaseSx/" + hwSL[0]
          + "/" + hwSL[1] + "/Results/" + hwSL[2] + ".txt");
  QFile file(filePathName);
  file.open(QFile::WriteOnly | QFile::Text) ;
  QTextStream in(&file) ;

  for (int i = 0 ; i < showResultWiget->mResData.size() ; i++)
  {
    in << showResultWiget->mResData[i];
    if(i != showResultWiget->mResData.size() - 1)//last line dont add newline
      in << '\n' ;
  }

  file.close() ;

  QMessageBox::information(this, "Suckes", "Wyniki zostały pomyślnie zmienione.") ;
}

void DatabaseWidget::deleteResult()
{
  FormDialog fDialog(this, FormDialog::SelectingRecord, "Nazwa rekordu:") ;

  fDialog.exec() ;

  if (fDialog.canceled) return ;

  QFile file(QDir::currentPath() + "/DatabaseSx/" + fDialog.company + '/' + fDialog.type + "/Results/" + fDialog.number + ".txt") ;

  if (file.remove() )
  {
    QMessageBox::information(this, "Sukces", "Wyniki zostały pomyślnie usunięte z bazy danych.") ;
  }
  else QMessageBox::critical(this, "Błąd", "Nie udało się usunąć wyników z bazy. Może one nigdy nie istniały?") ;
}
