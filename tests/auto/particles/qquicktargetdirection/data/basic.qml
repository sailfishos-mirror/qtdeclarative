// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 2.0
import QtQuick.Particles 2.0

Rectangle {
    color: "black"
    width: 320
    height: 320

    ParticleSystem {
        id: sys
        objectName: "system"
        anchors.fill: parent

        ImageParticle {
            source: "../../shared/star.png"
        }

        Emitter{
            //0,0 position
            velocity: TargetDirection{ targetItem: sys; proportionalMagnitude: true; magnitude: 1 }
            size: 32
            emitRate: 1000
            lifeSpan: 500
        }
    }
}
