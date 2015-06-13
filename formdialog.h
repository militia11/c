#ifndef FORMDIALOG_H
#define FORMDIALOG_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QDir>
#include <QFileInfo>

namespace Ui
{
  class FormDialog ;
}

class FormDialog : public QDialog
{
  Q_OBJECT

  public:
    enum FormType
    {
      AddingRecord,
      SelectingRecord
    } ;
    explicit FormDialog(QWidget *parent, FormType type, QString labelText) ;
    ~FormDialog() ;
    bool canceled ;
    QString company ;
    QString type ;
    QString number ;

  public slots:
    void ok() ;
    void cancel() ;

  private slots:
    void changeCompany(QString company) ;
    void changeType(QString type) ;

  private:
    Ui::FormDialog *ui ;
    QLineEdit *numberEdit ;
    QLabel *label ;
    QComboBox *numbersBox ;
    QStringList SL ;
    QString text ;
    QDir dir ;
    int formType ;
} ;

#endif
