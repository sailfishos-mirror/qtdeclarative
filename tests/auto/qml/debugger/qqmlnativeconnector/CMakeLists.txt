# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from qqmlnativeconnector.pro.

#####################################################################
## tst_qqmlnativeconnector Test:
#####################################################################

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qqmlnativeconnector LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_test(tst_qqmlnativeconnector
    SOURCES
        tst_qqmlnativeconnector.cpp
    DEFINES
        QT_QML_DEBUG
    LIBRARIES
        Qt::CorePrivate
        Qt::Gui
        Qt::GuiPrivate
        Qt::Qml
)

## Scopes:
#####################################################################
