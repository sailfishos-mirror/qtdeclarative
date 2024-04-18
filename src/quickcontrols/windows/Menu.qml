// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Window
import QtQuick.Effects

T.Menu {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    leftPadding: 5
    rightPadding: 5
    topPadding: 5
    bottomPadding: 5
    margins: 0
    overlap: 4

    delegate: MenuItem { }

    contentItem: ListView {
        implicitHeight: contentHeight
        model: control.contentModel
        interactive: Window.window
                     ? contentHeight + control.topPadding + control.bottomPadding > control.height
                     : false
        currentIndex: control.currentIndex
        spacing: 2

        ScrollIndicator.vertical: ScrollIndicator {}
    }

    background: MultiEffect {
        implicitWidth: 200
        implicitHeight: 20
        source: Rectangle {
            width: control.background.width
            height: control.background.height
            radius: 8
            color: Qt.lighter(control.palette.window, 1.15)
            border.color: Qt.darker(control.palette.window, 1.12)
            visible: false
        }
        shadowScale: 1.04
        shadowOpacity: 0.1
        shadowColor: 'black'
        shadowEnabled: true
        shadowHorizontalOffset: 0
        shadowVerticalOffset: 10
    }

    T.Overlay.modal: Rectangle {
        color: "transparent"
    }

    T.Overlay.modeless: Rectangle {
        color: "transparent"
    }
}