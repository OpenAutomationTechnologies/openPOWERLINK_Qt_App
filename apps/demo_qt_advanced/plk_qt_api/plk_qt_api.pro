
QT       += core xml

QT       -= gui

TARGET = PlkQtApi
TEMPLATE = lib

DEFINES += PLKQTAPI_LIB CONFIG_USE_PCAP CONFIG_CFM CONFIG_OPENCONFIGURATOR_MAPPING CONFIG_MN

INCLUDEPATH += include \
               $$PWD/../../../stack/include

win32:CONFIG(release, debug|release): INCLUDEPATH += $$PWD/../../../stack/proj/windows/liboplkmn
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += $$PWD/../../../stack/proj/windows/liboplkmn

HEADERS += include/api/OplkEventHandler.h \
           include/api/OplkQtApi.h \
           include/common/Utility.h \
           include/common/QtApiGlobal.h \
           include/api/ReceiverContext.h \
           include/user/SdoTransferJob.h \
           include/user/SdoTransferResult.h \
           include/user/processimage/Direction.h \
           include/user/processimage/IECDataType.h \
           include/user/processimage/Channel.h \
           include/user/processimage/ProcessImage.h \
           include/user/processimage/ProcessImageIn.h \
           include/user/processimage/ProcessImageOut.h \
           include/user/processimage/ProcessImageParser.h \
           include/user/processimage/ProcessImageParserType.h \
           include/user/processimage/QtProcessImageParser.h

SOURCES += src/api/OplkEventHandler.cpp \
           src/api/OplkQtApi.cpp \
           src/api/ReceiverContext.cpp \
           src/user/SdoTransferJob.cpp \
           src/user/SdoTransferResult.cpp \
           src/user/processimage/Channel.cpp \
           src/user/processimage/ProcessImage.cpp \
           src/user/processimage/ProcessImageIn.cpp \
           src/user/processimage/ProcessImageOut.cpp \
           src/user/processimage/ProcessImageParser.cpp \
           src/user/processimage/QtProcessImageParser.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../stack/lib/windows/x86 -loplkmn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../stack/lib/windows/x86 -loplkmn_d

win32:CONFIG(release, debug|release): DEPENDPATH += $$PWD/../../../stack/lib/windows/x86
else:win32:CONFIG(debug, debug|release): DEPENDPATH += $$PWD/../../../stack/lib/windows/x86
