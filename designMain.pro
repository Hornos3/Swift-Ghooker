QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20
CONFIG += resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Inject.cpp \
    Util.cpp \
    colorfulmodel.cpp \
    exceptionwidget.cpp \
    filewidget.cpp \
    heapwidget.cpp \
    historytracer.cpp \
    hookanalysis.cpp \
    injectthread.cpp \
    logwidget.cpp \
    main.cpp \
    memorywidget.cpp \
    modulewidget.cpp \
    netwidget.cpp \
    output.cpp \
    regwidget.cpp \
    widget.cpp

HEADERS += \
    Inject.h \
    Util.h \
    colorfulmodel.h \
    exceptionwidget.h \
    filewidget.h \
    heapwidget.h \
    historytracer.h \
    hookanalysis.h \
    injectthread.h \
    logwidget.h \
    memorywidget.h \
    modulewidget.h \
    netwidget.h \
    output.h \
    regwidget.h \
    widget.h

FORMS += \
    exceptionwidget.ui \
    filewidget.ui \
    heapwidget.ui \
    logwidget.ui \
    memorywidget.ui \
    modulewidget.ui \
    netwidget.ui \
    output.ui \
    regwidget.ui \
    widget.ui

LIBS += \
    -LD:/SSdesign/UI/designMain/include

INCLUDEPATH += \
    D:/SSdesign/Detours/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    background.qrc

win32: LIBS += -LD:/SSdesign/Detours/lib.X64/detours.lib

INCLUDEPATH += D:/SSdesign/Detours/lib.X64
DEPENDPATH += D:/SSdesign/Detours/lib.X64

DISTFILES += \
    update.log
