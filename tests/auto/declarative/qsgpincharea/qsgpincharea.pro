CONFIG += testcase
TARGET = tst_qsgpincharea
macx:CONFIG -= app_bundle

SOURCES += tst_qsgpincharea.cpp

DEFINES += SRCDIR=\\\"$$PWD\\\"

CONFIG += parallel_test

QT += core-private gui-private declarative-private testlib
