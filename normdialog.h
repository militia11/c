#ifndef NORMDIALOG_H
#define NORMDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui
{
  class NormDialog ;
}

struct normValuesType
{
    QString rationValue1 ;
    QString rationValue2 ;
    QString rationValue3 ;
    QString rationValue4 ;
    QString overflowValue1 ;
    QString overflowValue2 ;
    QString overflowValue3 ;
    QString overflowValue4 ;
    double rationMin;
    double rationMax;
    double overflowMin;
    double overflowMax;
};

class NormDialog : public QDialog
{
  Q_OBJECT

  public:
    explicit NormDialog(QWidget *parent, normValuesType startValues, bool erow = false) ;
    ~NormDialog() ;
    void setNormsVisible(bool visible);
    normValuesType mData;

  public slots:
    void ok() ;

  private:
    Ui::NormDialog *ui ;
    double rMin,rMax,oMin,oMax;
} ;

#endif
