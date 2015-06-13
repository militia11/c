#include "resultwidget.h"
#include "ui_resultwidget.h"
#include "normdialog.h"
#include "ratiochoicedialog.h"


ResultWidget::ResultWidget(QWidget *parent, QStringList data, QStringList resData, QString name, RESULT_DIALOG_TYPE type)
    : QWidget(parent, Qt::WindowTitleHint)
    , ui(new Ui::ResultWidget)
    , stackedWidget(qobject_cast<SlidingStackedWidget*>(parent))
    , hwName(name)
    , mData(data)
    , mResData(resData)
{
  ui -> setupUi(this) ;
  canceled = false ;
  ui->titleLabel->setText(hwName);

  if(type == SHOW_ONLY)
  {
      ui->resNormPart->setVisible(true);
      ui->resEditPart->setVisible(false);
  }
  else//EDIT
  {
      ui->resNormPart->setVisible(false);
      ui->resEditPart->setVisible(true);
  }

  //1st line
  //1st line
  ui->lowerTable->setSpan(0, 1, 1, 2);
  ui->lowerTable->setSpan(0, 3, 1, 8);
  ui->lowerTable->setSpan(0, 11, 1, 2);
  ui->lowerTable->setSpan(0, 13, 1, 8);
  //2nd line
  ui->lowerTable->setSpan(1, 1, 1, 2);
  ui->lowerTable->setSpan(1, 3, 1, 2);
  ui->lowerTable->setSpan(1, 5, 1, 2);
  ui->lowerTable->setSpan(1, 7, 1, 2);
  ui->lowerTable->setSpan(1, 9, 1, 2);
  ui->lowerTable->setSpan(1, 11, 1, 2);
  ui->lowerTable->setSpan(1, 13, 1, 2);
  ui->lowerTable->setSpan(1, 15, 1, 2);
  ui->lowerTable->setSpan(1, 17, 1, 2);
  ui->lowerTable->setSpan(1, 19, 1, 2);

  tableColumnBaseWidth.push_back(120);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);
  tableColumnBaseWidth.push_back(50);
  tableColumnBaseWidth.push_back(35);

  tableRowBaseHeight.push_back(25);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);
  tableRowBaseHeight.push_back(39);

  for(size_t i = 0; i < tableColumnBaseWidth.size(); i++)
      ui->lowerTable->setColumnWidth(i, tableColumnBaseWidth[i]);
  for(size_t i = 0; i < tableRowBaseHeight.size(); i++)
      ui->lowerTable->setRowHeight(i, tableRowBaseHeight[i]);

  ui -> editl1u1 -> setText(data[0]) ;
  ui -> editl1u2 -> setText(data[1]) ;
  ui -> editl1u3 -> setText(data[2]) ;
  ui -> editl1u4 -> setText(data[3]) ;

  ui -> editl2u1 -> setText(data[8]) ;
  ui -> editl2u2 -> setText(data[9]) ;
  ui -> editl2u3 -> setText(data[10]) ;
  ui -> editl2u4 -> setText(data[11]) ;

  ui -> editl3u1 -> setText(data[16]) ;
  ui -> editl3u2 -> setText(data[17]) ;
  ui -> editl3u3 -> setText(data[18]) ;
  ui -> editl3u4 -> setText(data[19]) ;

  ui -> editl4u1 -> setText(data[24]) ;
  ui -> editl4u2 -> setText(data[25]) ;
  ui -> editl4u3 -> setText(data[26]) ;
  ui -> editl4u4 -> setText(data[27]) ;

  ui -> editl5u1 -> setText(data[32]) ;
  ui -> editl5u2 -> setText(data[33]) ;
  ui -> editl5u3 -> setText(data[34]) ;
  ui -> editl5u4 -> setText(data[35]) ;

  fillInTable(0, resData);

  //show_only enum
  connect(ui ->closeDialogButton, SIGNAL(clicked() ), this, SLOT(closeDialog() ) ) ;
  //edit_res enum
  connect(ui -> cancelButton, SIGNAL(clicked() ), this, SLOT(cancel() ) ) ;
  connect(ui -> editButton, SIGNAL(clicked() ), this, SLOT(edit() ) ) ;
  connect(ui -> saveButton, SIGNAL(clicked() ), this, SLOT(save() ) ) ;

}

ResultWidget::~ResultWidget()
{
  delete ui ;
}

void ResultWidget::closeDialog()
{
  canceled = false;
  stackedWidget->slideInIdx(DATABASE_WIDGET);
}

void ResultWidget::cancel()
{
  canceled = true ;
  stackedWidget->slideInIdx(DATABASE_WIDGET);
  emit dialogClosed();
}

