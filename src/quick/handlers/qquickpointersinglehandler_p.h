/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QQUICKPOINTERSINGLEHANDLER_H
#define QQUICKPOINTERSINGLEHANDLER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qquickpointerdevicehandler_p.h"

QT_BEGIN_NAMESPACE

class Q_QUICK_PRIVATE_EXPORT QQuickPointerSingleHandler : public QQuickPointerDeviceHandler
{
    Q_OBJECT
    Q_PROPERTY(int pointId READ pointId NOTIFY pointIdChanged)
    Q_PROPERTY(QPointingDeviceUniqueId uniquePointId READ uniquePointId NOTIFY pointIdChanged)
    Q_PROPERTY(QPointF pos READ pos NOTIFY eventPointHandled)
    Q_PROPERTY(QPointF scenePos READ scenePos NOTIFY eventPointHandled)
    Q_PROPERTY(QPointF pressPos READ pressPos NOTIFY pressedButtonsChanged)
    Q_PROPERTY(QPointF scenePressPos READ scenePressPos NOTIFY pressedButtonsChanged)
    Q_PROPERTY(QPointF sceneGrabPos READ sceneGrabPos NOTIFY singlePointGrabChanged)
    Q_PROPERTY(Qt::MouseButtons pressedButtons READ pressedButtons NOTIFY pressedButtonsChanged)
    Q_PROPERTY(QVector2D velocity READ velocity NOTIFY eventPointHandled)
    Q_PROPERTY(qreal rotation READ rotation NOTIFY eventPointHandled)
    Q_PROPERTY(qreal pressure READ pressure NOTIFY eventPointHandled)
    Q_PROPERTY(QSizeF ellipseDiameters READ ellipseDiameters NOTIFY eventPointHandled)
    Q_PROPERTY(Qt::MouseButtons acceptedButtons READ acceptedButtons WRITE setAcceptedButtons NOTIFY acceptedButtonsChanged)
public:
    explicit QQuickPointerSingleHandler(QObject *parent = 0);
    virtual ~QQuickPointerSingleHandler() { }

    int pointId() const { return m_pointId; }
    Qt::MouseButtons pressedButtons() const { return m_pressedButtons; }
    Qt::MouseButtons acceptedButtons() const { return m_acceptedButtons; }
    void setAcceptedButtons(Qt::MouseButtons buttons);
    QPointF pressPos() const { return m_pressPos; }
    QPointF scenePressPos() const { return m_scenePressPos; }
    QPointF sceneGrabPos() const { return m_sceneGrabPos; }
    QPointF pos() const { return m_pos; }
    QPointF scenePos() const { return parentItem()->mapToScene(m_pos); }
    QVector2D velocity() const { return m_velocity; }
    qreal rotation() const { return m_rotation; }
    qreal pressure() const { return m_pressure; }
    QSizeF ellipseDiameters() const { return m_ellipseDiameters; }
    QPointingDeviceUniqueId uniquePointId() const { return m_uniquePointId; }

Q_SIGNALS:
    void pointIdChanged();
    void pressedButtonsChanged();
    void acceptedButtonsChanged();
    void singlePointGrabChanged(); // QQuickPointerHandler::grabChanged signal can't be a property notifier here
    void eventPointHandled();

protected:
    bool wantsPointerEvent(QQuickPointerEvent *event) override;
    virtual bool wantsEventPoint(QQuickEventPoint *point);
    void handlePointerEventImpl(QQuickPointerEvent *event) override;
    virtual void handleEventPoint(QQuickEventPoint *point) = 0;

    QQuickEventPoint *currentPoint(QQuickPointerEvent *ev) { return ev->pointById(m_pointId); }
    void onGrabChanged(QQuickPointerHandler *grabber, QQuickEventPoint::GrabState stateChange, QQuickEventPoint *point) override;

    void setIgnoreAdditionalPoints(bool v = true);

private:
    void setPressedButtons(Qt::MouseButtons buttons);
    void reset();

private:
    int m_pointId;
    QPointingDeviceUniqueId m_uniquePointId;
    Qt::MouseButtons m_pressedButtons;
    QPointF m_pos;
    QPointF m_pressPos;
    QPointF m_scenePressPos;
    QPointF m_sceneGrabPos;
    QVector2D m_velocity;
    qreal m_rotation;
    qreal m_pressure;
    QSizeF m_ellipseDiameters;
    Qt::MouseButtons m_acceptedButtons;
    bool m_ignoreAdditionalPoints : 1;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickPointerSingleHandler)

#endif // QQUICKPOINTERSINGLEHANDLER_H
