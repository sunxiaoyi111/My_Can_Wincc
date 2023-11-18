QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Candispatcher/can_dispatcher.cpp \
    m_can_task.cpp \
    m_canconfig.cpp \
    m_candatabase.cpp \
    m_candatasql.cpp \
    main.cpp \
    m_canwidget.cpp \
    projectwindow.cpp

HEADERS += \
    Candispatcher/can_dispatcher.h \
    Candispatcher/can_dispatcher_table.h \
    canlib/zlgcan/zlgcan_x64/canframe.h \
    canlib/zlgcan/zlgcan_x64/config.h \
    canlib/zlgcan/zlgcan_x64/typedef.h \
    canlib/zlgcan/zlgcan_x64/zlgcan.h \
    canlib/zlgcan/zlgcan_x86/canframe.h \
    canlib/zlgcan/zlgcan_x86/config.h \
    canlib/zlgcan/zlgcan_x86/typedef.h \
    canlib/zlgcan/zlgcan_x86/zlgcan.h \
    m_can_task.h \
    m_canconfig.h \
    m_candatabase.h \
    m_candatasql.h \
    m_canwidget.h \
    projectwindow.h \
    util_include/mc_util_include.h

FORMS += \
    m_canwidget.ui \
    projectwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#contains(QT_ARCH, i386) {
#    #message("32-bit")
#    CONFIG(debug,debug|release){
#        LIBS += -L$$PWD/canlib/zlgcan/zlgcan_x86/ -lzlgcan
#    }
#    else{
#        LIBS += -L$$PWD/canlib/zlgcan/zlgcan_x86/ -lzlgcan
#    }
#} else {
#    #message("64-bit")
#    CONFIG(debug,debug|release){
        LIBS += -L$$PWD/canlib/zlgcan/zlgcan_x64/ -lzlgcan
#    }
#    else{
#        LIBS += -L$$PWD/canlib/zlgcan/zlgcan_x64/ -lzlgcan
#    }
#}

#INCLUDEPATH += $$PWD/canlib/zlgcan/zlgcan_x64
DEPENDPATH += $$PWD/canlib/zlgcan/zlgcan_x64


INCLUDEPATH += $$PWD/util_include


#禁用-Wunknown-pragmas 报警
#CONFIG += warn_off
#QMAKE_CXXFLAGS += -Wall
#QMAKE_CXXFLAGS += -Wno-unknown-pragmas

CONFIG      += console
