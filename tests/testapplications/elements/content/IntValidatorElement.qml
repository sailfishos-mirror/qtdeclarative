// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 2.0

Item {
    id: intvalidatorelementtest
    anchors.fill: parent
    property string testtext: ""

    IntValidator { id: intvalidatorelement; top: 30; bottom: -50 //top: 20; bottom: -50
        //Behavior on top { NumberAnimation { duration: 1000 } }
    }

    Rectangle {
        id: intvalidatorelementbackground
        color: intvalidatorelementinput.acceptableInput ? "green" : "red"
        height: 50; width: parent.width *.8; border.color: "gray"; opacity: 0.7; radius: 5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15

        TextInput {
            id: intvalidatorelementinput
            font.pointSize: 12; width: parent.width; text: "0"; horizontalAlignment: Text.AlignHCenter; validator: intvalidatorelement
            anchors.centerIn: parent
            Behavior on font.pointSize { NumberAnimation { duration: 1000 } }
            Behavior on color { ColorAnimation { duration: 1000 } }
        }
    }

    Text{
        anchors.top: intvalidatorelementbackground.bottom; anchors.topMargin: 50; anchors.horizontalCenter: parent.horizontalCenter
        text: "Top: " + intvalidatorelement.top + " Bottom: " + intvalidatorelement.bottom
    }

    SystemTestHelp { id: helpbubble; visible: statenum != 0
        anchors { top: parent.top; horizontalCenter: parent.horizontalCenter; topMargin: 50 }
    }
    BugPanel { id: bugpanel }

    states: [
        State { name: "start"; when: statenum == 1
            PropertyChanges { target: intvalidatorelementinput; text: "0" }
            PropertyChanges { target: intvalidatorelementtest
                testtext: "This is a TextInput element using an IntValidator for input masking. At present it should be indicating 0.\n"+
                "Next, let's attempt to enter a number smaller than the lowest allowable value: -80" }
        },
        State { name: "lowerthan"; when: statenum == 2
            PropertyChanges { target: intvalidatorelementinput; text: "-80" }
            PropertyChanges { target: intvalidatorelementtest
                testtext: "The TextInput background should be showing red - input is not acceptable.\n"+
                "Next, let's enter a value equal to the lowest allowable value: -50." }
        },
        State { name: "equaltobottom"; when: statenum == 3
            PropertyChanges { target: intvalidatorelementinput; text: "-50" }
            PropertyChanges { target: intvalidatorelementtest
                testtext: "The TextInput background should be showing green - input is acceptable.\n"+
                "Next, let's attempt to enter a number greater than the highest allowable value: 35" }
        },
        State { name: "greaterthan"; when: statenum == 4
            PropertyChanges { target: intvalidatorelementinput; text: "35" }
            PropertyChanges { target: intvalidatorelementtest
                testtext: "The TextInput background should be showing red - input is not acceptable.\n"+
                "Next, let's change the value of top to be greater than the entered number." }
        },
        State { name: "increasedtop"; when: statenum == 5
            PropertyChanges { target: intvalidatorelementinput; text: "35" }
            PropertyChanges { target: intvalidatorelement; top: 35 }
            PropertyChanges { target: intvalidatorelementtest
                testtext: "The highest value should have increased to 35, thus making the number valid and turning the input background green.\n"+
                "Next, let's change the value of bottom to be greater than the entered number." }
            PropertyChanges { target: bugpanel; bugnumber: "19956" }
        },
        State { name: "increasedbottom"; when: statenum == 6
            PropertyChanges { target: intvalidatorelementinput; text: "35" }
            PropertyChanges { target: intvalidatorelement; top: 35; bottom: 36 }
            PropertyChanges { target: intvalidatorelementtest
                testtext: "The lowest value should have increased to 36, thus making the validator invalid and turning the input background red.\n"+
                "Press advance to restart the test." }
        }
    ]

}
