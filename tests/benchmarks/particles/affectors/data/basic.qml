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
        running: false //Benchmark will manage it

        ImageParticle {
            source: "../../shared/star.png"
        }

        Emitter{
            //0,0 position
            id: emitter
            enabled: false
            size: 32
            emitRate: 1000
            lifeSpan: Emitter.InfiniteLife
            maximumEmitted: 1000
            Component.onCompleted: emitter.burst(1000);
        }

        Affector{
        }
    }
}
