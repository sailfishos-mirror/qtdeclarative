# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from revisions.pro.

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_revisions LANGUAGES C CXX ASM)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

#####################################################################
## tst_revisions Test:
#####################################################################

qt_internal_add_test(tst_revisions
    SOURCES
        tst_revisions.cpp
    LIBRARIES
        Qt::Gui
        Qt::Qml
        Qt::QuickControls2
)

## Scopes:
#####################################################################
