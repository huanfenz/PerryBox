QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# fmt library
INCLUDEPATH += $$PWD/thirdparty/fmt-12.1.0/include
SOURCES += $$PWD/thirdparty/fmt-12.1.0/src/format.cc

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD/src \
    $$PWD/src/core \
    $$PWD/src/ui

SOURCES += \
    src/core/ascii_converter.cpp \
    src/core/crc_calculator.cpp \
    src/core/perry_common.cpp \
    src/core/regex_tester.cpp \
    src/core/timestamp_converter.cpp \
    src/main.cpp \
    src/ui/ascii_converter_page.cpp \
    src/ui/byte_warn_highlighter.cpp \
    src/ui/crc_calculator_page.cpp \
    src/ui/mainwindow.cpp \
    src/ui/regex_tester_page.cpp \
    src/ui/timestamp_converter_page.cpp

HEADERS += \
    src/core/ascii_converter.h \
    src/core/crc_calculator.h \
    src/core/perry_common.h \
    src/core/regex_tester.h \
    src/core/timestamp_converter.h \
    src/ui/ascii_converter_page.h \
    src/ui/byte_warn_highlighter.h \
    src/ui/crc_calculator_page.h \
    src/ui/mainwindow.h \
    src/ui/regex_tester_page.h \
    src/ui/timestamp_converter_page.h

FORMS += \
    src/ui/ascii_converter_page.ui \
    src/ui/crc_calculator_page.ui \
    src/ui/mainwindow.ui \
    src/ui/regex_tester_page.ui \
    src/ui/timestamp_converter_page.ui

TRANSLATIONS += \
    PerryBox_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = BirdIcon4.ico
