TEMPLATE = app
QT += core \
	gui \
	declarative \
	sql
CONFIG += console

SOURCES += main.cpp \
	config.cpp \
	listmodel.cpp \
	gameromitem.cpp \
	gamerommodel.cpp \
	emulatoritem.cpp \
	emulatormodel.cpp \
	gameprocess.cpp \
	soundengine.cpp \
	keyboardmap.cpp \
    systemmodel.cpp \
    systemitem.cpp \
    exception.cpp \
    systemprocess.cpp

HEADERS += \
	config.h \
	modelitem.h \
	listmodel.h \
	gameromitem.h \
	gamerommodel.h \
	main.h \
	emulatoritem.h \
	emulatormodel.h \
	gameprocess.h \
	soundengine.h \
	keyboardmap.h \
	qtkeymap.h \
    systemmodel.h \
    systemitem.h \
    exception.h \
    systemprocess.h

TARGET = llamaui

OTHER_FILES += \
    SystemView.qml \
    SystemModel.qml \
    SystemDelegate.qml

