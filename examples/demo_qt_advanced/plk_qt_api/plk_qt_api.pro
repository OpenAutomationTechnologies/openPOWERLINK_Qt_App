
QT       += core xml

QT       -= gui

TARGET = PlkQtApi
TEMPLATE = lib

DEFINES += PLKQTAPI_LIB CONFIG_USE_PCAP CONFIG_CFM CONFIG_OPENCONFIGURATOR_MAPPING CONFIG_MN

INCLUDEPATH += include \
			$$PWD/../../../include \
			$$PWD/../../../stack/make/lib/libpowerlink \
			$$PWD/../../../libs \
			$$PWD/../../../libs/ami \
			$$PWD/../../../libs/pcap/windows/WpdPack/Include \
			$$PWD/../../../build/stack/make/lib/libpowerlink/Debug

HEADERS += include/api/OplkEventHandler.h \
			include/api/OplkQtApi.h \
			include/common/QtApiGlobal.h \
			include/api/ReceiverContext.h \
			include/user/SdoTransferJob.h \
			include/user/SdoTransferResult.h \
			include/user/processimage/Direction.h \
			include/user/processimage/IECDataType.h \
			include/user/processimage/Channel.h

SOURCES += src/api/OplkEventHandler.cpp \
			src/api/OplkQtApi.cpp \
			src/api/ReceiverContext.cpp \
			src/user/SdoTransferJob.cpp \
			src/user/SdoTransferResult.cpp \
			src/user/processimage/Channel.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../build/stack/make/lib/libpowerlink/Release/ -lpowerlink
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../build/stack/make/lib/libpowerlink/Debug/ -lpowerlink

win32:CONFIG(release, debug|release): DEPENDPATH += $$PWD/../../../build/stack/make/lib/libpowerlink/Release
else:win32:CONFIG(debug, debug|release): DEPENDPATH += $$PWD/../../../build/stack/make/lib/libpowerlink/Debug
