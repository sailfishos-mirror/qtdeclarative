CONFIG += testcase
TARGET = tst_qdeclarativeengine
macx:CONFIG -= app_bundle

SOURCES += tst_qdeclarativeengine.cpp 

DEFINES += SRCDIR=\\\"$$PWD\\\"

CONFIG += parallel_test

QT += core-private gui-private declarative-private network testlib
