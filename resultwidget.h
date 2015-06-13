#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QStringList>

#include <vector>

#include "slidingstackedwidget.h"

namespace Ui
{
  class ResultWidget ;
}

enum RESULT_DIALOG_TYPE
{
      SHOW_ONLY = 0
    , EDIT = 1
};

class ResultWidget : public QWidget
{
  Q_OBJECT

  public:
    explicit ResultWidget(QWidget *parent, QStringList data, QStringList resData, QString name, RESULT_DIALOG_TYPE type) ;
    ~ResultWidget() ;
    void resizeLowerTable();
    bool canceled;

  public slots:
    void closeDialog() ;
    void cancel() ;
    void edit() ;
    void save() ;

  signals:
    void dialogClosed();

  private:
    Ui::ResultWidget *ui ;
    QStringList SL ;
    SlidingStackedWidget* stackedWidget;
  public:
    QString hwName;
    QStringList mData;
    QStringList mResData;
  private:
    void fillInTable(int startIdx, QStringList data);
    QStringList SL2 ;
    std::vector<int> tableColumnBaseWidth;
    std::vector<int> tableRowBaseHeight;

  protected:
    void resizeEvent(QResizeEvent *event);
} ;

#endif
