#ifndef PRINTINGFORMDIALOG_H
#define PRINTINGFORMDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QDate>

namespace Ui
{
  class PrintingFormDialog ;
}

class PrintingFormDialog : public QDialog
{
  Q_OBJECT

  public:
    enum FormType
    {
      RememberedNumber,
      NoNumber
    } ;
    explicit PrintingFormDialog(QWidget *parent, FormType type) ;
    ~PrintingFormDialog() ;
    QString name ;
    QString date ;
    QString company ;
    QString type ;
    QString number ;
    bool canceled ;
    QString filepath;

  public slots:
    void ok() ;
    void cancel() ;

private slots:
    void on_pushButton_clicked();

private:
    Ui::PrintingFormDialog *ui ;
    int formType ;

} ;

#endif
