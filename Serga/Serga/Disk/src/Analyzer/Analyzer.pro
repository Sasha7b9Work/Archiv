######################################################################
# Automatically generated by qmake (2.01a) Wed Apr 11 10:54:24 2012
######################################################################

CONFIG += shared qt release
TEMPLATE = app
TARGET = 
DEPENDPATH += . src
INCLUDEPATH += .
DESTDIR = ../Release/
DESTDIR_TARGET = ../Release/

# Input
HEADERS += src/Cursors.h \
           src/Display.h \
           src/MathDisplayAnalyzer.h \
           src/SettingsDisplay.h \
           src/SignalScreen.h \
           src/Strings.h \
           src/WindowAnalyzer.h \
           ../common/AbstractSettingsDisplay.h \
           ../common/defines.h \
           ../common/GlobalFunctions.h \
           ../common/TablesOfConverting.h \
           ../common/TypesAltera.h \
           ../Device129Multi/src/Device129Multi.h \
           ../common/Configurator.h \
           ../Device129Multi/src/AbstractAltera.h \
           ../Device129Multi/src/SettingsAltera.h \
           ../Device129Multi/src/ComPort.h \
           ../common/Timer.h \
           ../common/Time.h \
           ../Device129Multi/src/AddressSpaceAltera.h
FORMS += src/WindowAnalyzer.ui
SOURCES += src/UnityAnalyzer.cpp
RESOURECS += resources.qrc
RC_FILE = analyzer.rc