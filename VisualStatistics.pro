QT       += core gui widgets
QT       += printsupport
QT += webenginewidgets webchannel positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

win32-msvc* {
    QMAKE_CXXFLAGS_RELEASE -= /permissive-
    QMAKE_CXXFLAGS_RELEASE += /permissive
    QMAKE_CXXFLAGS += /Zc:__cplusplus
    DEFINES += _CRT_SECURE_NO_WARNINGS
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    math/distributionmodel.cpp \
    pages/detailpage.cpp \
    pages/networkpage.cpp \
    pages/welcomepage.cpp \
    qcustomplot.cpp

HEADERS += \
    mainwindow.h \
    math/distributionmodel.h \
    pages/detailpage.h \
    pages/networkpage.h \
    pages/welcomepage.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui \
    pages/detailpage.ui \
    pages/networkpage.ui \
    pages/welcomepage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
