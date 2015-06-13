#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSignalMapper>
#include <QString>

#include "slidingstackedwidget.h"
#include "diagnosticwidget.h"
#include "databasewidget.h"
#include "portconfigwidget.h"
#include "ownerInfoType.h"

namespace Ui
{
  class MenuWidget;
}

class MenuWidget : public QWidget
{
  Q_OBJECT

  public:
    explicit MenuWidget(QWidget* parent = 0);
    ~MenuWidget();
    bool shouldOpenSettingsOnStart;

  public slots:
    void onDbWidgetClosed();
    void onConfigWidgetClosed();
    void search();
    void reloadHwLists();

  private slots:
    void changeCompany(QString company);
    void changeType(QString type);
    void ownerInfoChanged();

  signals:
    void hwListsChanged();

  private:
    ownerInfoType readOwnerInfoFile();
    void writeOwnerInfoFile(ownerInfoType info);

  private:
    Ui::MenuWidget* ui;
    QStringList portsList;
    QSerialPort* port;
    SlidingStackedWidget* stackedWidget;
    QSignalMapper* signalMapper;
    DiagnosticWidget* diagnostic;
    DatabaseWidget* dbWidget;
    PortConfigWidget* configWidget;
};

#endif
