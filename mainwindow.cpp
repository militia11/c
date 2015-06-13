#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menuwidget.h"

#include <QWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , animTimeMs(500)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);

    QWidget* centralWidget =new QWidget(this);
    centralWidget->setContentsMargins(0,0,0,0);
    QVBoxLayout* mainLayout =new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);

    stackedWidget =new SlidingStackedWidget(centralWidget);
    stackedWidget->setSpeed(animTimeMs);
    stackedWidget->setWrap(false);
    stackedWidget->setVerticalMode(false);
    mainLayout->addWidget(stackedWidget);

    MenuWidget* mainWidget =new MenuWidget(stackedWidget);
    stackedWidget->addWidget(MENU_WIDGET, mainWidget);
    if(mainWidget->shouldOpenSettingsOnStart)
        stackedWidget->setCurrentIndex(stackedWidget->mappedViews[CONFIG_WIDGET]);//set as entry view
    else
        stackedWidget->setCurrentIndex(stackedWidget->mappedViews[MENU_WIDGET]);//set as entry view
}

MainWindow::~MainWindow()
{
    delete ui;
}