void ResultWidget::edit()
{
  RatioChoiceDialog rChoiceDiag(this);
  rChoiceDiag.exec();
  const int& chosenRow = rChoiceDiag.chosenActualDataVisRow;
  const int idxToAdd = chosenRow * 20;

  normValuesType curTableValues;
  curTableValues.rationMin = mResData[0 + idxToAdd].toInt();
  curTableValues.rationMax = mResData[1 + idxToAdd].toInt();
  curTableValues.rationValue1 = mResData[2 + idxToAdd];
  curTableValues.rationValue2 = mResData[4 + idxToAdd];
  curTableValues.rationValue3 = mResData[6 + idxToAdd];
  curTableValues.rationValue4 = mResData[8 + idxToAdd];
  curTableValues.overflowMin = mResData[10 + idxToAdd].toInt();
  curTableValues.overflowMax = mResData[11 + idxToAdd].toInt();
  curTableValues.overflowValue1 = mResData[12 + idxToAdd];
  curTableValues.overflowValue2 = mResData[14 + idxToAdd];
  curTableValues.overflowValue3 = mResData[16 + idxToAdd];
  curTableValues.overflowValue4 = mResData[18 + idxToAdd];

  NormDialog dialog(this, curTableValues);
  dialog.setNormsVisible(true);
  dialog.exec() ;

  mResData[0 + idxToAdd] = QString::number(dialog.mData.rationMin);
  mResData[1 + idxToAdd] = QString::number(dialog.mData.rationMax);
  mResData[2 + idxToAdd] = dialog.mData.rationValue1;
  mResData[4 + idxToAdd] = dialog.mData.rationValue2;
  mResData[6 + idxToAdd] = dialog.mData.rationValue3;
  mResData[8 + idxToAdd] = dialog.mData.rationValue4;
  mResData[10 + idxToAdd] = QString::number(dialog.mData.overflowMin);
  mResData[11 + idxToAdd] = QString::number(dialog.mData.overflowMax);
  mResData[12 + idxToAdd] = dialog.mData.overflowValue1;
  mResData[14 + idxToAdd] = dialog.mData.overflowValue2;
  mResData[16 + idxToAdd] = dialog.mData.overflowValue3;
  mResData[18 + idxToAdd] = dialog.mData.overflowValue4;

  //recalc if results(changed) are between norms
  auto recalcResult = [&](int minNormIdx, int maxNormIdx
          , int resultIdx, int testIdx)
  {
      if(mResData[resultIdx].toInt() >= mResData[minNormIdx].toInt()
              && mResData[resultIdx].toInt() <= mResData[maxNormIdx].toInt())
          mResData[testIdx] = "OK";
      else
          mResData[testIdx] = "NG";
  };
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 2 + idxToAdd, 3 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 4 + idxToAdd, 5 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 6 + idxToAdd, 7 + idxToAdd);
  recalcResult(0 + idxToAdd, 1 + idxToAdd, 8 + idxToAdd, 9 + idxToAdd);
  recalcResult(10 + idxToAdd, 11 + idxToAdd, 12 + idxToAdd, 13 + idxToAdd);
  recalcResult(10 + idxToAdd, 11 + idxToAdd, 14 + idxToAdd, 15 + idxToAdd);
  recalcResult(10 + idxToAdd, 11 + idxToAdd, 16 + idxToAdd, 17 + idxToAdd);
  recalcResult(10 + idxToAdd, 11 + idxToAdd, 18 + idxToAdd, 19 + idxToAdd);

  fillInTable(0, mResData);//update table
}

void ResultWidget::save()
{
  canceled = false;
  stackedWidget->slideInIdx(DATABASE_WIDGET);
  emit dialogClosed();
}

void ResultWidget::fillInTable(int startIdx, QStringList data)
{
    auto safeSetTableText = [&](int _row, int _col, QString _text)
    {
        QTableWidgetItem* tItem = ui->lowerTable->item(_row, _col);
        if(tItem == nullptr)
        {
            tItem =new QTableWidgetItem();
            ui->lowerTable->setItem(_row, _col, tItem);
        }
        tItem->setText(_text);
        tItem->setTextAlignment(Qt::AlignCenter);
        if(_text == "OK")
            tItem->setTextColor(Qt::darkGreen);
        else if(_text == "NG")
            tItem->setTextColor(Qt::red);
    };

    int itemCounter = startIdx;
    for(int i = 2; i <= 6; i++)
    {
      for(int j = 1; j <= 20; j++)
      {
          safeSetTableText(i, j, data[itemCounter]);
          itemCounter++;
      }
    }

//    ui->lowerTable->resizeColumnsToContents();
//    ui->lowerTable->resizeRowsToContents();
}

void ResultWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeLowerTable();
}

void ResultWidget::resizeLowerTable()
{
    int maxSizeX = ui->lowerTable->width() - 20;//leave some space
    int maxSizeY = ui->lowerTable->height() - 20;
    int sizex = 0;
    for(size_t i = 0; i < tableColumnBaseWidth.size(); i++)
        sizex += tableColumnBaseWidth[i];
    int sizey = 0;
     for(size_t i = 0; i < tableRowBaseHeight.size(); i++)
        sizey += tableRowBaseHeight[i];

     if(maxSizeX < sizex)
         maxSizeX = sizex;
     if(maxSizeY < sizey)
         maxSizeY = sizey;

    const int sizeXLeft =  maxSizeX - sizex;
    const int eachColumnAdd = sizeXLeft / tableColumnBaseWidth.size();
    for(size_t i = 0; i < tableColumnBaseWidth.size(); i++)
       ui->lowerTable->setColumnWidth(i, tableColumnBaseWidth[i] + eachColumnAdd);

    const int sizeYLeft = maxSizeY - sizey;
    const int eachRowAdd = sizeYLeft / tableRowBaseHeight.size();
    for(size_t i = 0; i < tableRowBaseHeight.size(); i++)
       ui->lowerTable->setRowHeight(i, tableRowBaseHeight[i] + eachRowAdd);
}
