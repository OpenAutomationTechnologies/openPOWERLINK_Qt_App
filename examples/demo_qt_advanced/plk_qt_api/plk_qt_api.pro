
QT       += core xml

QT       -= gui

TARGET = PlkQtApi
TEMPLATE = lib

DEFINES += PLKQTAPI_LIB CONFIG_USE_PCAP CONFIG_CFM

INCLUDEPATH += include \
			$$PWD/../../../include \
			$$PWD/../../../libs \
			$$PWD/../../../libs/ami \
			$$PWD/../../../libs/pcap/windows/WpdPack/Include \
			$$PWD/../../../stack/make/lib/libpowerlink \
			$$PWD/../../../build/stack/make/lib/libpowerlink/Debug

HEADERS += include/api/OplkEventHandler.h \
			include/api/OplkQtApi.h \
			include/common/global.h

SOURCES += src/api/OplkEventHandler.cpp \
			src/api/OplkQtApi.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../build/stack/make/lib/libpowerlink/Release/ -lpowerlink
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../build/stack/make/lib/libpowerlink/Debug/ -lpowerlink

win32:CONFIG(release, debug|release): DEPENDPATH += $$PWD/../../../build/stack/make/lib/libpowerlink/Release
else:win32:CONFIG(debug, debug|release): DEPENDPATH += $$PWD/../../../build/stack/make/lib/libpowerlink/Debug
