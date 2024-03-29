// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 2.0

Item {
    id: systempaletteelementtest
    anchors.fill: parent
    property string testtext: ""

    SystemPalette { id: syspal; colorGroup: SystemPalette.Inactive }
    Rectangle {
        height: parent.height *.8; width: parent.width *.8; border.width: 6; radius: 4
        anchors.centerIn: parent
        color: syspal.base; border.color: syspal.window
        Rectangle {
            height: 20; width: parent.width; border.color: "black"; color: syspal.window; radius: 4
            Text { text: "File"; color: syspal.windowText; font.pointSize: 9
                anchors { left: parent.left; leftMargin: 5; verticalCenter: parent.verticalCenter }
            }
            Rectangle {
                id: shadow
                height: button.height; width: button.width; color: syspal.shadow; radius: 5; opacity: .5
                anchors { left: button.left; top: button.top; leftMargin: 2; topMargin: 2 }
            }
            Rectangle {
                id: button
                width: 150; height: 50; radius: 5; border.color: "black"; color: clicky.pressed ? syspal.highlight : syspal.button
                Behavior on color { ColorAnimation { duration: 500 } }
                anchors { left: parent.left; top: parent.top; leftMargin: 10; topMargin: 30 }
                Text { anchors.centerIn: parent; text: "Button"; color: syspal.buttonText }
                MouseArea { id: clicky; anchors.fill: parent
                    onPressed: { shadow.anchors.topMargin = 1; shadow.anchors.leftMargin = 1 }
                    onReleased: { shadow.anchors.topMargin = 2; shadow.anchors.leftMargin = 2 }
                }
            }
        }
    }

    SystemTestHelp { id: helpbubble; visible: statenum != 0
        anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter; bottomMargin: 50 }
    }
    BugPanel { id: bugpanel }

    states: [
        State { name: "start"; when: statenum == 1
            PropertyChanges { target: systempaletteelementtest
                testtext: "This is an mock application shaded with the help of the SystemPalette element.\n"+
                "The colors of the menu bar, menu text and button should mimic that of the OS it is running on.\n"+
                "Pressing the labelled button should shade it to the system highlight color." }
        }
    ]

}
