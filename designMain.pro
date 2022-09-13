QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Inject.cpp \
    Util.cpp \
    colorfulmodel.cpp \
    hookanalysis.cpp \
    injectthread.cpp \
    main.cpp \
    output.cpp \
    widget.cpp

HEADERS += \
    Inject.h \
    Util.h \
    colorfulmodel.h \
    hookanalysis.h \
    injectthread.h \
    output.h \
    widget.h

FORMS += \
    output.ui \
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
