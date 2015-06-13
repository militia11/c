#include "datawidget.h"
#include "ui_datawidget.h"

DataWidget::DataWidget(QWidget *parent, QString title, QStringList SL, QString file)
  : QWidget(parent)
  , inputData(SL)
  , filename(file)
  , ui(new Ui::DataWidget)
  , stackedWidget(qobject_cast<SlidingStackedWidget*>(parent))
{
  ui->setupUi(this) ;
  ui->titleLabel->setText(title);
  canceled = false ;

  if(!SL.empty())
    fillUiFromSList(SL);

  connect(ui->cancelButton, SIGNAL(clicked() ), this, SLOT(cancel() ) ) ;
  connect(ui->saveButton, SIGNAL(clicked() ), this, SLOT(saveData() ) ) ;
}

DataWidget::~DataWidget()
{
  delete ui ;
}

void DataWidget::fillUiFromSList(QStringList SL)
{
    ui->editl1u1->setText(SL[0]);
    ui->editl1u2->setText(SL[1]);
    ui->editl1u3->setText(SL[2]);
    ui->editl1u4->setText(SL[3]);
    ui->editl1u5->setText(SL[4]);
    ui->editl1u6->setText(SL[5]);
    ui->editl1u7->setText(SL[6]);
    ui->editl1u8->setText(SL[7]);
    ui->editl2u1->setText(SL[8]);
    ui->editl2u2->setText(SL[9]);
    ui->editl2u3->setText(SL[10]);
    ui->editl2u4->setText(SL[11]);
    ui->editl2u5->setText(SL[12]);
    ui->editl2u6->setText(SL[13]);
    ui->editl2u7->setText(SL[14]);
    ui->editl2u8->setText(SL[15]);
    ui->editl3u1->setText(SL[16]);
    ui->editl3u2->setText(SL[17]);
    ui->editl3u3->setText(SL[18]);
    ui->editl3u4->setText(SL[19]);
    ui->editl3u5->setText(SL[20]);
    ui->editl3u6->setText(SL[21]);
    ui->editl3u7->setText(SL[22]);
    ui->editl3u8->setText(SL[23]);
    ui->editl4u1->setText(SL[24]);
    ui->editl4u2->setText(SL[25]);
    ui->editl4u3->setText(SL[26]);
    ui->editl4u4->setText(SL[27]);
    ui->editl4u5->setText(SL[28]);
    ui->editl4u6->setText(SL[29]);
    ui->editl4u7->setText(SL[30]);
    ui->editl4u8->setText(SL[31]);
    ui->editl5u1->setText(SL[32]);
    ui->editl5u2->setText(SL[33]);
    ui->editl5u3->setText(SL[34]);
    ui->editl5u4->setText(SL[35]);
    ui->editl5u5->setText(SL[36]);
    ui->editl5u6->setText(SL[37]);
    ui->editl5u7->setText(SL[38]);
    ui->editl5u8->setText(SL[39]);
}

void DataWidget::cancel()
{
  canceled = true ;

  if(!inputData.empty())
      inputData.clear();

  stackedWidget->slideInIdx(DATABASE_WIDGET);
  emit dialogClosed();
}

void DataWidget::saveData()
{
  if(!data.isEmpty())
    data.clear();

  for(int i = 0; i < 5; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      const QString objectName =
              QString("editl%1").arg(i+1) + QString("u%1").arg(j+1);
      QLineEdit* lineEdit = this->findChild<QLineEdit*>(
                  objectName, Qt::FindChildrenRecursively);
      const QString text = lineEdit->text();
      if(text.isEmpty())
         data.append("0");
      else
         data.append(text);
    }
  }
  fillUiFromSList(data);

  bool showError = false;
  for(int i = 0; i < 5; i++)
  {
      const QString objectName1_1 = QString("editl%1u5").arg(i+1);
      QLineEdit* lineEdit1_1 = this->findChild<QLineEdit*>(
                  objectName1_1, Qt::FindChildrenRecursively);
      const QString objectName1_2 = QString("editl%1u6").arg(i+1);
      QLineEdit* lineEdit1_2 = this->findChild<QLineEdit*>(
                  objectName1_2, Qt::FindChildrenRecursively);

      const QString objectName2_1 = QString("editl%1u7").arg(i+1);
      QLineEdit* lineEdit2_1 = this->findChild<QLineEdit*>(
                  objectName2_1, Qt::FindChildrenRecursively);
      const QString objectName2_2 = QString("editl%1u8").arg(i+1);
      QLineEdit* lineEdit2_2 = this->findChild<QLineEdit*>(
                  objectName2_2, Qt::FindChildrenRecursively);

      if((lineEdit1_1->text() == "0" && lineEdit1_2->text() == "0")
              || (lineEdit2_1->text() == "0" && lineEdit2_2->text() == "0"))
          continue;
      if(lineEdit1_1->text().toInt() > lineEdit1_2->text().toInt()
         || lineEdit2_1->text().toInt() > lineEdit2_2->text().toInt())
          showError = true;
  }
  if(showError)
  {
      QMessageBox::warning(this, "Błąd",
                           "Wartość \"od\" musi być większa niż wartość \"do\".");
      return;
  }

  if(!inputData.empty())
    inputData.clear();

  stackedWidget->slideInIdx(DATABASE_WIDGET);
  emit dialogClosed();
}
