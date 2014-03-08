#-------------------------------------------------
#
# Project created by QtCreator 2014-03-05T14:31:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plk_qt_gui
TEMPLATE = app

################################TargetInstallPath#######################################
release:DESTDIR = ../plk_qt_gui/release
release:OBJECTS_DIR = ../plk_qt_gui/release/.obj
release:MOC_DIR = ../plk_qt_gui/release/.moc
release:RCC_DIR = ../plk_qt_gui/release/.rcc
release:UI_DIR = ../plk_qt_gui/release/.ui

debug:DESTDIR = ../plk_qt_gui/debug
debug:OBJECTS_DIR = ../plk_qt_gui/debug/.obj
debug:MOC_DIR = ../plk_qt_gui/debug/.moc
debug:RCC_DIR = ../plk_qt_gui/debug/.rcc
debug:UI_DIR = ../plk_qt_gui/debug/.ui

INCLUDEPATH += include

win32: INCLUDEPATH +=  $$PWD/../../../contrib/pcap/windows/WpdPack/Include

win32: LIBS += -L$$PWD/../../../contrib/pcap/windows/WpdPack/Lib/ -lwpcap \
			   -L$$PWD/../../../contrib/pcap/windows/WpdPack/Lib/ -lPacket

win32: DEPENDPATH +=  $$PWD/../../../contrib/pcap/windows/WpdPack/Lib

SOURCES += src/main.cpp \
		   src/MainWindow.cpp \
	src/LogerWindow.cpp \
	src/SdoTransfer.cpp \
	src/ProcessImageVariables.cpp \
	src/ProcessImageMemory.cpp \
	src/DialogOpenCdc.cpp \
	src/SelectNwInterfaceDialog.cpp \
	src/NmtCommandsDock.cpp \
	src/NodeStatusDock.cpp \
	src/Node.cpp \
	src/Channel.cpp

HEADERS  += include/MainWindow.h \
	include/LogerWindow.h \
	include/SdoTransfer.h \
	include/ProcessImageVariables.h \
	include/ProcessImageMemory.h \
	include/DialogOpenCdc.h \
	include/SelectNwInterfaceDialog.h \
	include/NmtCommandsDock.h \
	include/NodeStatusDock.h \
	include/Node.h \
	include/Channel.h

FORMS    += ui/MainWindow.ui \
	ui/MainWindow.ui \
	ui/LogerWindow.ui \
	ui/SdoTransfer.ui \
	ui/ProcessImageVariables.ui \
	ui/ProcessImageMemory.ui \
	ui/DialogOpenCdc.ui \
	ui/SelectNwInterfaceDialog.ui \
	ui/NmtCommandsDock.ui \
	ui/NodeStatusDock.ui \
	ui/Channel.ui

RESOURCES += \
	ui/resources/images.qrc
