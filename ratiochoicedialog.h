#ifndef RATIOCHOICEDIALOG_H
#define RATIOCHOICEDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui
{
  class RatioChoiceDialog ;
}

class RatioChoiceDialog : public QDialog
{
  Q_OBJECT

  public:
    explicit RatioChoiceDialog(QWidget *parent) ;
    ~RatioChoiceDialog() ;

  public slots:
    void maxChosen();
    void medChosen();
    void minChosen();
    void xxChosen();
    void xxxChosen();

  public:
    int chosenRatioTableRowIdx;
    int chosenActualDataVisRow;//visible where real data is written starting from 0
    //here its 5 rows of data filled from file so it will be 0-4

  private:
    Ui::RatioChoiceDialog *ui ;
} ;

#endif
