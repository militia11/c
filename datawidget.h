#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QStringList>

#include "slidingstackedwidget.h"

namespace Ui
{
  class DataWidget ;
}

class DataWidget : public QWidget
{
  Q_OBJECT

  public:
    explicit DataWidget(QWidget *parent, QString title, QStringList SL, QString file);
    ~DataWidget() ;
    bool canceled ;
    QStringList data ;
    QStringList inputData;
    QString filename;

  public slots:
    void cancel() ;
    void saveData() ;

  signals:
    void dialogClosed();

  private:
    Ui::DataWidget *ui ;
    SlidingStackedWidget* stackedWidget;

    void fillUiFromSList(QStringList SL);
} ;

#endif
