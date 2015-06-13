#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "slidingstackedwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    SlidingStackedWidget* stackedWidget;
    const int animTimeMs;
};

#endif // MAINWINDOW_H
