QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./QXlsx          # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./QXlsx/source/  # current QXlsx source path is ./source/
include(./QXlsx/QXlsx.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    measurements.cpp \
    setcategoriesdialog.cpp \
    simplediagramfunction.cpp

HEADERS += \
    enumerations.h \
    mainwindow.h \
    measurements.h \
    setcategoriesdialog.h \
    simplediagramfunction.h

FORMS += \
    mainwindow.ui \
    setcategoriesdialog.ui \
    simplediagramfunction.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
