// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick

Item {
    function testForNull() {
        for (;;)
        {}
        for (;;)
            x
        {} {} {} {}
    }
}
