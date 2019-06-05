QT += core network

CONFIG *= c++11

HEADERS += \
    $$PWD/snap7.h \
    $$PWD/ocv_plc_define.h \
    $$PWD/siemensplcdevice.h \
    $$PWD/siemensplcdevice_1.h \
    $$PWD/siemensplcdevice_2.h \
    $$PWD/siemensplcdevice_3.h \
    $$PWD/siemensplcdevicemanager.h

SOURCES += \
    $$PWD/siemensplcdevice.cpp \
    $$PWD/snap7.cpp \
    $$PWD/siemensplcdevice_1.cpp \
    $$PWD/siemensplcdevice_2.cpp \
    $$PWD/siemensplcdevice_3.cpp \
    $$PWD/siemensplcdevicemanager.cpp

INCLUDEPATH += $$PWD
LIBPATH     += $$PWD

LIBS += -L$$PWD -lsnap7

DEFINES += _TTY_WIN_  WINVER=0x0501 \
    _USE_32BIT_TIME_T
LIBS += -Ldll -lsetupapi -lws2_32
