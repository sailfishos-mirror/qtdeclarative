// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick

Rectangle {
    id: root

    property var model
    property alias view: view

    ListView {
        id: view
        anchors.fill: parent
        objectName: "view"
        model: root.model
        delegate: Text {
            height: view.width
            text: modelData
        }
    }
}
