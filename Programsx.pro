#-------------------------------------------------
#
# Project created by QtCreator 2013-10-11T17:30:50
#
#-------------------------------------------------

QT += core gui serialport printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = programsx
TEMPLATE = app


SOURCES += \
	main.cpp \
	formdialog.cpp \
	normdialog.cpp \
	printingformdialog.cpp \
	diagnosticwidget.cpp \
	slidingstackedwidget.cpp \
	mainwindow.cpp \
	menuwidget.cpp \
	databasewidget.cpp \
	portconfigwidget.cpp \
	datawidget.cpp \
	resultwidget.cpp \
	ratiochoicedialog.cpp \
    hwcomio.cpp

HEADERS  += \
	formdialog.h \
	normdialog.h \
	printingformdialog.h \
	diagnosticwidget.h \
	slidingstackedwidget.h \
	mainwindow.h \
	menuwidget.h \
	databasewidget.h \
	portconfigwidget.h \
	datawidget.h \
	resultwidget.h \
	ownerInfoType.h \
	ratiochoicedialog.h \
    hwcomio.h

FORMS    += \
	formdialog.ui \
	portconfigwidget.ui \
	normdialog.ui \
	resultwidget.ui \
	printingformdialog.ui \
	diagnosticwidget.ui \
	mainwindow.ui \
	menuwidget.ui \
	databasewidget.ui \
	datawidget.ui \
	ratiochoicedialog.ui

RESOURCES = \
	resources.qrc

RC_FILE = program.rc

QMAKE_CXXFLAGS += -std=c++0x
CXXFLAGS += -std=c++0x
