
QT       += core

QT       -= gui

TARGET = console_app
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

DEFINES += CONFIG_USE_PCAP _CONSOLE

INCLUDEPATH +=  ../plk_qt_api/include \
                $$PWD/../../../contrib \
                $$PWD/../../../stack/include \
                $$PWD/../../../apps/common/src \
                $$PWD/../../../stack/proj/windows/liboplkmn \
                $$PWD/../../../contrib/pcap/windows/WpdPack/Include 

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-plk_qt_api-Qt520_vs2010-Debug/debug -lPlkQtApi \
                                              -L$$PWD/../../../stack/lib/windows/x86 -loplkmn \
                                              -L$$PWD/../../../contrib/pcap/windows/WpdPack/Lib/ -lwpcap \
                                              -L$$PWD/../../../contrib/pcap/windows/WpdPack/Lib/ -lPacket

win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-plk_qt_api-Qt520_vs2010-Debug/debug -lPlkQtApi \
                                            -L$$PWD/../../../stack/lib/windows/x86 -loplkmn_d \
                                            -L$$PWD/../../../contrib/pcap/windows/WpdPack/Lib/ -lwpcap \
                                            -L$$PWD/../../../contrib/pcap/windows/WpdPack/Lib/ -lPacket

INCLUDEPATH += $$PWD/../build-plk_qt_api-Qt520_vs2010-Debug/debug \
               $$PWD/../../../build/stack/make/lib/libpowerlink/Debug \
               $$PWD/../../../contrib/pcap/windows/WpdPack/Include

DEPENDPATH += $$PWD/../build-plk_qt_api-Qt520_vs2010-Debug/debug \
              $$PWD/../../../stack/lib/windows/x86 \
              $$PWD/../../../contrib/pcap/windows/WpdPack/Lib

HEADERS += \
        include/Receiver.h \
        include/ConsoleReader.h

SOURCES += \
           src/main.cpp \
           src/Receiver.cpp \
           src/ConsoleReader.cpp \
           $$PWD/../../../contrib/console/console-windows.c \
           $$PWD/../../../apps/common/src/pcap-console.c
