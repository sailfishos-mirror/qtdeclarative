CONFIG += testcase
TARGET = tst_qdeclarativeanimations
SOURCES += tst_qdeclarativeanimations.cpp
macx:CONFIG -= app_bundle

DEFINES += SRCDIR=\\\"$$PWD\\\"

CONFIG += parallel_test

QT += core-private gui-private v8-private declarative-private opengl-private testlib
