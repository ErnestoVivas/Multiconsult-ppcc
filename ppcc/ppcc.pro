QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./QXlsx          # current QXlsx path is . (. means current directory)
QXLSX_HEADERPATH=./QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./QXlsx/source/  # current QXlsx source path is ./source/
include(./QXlsx/QXlsx.pri)

SOURCES += \
    categoriestracking.cpp \
    configdiagram.cpp \
    enumerations.cpp \
    exportdiagramdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    measurements.cpp \
    sectorandtotalnatdiagram.cpp \
    sectordayanalysis.cpp \
    sectorsubcatsanalysis.cpp \
    sectorweekanalysis.cpp \
    selectfunction.cpp \
    selectimportmethod.cpp \
    setcategoriesdialog.cpp \
    simplediagramfunction.cpp \
    siteanalysis.cpp

HEADERS += \
    categoriestracking.h \
    configdiagram.h \
    enumerations.h \
    exportdiagramdialog.h \
    mainwindow.h \
    measurements.h \
    sectorandtotalnatdiagram.h \
    sectordayanalysis.h \
    sectorsubcatsanalysis.h \
    sectorweekanalysis.h \
    selectfunction.h \
    selectimportmethod.h \
    setcategoriesdialog.h \
    simplediagramfunction.h \
    siteanalysis.h

FORMS += \
    configdiagram.ui \
    exportdiagramdialog.ui \
    mainwindow.ui \
    sectorandtotalnatdiagram.ui \
    sectordayanalysis.ui \
    sectorsubcatsanalysis.ui \
    sectorweekanalysis.ui \
    selectfunction.ui \
    selectimportmethod.ui \
    setcategoriesdialog.ui \
    simplediagramfunction.ui \
    siteanalysis.ui

RC_ICONS = logo_multiconsult.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
