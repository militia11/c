#include "printingformdialog.h"
#include "ui_printingformdialog.h"
#include <QFileDialog>
PrintingFormDialog::PrintingFormDialog(QWidget *parent, FormType type) :
  QDialog(parent, Qt::WindowTitleHint),
  ui(new Ui::PrintingFormDialog), filepath("")
{
  ui -> setupUi(this) ;

 // numberEdit = new QLineEdit ;

  canceled = false ;

  formType = type ;

  ui -> dateEdit -> setDate(QDate::currentDate() ) ;

  if (formType == NoNumber)
  {
   // ui -> formLayout -> addRow("Numer wtryskiwacza:", numberEdit) ;
    ui -> numberLine -> setVisible(true) ;
    ui -> label_5 -> setVisible(true) ;
  }
  else
  {
    ui -> numberLine -> setVisible(false) ;
    ui -> label_5 -> setVisible(false) ;
  }

  connect(ui -> okButton, SIGNAL(clicked() ), this, SLOT(ok() ) ) ;
  connect(ui -> cancelButton, SIGNAL(clicked() ), this, SLOT(cancel() ) ) ;
}

PrintingFormDialog::~PrintingFormDialog()
{
//  delete numberEdit ;
  delete ui ;
}

void PrintingFormDialog::ok()
{
  name = ui -> nameLine -> text() ;
  date = ui -> dateEdit -> text() ;
  company = ui -> companyBox -> currentText() ;
  type = ui -> typeBox -> currentText() ;
  if (formType == NoNumber)
      number = ui -> numberLine -> text() ;

/*unwanted
  if (name.isEmpty() || street.isEmpty()
          || postalCode.isEmpty() || city.isEmpty()
          || country.isEmpty() || (formType == NoNumber && number.isEmpty() ) )
  {
    QMessageBox::warning(this, "Puste pola", "Uzupełnij brakujące pola.") ;
    return ;
  }
*/

  this -> close() ;
}

void PrintingFormDialog::cancel()
{
  canceled = true ;

  this -> close() ;
}

void PrintingFormDialog::on_pushButton_clicked()
{
    filepath = QFileDialog::getOpenFileName(this,tr("Otworz Plik"),"C://","Wszystkie pliki(*.*);;Mapy bitowe(*.bmp);;Obrazki JPG(*.jpg)");
}
