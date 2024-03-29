// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

// This JavaScript file is a single, small, imported script.
// It imports many other (non-nested) single, small, scripts.

.import "msbsi1.js" as Msbsi1
.import "msbsi2.js" as Msbsi2
.import "msbsi3.js" as Msbsi3
.import "msbsi4.js" as Msbsi4
.import "msbsi5.js" as Msbsi5
.import "msbsi6.js" as Msbsi6
.import "msbsi7.js" as Msbsi7
.import "msbsi8.js" as Msbsi8
.import "msbsi9.js" as Msbsi9
.import "msbsi10.js" as Msbsi10
.import "msbsi11.js" as Msbsi11
.import "msbsi12.js" as Msbsi12
.import "msbsi13.js" as Msbsi13
.import "msbsi14.js" as Msbsi14
.import "msbsi15.js" as Msbsi15

function testFunc(seedValue) {
    var retn = 10 * (seedValue * 0.45);
    retn += Msbsi1.testFunc(seedValue);
    retn += Msbsi2.testFunc(seedValue);
    retn += Msbsi3.testFunc(seedValue);
    retn += Msbsi4.testFunc(seedValue);
    retn += Msbsi5.testFunc(seedValue);
    retn += Msbsi6.testFunc(seedValue);
    retn += Msbsi7.testFunc(seedValue);
    retn += Msbsi8.testFunc(seedValue);
    retn += Msbsi9.testFunc(seedValue);
    retn += Msbsi10.testFunc(seedValue);
    retn += Msbsi11.testFunc(seedValue);
    retn += Msbsi12.testFunc(seedValue);
    retn += Msbsi13.testFunc(seedValue);
    retn += Msbsi14.testFunc(seedValue);
    retn += Msbsi15.testFunc(seedValue);
    return retn;
}
