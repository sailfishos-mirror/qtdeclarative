# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from pressandhold.pro.

if (NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_pressandhold LANGUAGES C CXX ASM)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

#####################################################################
## tst_pressandhold Test:
#####################################################################

qt_internal_add_test(tst_pressandhold
    SOURCES
        tst_pressandhold.cpp
    LIBRARIES
        Qt::Gui
        Qt::Quick
        Qt::QuickControls2
)

#### Keys ignored in scope 1:.:.:pressandhold.pro:<TRUE>:
# TEMPLATE = "app"

## Scopes:
#####################################################################
