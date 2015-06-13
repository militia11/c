#include <QApplication>
#include <QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QTextCodec::codecForName("UTF-8");

  MainWindow w;
  w.show();

  return app.exec();
}
