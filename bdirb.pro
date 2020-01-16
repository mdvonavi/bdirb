TEMPLATE = app
QT = gui \
    core \
    sql
CONFIG += qt \
    release \
    warn_on
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/mainwindow.ui \
    ui/settings.ui \
    ui/mainlists.ui \
    ui/pupilsform.ui \
    ui/measform.ui \
    ui/addmeas.ui \
    ui/search.ui
HEADERS = src/mainwindowimpl.h \
    src/settingsimpl.h \
    src/mainlistsimpl.h \
    src/pupilsform.h \
    src/measform.h \
    src/addmeas.h \
    src/search.h
SOURCES = src/mainwindowimpl.cpp \
    src/main.cpp \
    src/settingsimpl.cpp \
    src/mainlistsimpl.cpp \
    src/pupilsform.cpp \
    src/measform.cpp \
    src/addmeas.cpp \
    src/search.cpp
TRANSLATIONS += ts/ru_RU.ts
RESOURCES += qrc/images.qrc
LIBS += sqldrivers/qsqlite4.dll \
        sqldrivers/libqsqlite4.a
RC_FILE = bdirb.rc
