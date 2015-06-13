#include "formdialog.h"
#include "ui_formdialog.h"

FormDialog::FormDialog(QWidget *parent, FormType type, QString labelText) :
  QDialog(parent, Qt::WindowTitleHint),
  ui(new Ui::FormDialog)
{
  ui -> setupUi(this) ;

  text = labelText ;

  label = new QLabel(labelText) ;
  numberEdit = new QLineEdit ;
  numbersBox = new QComboBox ;

  formType = type ;

  switch(type)
  {
    case AddingRecord:
      ui -> formLayout_2 -> addRow(label, numberEdit) ;
      break ;

    case SelectingRecord:
      ui -> formLayout_2 -> addRow(label, numbersBox) ;
      connect(ui -> companyBox, SIGNAL(currentIndexChanged(QString) ), this, SLOT(changeCompany(QString) ) ) ;
      connect(ui -> typeBox, SIGNAL(currentIndexChanged(QString) ), this, SLOT(changeType(QString) ) ) ;
      break ;
  }

  if (labelText == "Numer wtryskiwacza:")
  {
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/" + ui -> typeBox -> currentText() ) ;
    SL = dir.entryList(QDir::Files) ;
  }
  else
  {
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/" + ui -> typeBox -> currentText() + "/Results/") ;
    SL = dir.entryList() ;

    SL.removeFirst() ;
    SL.removeFirst() ;
  }

  for (int i = 0 ; i < SL.size() ; i++)
  {
    SL[i].chop(4) ;
  }

  if (SL.isEmpty() && type == SelectingRecord)
  {
    ui -> okButton -> setDisabled(true) ;
  }
  else ui -> okButton -> setEnabled(true) ;

  numbersBox -> addItems(SL) ;

  canceled = false ;

  connect(ui -> okButton, SIGNAL(clicked() ), this, SLOT(ok() ) ) ;
  connect(ui -> cancelButton, SIGNAL(clicked() ), this, SLOT(cancel() ) ) ;
}

FormDialog::~FormDialog()
{
  delete label ;
  delete numberEdit ;
  delete numbersBox ;
  delete ui ;
}

void FormDialog::ok()
{
  company = ui -> companyBox -> currentText() ;
  type = ui -> typeBox -> currentText() ;

  if (formType == AddingRecord)
  {
    number = numberEdit -> text() ;

    if (number.isEmpty() )
    {
      QMessageBox::warning(this, "Numer wtryskiwacza", "Wprowadź numer wtryskiwacza.") ;
      return ;
    }
  }
  else
  {
    number = numbersBox -> currentText() ;
  }

  this -> close() ;
}

void FormDialog::cancel()
{
  canceled = true ;

  this -> close() ;
}

void FormDialog::changeCompany(QString company)
{
  numbersBox -> clear() ;

  QDir dir ;

  if (text == "Numer wtryskiwacza:")
  {
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + company + "/" + ui -> typeBox -> currentText() ) ;
    SL = dir.entryList(QDir::Files) ;
  }
  else
  {
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + company + "/" + ui -> typeBox -> currentText() + "/Results/") ;
    SL = dir.entryList() ;

    SL.removeFirst() ;
    SL.removeFirst() ;
  }

  if (SL.isEmpty() )
  {
    ui -> okButton -> setDisabled(true) ;
    return ;
  }
  else ui -> okButton -> setEnabled(true) ;

  for (int i = 0 ; i < SL.size() ; i++)
  {
    SL[i].chop(4) ;
  }

  numbersBox -> addItems(SL) ;
}

void FormDialog::changeType(QString type)
{
  numbersBox -> clear() ;

  QDir dir ;

  if (text == "Numer wtryskiwacza:")
  {
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/" + type) ;
    SL = dir.entryList(QDir::Files) ;
  }
  else
  {
    dir.setPath(QDir::currentPath() + "/DatabaseSx/" + ui -> companyBox -> currentText() + "/" + type + "/Results/") ;
    SL = dir.entryList() ;

    SL.removeFirst() ;
    SL.removeFirst() ;
  }

  for (int i = 0 ; i < SL.size() ; i++)
  {
    SL[i].chop(4) ;
  }

  if (SL.isEmpty() )
  {
    ui -> okButton -> setDisabled(true) ;
    return ;
  }
  else ui -> okButton -> setEnabled(true) ;

  numbersBox -> addItems(SL) ;
}
