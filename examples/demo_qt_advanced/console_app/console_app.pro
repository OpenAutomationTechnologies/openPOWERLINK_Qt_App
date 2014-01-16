
QT       += core

QT       -= gui

TARGET = console_app
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

INCLUDEPATH +=  ../plk_qt_api/include \
			$$PWD/../../../stack/make/lib/libpowerlink \
			$$PWD/../../../include \
			$$PWD/../../../libs \
			$$PWD/../../../libs/ami \
			$$PWD/../../../libs/pcap/windows/WpdPack/Include \
			$$PWD/../../../examples/common \
			$$PWD/../../../examples/common/system

win32: LIBS += -L$$PWD/../build-plk_qt_api-Qt520_vs2010-Debug/debug -lPlkQtApi \
				-L$$PWD/../../../build/stack/make/lib/libpowerlink/Debug -lpowerlink \
				-L$$PWD/../../../libs/pcap/windows/WpdPack/Lib -lwpcap \
				-L$$PWD/../../../libs/pcap/windows/WpdPack/Lib -lPacket

INCLUDEPATH += $$PWD/../build-plk_qt_api-Qt520_vs2010-Debug/debug \
				$$PWD/../../../build/stack/make/lib/libpowerlink/Debug \
				$$PWD/../../../libs/pcap/windows/WpdPack/Lib

DEPENDPATH += $$PWD/../build-plk_qt_api-Qt520_vs2010-Debug/debug \
				$$PWD/../../../build/stack/make/lib/libpowerlink/Debug \
				$$PWD/../../../libs/pcap/windows/WpdPack/Lib

HEADERS += \
	include/Receiver.h
SOURCES += \
	src/main.cpp \
	src/Receiver.cpp
