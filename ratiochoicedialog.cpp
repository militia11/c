#include "ratiochoicedialog.h"
#include "ui_ratiochoicedialog.h"

RatioChoiceDialog::RatioChoiceDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowTitleHint)
    , chosenRatioTableRowIdx(2)
    , chosenActualDataVisRow(0)
    , ui(new Ui::RatioChoiceDialog)
{
  ui -> setupUi(this) ;

  connect(ui->rmaxButton, SIGNAL(clicked()), this, SLOT(maxChosen()));
  connect(ui->rmediumButton, SIGNAL(clicked()), this, SLOT(medChosen()));
  connect(ui->rminimalButton, SIGNAL(clicked()), this, SLOT(minChosen()));
  connect(ui->rxxButton, SIGNAL(clicked()), this, SLOT(xxChosen()));
  connect(ui->rxxxButton, SIGNAL(clicked()), this, SLOT(xxxChosen()));
}

RatioChoiceDialog::~RatioChoiceDialog()
{
  delete ui ;
}

void RatioChoiceDialog::maxChosen()
{
    chosenRatioTableRowIdx = 2;
    chosenActualDataVisRow = 0;
    this->close();
}

void RatioChoiceDialog::medChosen()
{
    chosenRatioTableRowIdx = 3;
    chosenActualDataVisRow = 1;
    this->close();
}

void RatioChoiceDialog::minChosen()
{
    chosenRatioTableRowIdx = 4;
    chosenActualDataVisRow = 2;
    this->close();
}

void RatioChoiceDialog::xxChosen()
{
    chosenRatioTableRowIdx = 5;
    chosenActualDataVisRow = 3;
    this->close();
}

void RatioChoiceDialog::xxxChosen()
{
    chosenRatioTableRowIdx = 6;
    chosenActualDataVisRow = 4;
    this->close();
}
