#ifndef DATABASEWIDGET_H
#define DATABASEWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include "formdialog.h"
#include "datawidget.h"
#include "resultwidget.h"
#include "slidingstackedwidget.h"

namespace Ui
{
  class DatabaseWidget ;
}

class DatabaseWidget : public QWidget
{
  Q_OBJECT

  public:
    explicit DatabaseWidget(QWidget *parent) ;
    ~DatabaseWidget() ;

  public slots:
    void closeDialog() ;
    void addDataToDatabaseInit();
    void addDataToDatabaseClosed();
    void editDataInit();
    void editDataClosed();
    void deleteDataFromDatabase() ;
    void showResult() ;
    void editResultInit();
    void editResultClosed();
    void deleteResult() ;

  signals:
    void dialogClosed();
    void hwListsChanged();

  private:
    Ui::DatabaseWidget *ui;
    SlidingStackedWidget* stackedWidget;
    DataWidget* dataWidget;
    ResultWidget* showResultWiget;
} ;

#endif
