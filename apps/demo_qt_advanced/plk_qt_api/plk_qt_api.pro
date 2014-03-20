
QT       += core xml

QT       -= gui

TARGET = PlkQtApi
TEMPLATE = lib

QMAKE_CXXFLAGS += -Wall
unix: QMAKE_CXXFLAGS += -Wextra -pedantic
#Compile as static lib for unix
unix:CONFIG += staticlib

DEFINES += PLKQTAPI_LIB CONFIG_MN


################################TargetInstallPath#######################################

CONFIG(release, debug|release) {
	DESTDIR = ../plk_qt_api/release
	OBJECTS_DIR = ../plk_qt_api/release/.obj
	MOC_DIR = ../plk_qt_api/release/.moc
	RCC_DIR = ../plk_qt_api/release/.rcc
	UI_DIR = ../plk_qt_api/release/.ui
}
CONFIG(debug, debug|release) {
	DESTDIR = ../plk_qt_api/debug
	OBJECTS_DIR = ../plk_qt_api/debug/.obj
	MOC_DIR = ../plk_qt_api/debug/.moc
	RCC_DIR = ../plk_qt_api/debug/.rcc
	UI_DIR = ../plk_qt_api/debug/.ui
}

###########################INCLUDEPATH######################################################
INCLUDEPATH += include \
			   $$PWD/../../../stack/include

win32:CONFIG(release, debug|release): INCLUDEPATH += $$PWD/../../../stack/proj/windows/liboplkmn
else:win32:CONFIG(debug, debug|release): INCLUDEPATH += $$PWD/../../../stack/proj/windows/liboplkmn
######Link to application##########
unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmn
##########for userspace-demon###########
#unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmnapp-userintf
#######link to kernel space-demon#######
#unix:!macx: INCLUDEPATH += $$PWD/../../../stack/proj/linux/liboplkmnapp-kernelintf

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
		   include/user/processimage/QtProcessImageParser.h \
		   include/common/XmlParserException.h

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
		   src/user/processimage/QtProcessImageParser.cpp \
		   src/common/Utility.cpp \
		   src/user/processimage/IECDataType.cpp \
		   src/common/XmlParserException.cpp

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

win32:CONFIG(release, debug|release): DEPENDPATH += $$PWD/../../../stack/lib/windows/x86
else:win32:CONFIG(debug, debug|release): DEPENDPATH += $$PWD/../../../stack/lib/windows/x86
else:unix:!macx: DEPENDPATH += $$PWD/../../../stack/lib/linux/i686

unix: INCLUDEPATH += $$PWD/../../../stack/lib/linux/i686

###########################PRE_TARGETDEPS######################################################

######Link to application##########
unix:!macx:CONFIG(release, debug|release) PRE_TARGETDEPS += $$PWD/../../../stack/lib/linux/i686/liboplkmn.a
unix:!macx:CONFIG(debug, debug|release) PRE_TARGETDEPS += $$PWD/../../../stack/lib/linux/i686/liboplkmn_d.a
##########for userspace-demon###########
#unix:!macx:CONFIG(release, debug|release) PRE_TARGETDEPS += $$PWD/../../../stack/lib/linux/i686/liboplkmnapp-userintf.a
#unix:!macx:CONFIG(debug, debug|release) PRE_TARGETDEPS += $$PWD/../../../stack/lib/linux/i686/liboplkmnapp-userintf_d.a
#######link to kernel space-demon#######
#unix:!macx:CONFIG(release, debug|release) PRE_TARGETDEPS += $$PWD/../../../stack/lib/linux/i686/liboplkmnapp-kernelintf.a
#unix:!macx:CONFIG(debug, debug|release) PRE_TARGETDEPS += $$PWD/../../../stack/lib/linux/i686/liboplkmnapp-kernelintf_d.a

