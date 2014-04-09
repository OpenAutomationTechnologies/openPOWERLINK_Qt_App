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
CONFIG(release, debug|release) {
	DESTDIR = ../plk_qt_gui/release
	OBJECTS_DIR = release/.obj
	MOC_DIR = release/.moc
	RCC_DIR = release/.rcc
	UI_DIR = release/.ui
}
CONFIG(debug, debug|release) {
	DESTDIR = ../plk_qt_gui/debug
	OBJECTS_DIR = debug/.obj
	MOC_DIR = debug/.moc
	RCC_DIR = debug/.rcc
	UI_DIR = debug/.ui
}

INCLUDEPATH += include \
			$$PWD/../plk_qt_api/include \
			$$PWD/../../../stack/include

#oplkCfg.h
win32: INCLUDEPATH += $$PWD/../../../stack/proj/windows/liboplkmn
######Link to application##########
unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmn
##########for userspace-demon###########
#unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmnapp-userintf
#######link to kernel space-demon#######
#unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmnapp-kernelintf

SOURCES += src/main.cpp \
		   src/MainWindow.cpp \
	src/SdoTransfer.cpp \
	src/ProcessImageVariables.cpp \
	src/ProcessImageMemory.cpp \
	src/DialogOpenCdc.cpp \
	src/SelectNwInterfaceDialog.cpp \
	src/NmtCommandsDock.cpp \
	src/NodeStatusDock.cpp \
	src/LoggerWindow.cpp \
	src/ChannelUi.cpp \
	src/NodeUi.cpp \
	src/AboutDialog.cpp \
	src/DataSyncThread.cpp \
	src/StatusBar.cpp

HEADERS  += include/MainWindow.h \
	include/SdoTransfer.h \
	include/ProcessImageVariables.h \
	include/ProcessImageMemory.h \
	include/DialogOpenCdc.h \
	include/SelectNwInterfaceDialog.h \
	include/NmtCommandsDock.h \
	include/NodeStatusDock.h \
	include/LoggerWindow.h \
	include/ChannelUi.h \
	include/NodeUi.h \
	include/AboutDialog.h \
	include/DataSyncThread.h \
	include/StatusBar.h

FORMS    += ui/MainWindow.ui \
	ui/MainWindow.ui \
	ui/SdoTransfer.ui \
	ui/ProcessImageVariables.ui \
	ui/ProcessImageMemory.ui \
	ui/DialogOpenCdc.ui \
	ui/SelectNwInterfaceDialog.ui \
	ui/NmtCommandsDock.ui \
	ui/NodeStatusDock.ui \
	ui/LoggerWindow.ui \
	ui/ChannelUi.ui \
	ui/AboutDialog.ui

RESOURCES += \
	ui/resources/images.qrc

INCLUDEPATH += $$PWD/../plk_qt_api/debug
DEPENDPATH += $$PWD/../plk_qt_api/debug


######Link to application##########
unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmn
##########for userspace-demon###########
#unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmnapp-userintf
#######link to kernel space-demon#######
#unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmnapp-kernelintf


###########################LIB-PATH######################################################

CONFIG(release, debug|release): LIBS += -L$$PWD/../plk_qt_api/release/ -lPlkQtApi
CONFIG(debug, debug|release): LIBS += -L$$PWD/../plk_qt_api/debug/ -lPlkQtApi

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../stack/lib/windows/x86 -loplkmn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../stack/lib/windows/x86 -loplkmn_d
######Link to application##########
else:unix:!macx:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../stack/lib/linux/i686/ -loplkmn
else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../stack/lib/linux/i686/ -loplkmn_d
##########for userspace-demon###########
#else:unix:!macx:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../stack/lib/linux/i686/ -loplkmnapp-userintf
#else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../stack/lib/linux/i686/ -loplkmnapp-userintf_d
#######link to kernel space-demon#######
#else:unix:!macx:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../stack/lib/linux/i686/ -loplkmnapp-kernelintf
#else:unix:!macx:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../stack/lib/linux/i686/ -loplkmnapp-kernelintf_d


win32: LIBS += -L$$PWD/../../../contrib/pcap/windows/WpdPack/Lib/ -lwpcap \
			   -L$$PWD/../../../contrib/pcap/windows/WpdPack/Lib/ -lPacket

###########################PRE_TARGETDEPS######################################################
unix:!macx:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../stack/lib/linux/i686/liboplkmn_d.a
unix:!macx:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../plk_qt_api/debug/libPlkQtApi.a


###########################LIB-INCLUDE-PATH######################################################

CONFIG(release, debug|release): INCLUDEPATH += $$PWD/../plk_qt_api/release
CONFIG(debug, debug|release): INCLUDEPATH += $$PWD/../plk_qt_api/debug

win32:CONFIG(release, debug|release): INCLUDEPATH += $$PWD/../../../stack/lib/windows/x86
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += $$PWD/../../../stack/lib/windows/x86
else:unix:!macx: INCLUDEPATH += $$PWD/../../../stack/lib/linux/i686

win32: INCLUDEPATH += $$PWD/../../../contrib/pcap/windows/WpdPack/Include


###########################DEPENDPATH######################################################
CONFIG(release, debug|release): DEPENDPATH += $$PWD/../plk_qt_api/release
CONFIG(debug, debug|release): DEPENDPATH += $$PWD/../plk_qt_api/debug

win32: DEPENDPATH += $$PWD/../../../stack/lib/windows/x86 \
			  $$PWD/../../../contrib/pcap/windows/WpdPack/Lib

unix:!macx: LIBS += -lpcap -lrt -lpthread
