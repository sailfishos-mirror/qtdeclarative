// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtQuick.Controls

MouseArea {
    width: column.width
    height: column.height
    cursorShape: Qt.ForbiddenCursor

    Column {
        id: column
        padding: 10
        spacing: 10

        Page {
            width: 200
            height: 20
        }
        Pane {
            width: 200
            height: 20
        }
        ToolBar {
            width: 200
            height: 20
        }
    }
}
