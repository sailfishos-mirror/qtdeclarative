# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from toolsupport.pro.

#####################################################################
## tst_toolsupport Test:
#####################################################################

if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qml_toolsupport LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_test(tst_qml_toolsupport
    SOURCES
        tst_toolsupport.cpp
    LIBRARIES
        Qt::CorePrivate
        Qt::QmlPrivate
)
