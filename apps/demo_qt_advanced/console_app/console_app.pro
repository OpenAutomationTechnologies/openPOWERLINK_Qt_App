
QT       += core

QT       -= gui

TARGET = console_app
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

release:DESTDIR = ../console_app/release
release:OBJECTS_DIR = ../console_app/release/.obj
release:MOC_DIR = ../console_app/release/.moc
release:RCC_DIR = ../console_app/release/.rcc
release:UI_DIR = ../console_app/release/.ui

debug:DESTDIR = ../console_app/debug
debug:OBJECTS_DIR = ../console_app/debug/.obj
debug:MOC_DIR = ../console_app/debug/.moc
debug:RCC_DIR = ../console_app/debug/.rcc
debug:UI_DIR = ../console_app/debug/.ui

QMAKE_CXXFLAGS += -Wall
unix:QMAKE_CXXFLAGS += -pthread -Wextra -pedantic
DEFINES += _CONSOLE
#if PCAP is used. Un comment the following
DEFINES += CONFIG_USE_PCAP
unix:DEFINES += _GNU_SOURCE

CONFIG += no_lflags_merge
###########################INCLUDEPATH######################################################

INCLUDEPATH +=  ../plk_qt_api/include \
				$$PWD/../../../contrib \
				$$PWD/../../../stack/include \
				$$PWD/../../../apps/common/src

win32: INCLUDEPATH += $$PWD/../../../stack/proj/windows/liboplkmn \
					   $$PWD/../../../contrib/pcap/windows/WpdPack/Include

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
unix:!macx: LIBS += -lpcap -lpthread -lrt


###########################PRE_TARGETDEPS######################################################

unix:!macx:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../stack/lib/linux/i686/liboplkmn_d.a
unix:!macx: PRE_TARGETDEPS += $$PWD/../plk_qt_api/debug/libPlkQtApi.a


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
else:unix:!macx: DEPENDPATH += /usr/lib/i386-linux-gnu


###########################HEADERS######################################################

HEADERS += \
		include/Receiver.h \
		include/ConsoleReader.h

###########################SOURCES######################################################
SOURCES += \
		   src/main.cpp \
		   src/Receiver.cpp \
		   src/ConsoleReader.cpp

#if pacp is used
SOURCES += $$PWD/../../../apps/common/src/pcap-console.c

unix: SOURCES += $$PWD/../../../apps/common/src/system/system-linux.c \
					$$PWD/../../../contrib/console/console-linux.c
else:win32: SOURCES += $$PWD/../../../contrib/console/console-windows.c

