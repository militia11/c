#include "normdialog.h"
#include "ui_normdialog.h"

NormDialog::NormDialog(QWidget *parent, normValuesType startValues, bool erow) :
  QDialog(parent, Qt::WindowTitleHint),
  ui(new Ui::NormDialog)
{
  ui -> setupUi(this) ;

  if(erow){
      ui->overflowLine1->setEnabled(false);
      ui->overflowLine2->setEnabled(false);
      ui->overflowLine3->setEnabled(false);
      ui->overflowLine4->setEnabled(false);
  }
  rMin=startValues.rationMin;
  rMax=startValues.rationMax;
  oMin=startValues.overflowMin;
  oMax=startValues.overflowMax;

  ui->rationLine1->setText(startValues.rationValue1);
  ui->rationLine2->setText(startValues.rationValue2);
  ui->rationLine3->setText(startValues.rationValue3);
  ui->rationLine4->setText(startValues.rationValue4);

  ui->overflowLine1->setText(startValues.overflowValue1);
  ui->overflowLine2->setText(startValues.overflowValue2);
  ui->overflowLine3->setText(startValues.overflowValue3);
  ui->overflowLine4->setText(startValues.overflowValue4);

  ui->rationFromBox->setValue(startValues.rationMin);
  ui->rationToBox->setValue(startValues.rationMax);
  ui->overflowFromBox->setValue(startValues.overflowMin);
  ui->overflowToBox->setValue(startValues.overflowMax);

  connect(ui -> okButton, SIGNAL(clicked() ), this, SLOT(ok() ) ) ;
}

NormDialog::~NormDialog()
{
  delete ui ;
}

void NormDialog::setNormsVisible(bool visible)
{
    ui->normsSubWidget->setVisible(visible);
}

void NormDialog::ok()
{

  mData.rationMin = rMin;
  mData.rationMax = rMax;
  mData.overflowMin = oMin;
  mData.overflowMax = oMax;

  mData.rationValue1 = ui -> rationLine1 -> text() ;
  mData.rationValue2 = ui -> rationLine2 -> text() ;
  mData.rationValue3 = ui -> rationLine3 -> text() ;
  mData.rationValue4 = ui -> rationLine4 -> text() ;

  mData.overflowValue1 = ui -> overflowLine1 -> text() ;
  mData.overflowValue2 = ui -> overflowLine2 -> text() ;
  mData.overflowValue3 = ui -> overflowLine3 -> text() ;
  mData.overflowValue4 = ui -> overflowLine4 -> text() ;

//  if (mData.rationValue1.isEmpty() || mData.rationValue2.isEmpty()
//      || mData.rationValue3.isEmpty() || mData.rationValue4.isEmpty()
//      || mData.overflowValue1.isEmpty() || mData.overflowValue2.isEmpty()
//      || mData.overflowValue3.isEmpty() || mData.overflowValue4.isEmpty())
//  {
//    QMessageBox::warning(this, "Wyniki", "Uzupełnij pozostałe pola.") ;
//    return ;
//  }

  if(ui->normsSubWidget->isVisible())
  {
    if (mData.rationMin == mData.rationMax || mData.rationMin > mData.rationMax)
    {
      QMessageBox::warning(this, "Błąd", "Wartość \"od\" musi być większa niż wartość \"do\".") ;
      return ;
    }

    if (mData.overflowMin == mData.overflowMax || mData.overflowMin > mData.overflowMax)
    {
      QMessageBox::warning(this, "Błąd", "Wartość \"od\" musi być większa niż wartość \"do\".") ;
      return ;
    }
  }
  this -> close() ;
}
