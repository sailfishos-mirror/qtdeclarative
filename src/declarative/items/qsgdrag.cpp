/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qsgdrag_p.h"

#include <private/qsgitem_p.h>
#include <private/qsgevents_p_p.h>
#include <private/qsgitemchangelistener_p.h>
#include <private/qv8engine_p.h>

#include <QtGui/qevent.h>

QT_BEGIN_NAMESPACE

class QSGDragAttachedPrivate : public QObjectPrivate, public QSGItemChangeListener
{
    Q_DECLARE_PUBLIC(QSGDragAttached)
public:
    static QSGDragAttachedPrivate *get(QSGDragAttached *attached) {
        return static_cast<QSGDragAttachedPrivate *>(QObjectPrivate::get(attached)); }

    QSGDragAttachedPrivate()
        : attachedItem(0)
        , mimeData(0)
        , proposedAction(Qt::MoveAction)
        , supportedActions(Qt::MoveAction | Qt::CopyAction | Qt::LinkAction)
        , active(false)
        , listening(false)
    {
    }

    void itemGeometryChanged(QSGItem *, const QRectF &, const QRectF &);
    void start() { start(supportedActions); }
    void start(Qt::DropActions supportedActions);
    void setTarget(QSGItem *item);

    QSGDragGrabber dragGrabber;

    QDeclarativeGuard<QObject> source;
    QDeclarativeGuard<QObject> target;
    QSGItem *attachedItem;
    QSGDragMimeData *mimeData;
    Qt::DropAction proposedAction;
    Qt::DropActions supportedActions;
    bool active : 1;
    bool listening : 1;
    QPointF hotSpot;
    QStringList keys;
};

/*!
    \qmlclass Drag QSGDrag
    \inqmlmodule QtQuick 2
    \brief The Drag attached property provides drag and drop events for moved Items.

    Using the Drag attached property any Item can made a source of drag and drop
    events within a scene.

    When a drag is \l active on an item any change in that items position will
    generate a drag events that will be sent to any DropArea that intersects
    the with new  position of the item.  Other items which implement drag and
     drop event handlers can also receive these events.

    The following snippet shows how an item can be dragged with a MouseArea.
    However, dragging is not limited to mouse drags, anything that can move an item
    can generate drag events, this can include touch events, animations and bindings.

    \snippet doc/src/snippets/declarative/drag.qml 0

    A drag can be terminated either by cancelling it with Drag.cancel() or setting
    Drag.active to false, or it can be terminated with a drop event by calling
    Drag.drop().  If the drop event is accepted Drag.drop() will return the
    \l {supportedActions}{drop action} chosen by the recipient of the event,
    otherwise it will return Qt.IgnoreAction.

*/

void QSGDragAttachedPrivate::itemGeometryChanged(QSGItem *, const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_Q(QSGDragAttached);
    if (newGeometry.topLeft() == oldGeometry.topLeft() || !active)
        return;

    if (QSGCanvas *canvas = attachedItem->canvas()) {
        QPoint scenePos = attachedItem->mapToScene(hotSpot).toPoint();
        QDragMoveEvent event(scenePos, mimeData->m_supportedActions, mimeData, Qt::NoButton, Qt::NoModifier);
        QSGDropEventEx::setProposedAction(&event, proposedAction);
        QSGCanvasPrivate::get(canvas)->deliverDragEvent(&dragGrabber, &event);
        if (target != dragGrabber.target()) {
            target = dragGrabber.target();
            emit q->targetChanged();
        }
    }
}

QSGDragAttached::QSGDragAttached(QObject *parent)
    : QObject(*new QSGDragAttachedPrivate, parent)
{
    Q_D(QSGDragAttached);
    d->attachedItem = qobject_cast<QSGItem *>(parent);
    d->source = d->attachedItem;
}

QSGDragAttached::~QSGDragAttached()
{
    Q_D(QSGDragAttached);
    delete d->mimeData;
}

/*!
    \qmlattachedproperty bool QtQuick2::Drag::active

    This property holds whether a drag event sequence is currently active.

    Setting this property to true will send a QDragEnter event to the scene
    with the item's current position.  Setting it to false will send a
    QDragLeave event.

    While a drag is active any change in an item's position will send a QDragMove
    event with item's new position to the scene.
*/

bool QSGDragAttached::isActive() const
{
    Q_D(const QSGDragAttached);
    return d->active;
}

void QSGDragAttached::setActive(bool active)
{
    Q_D(QSGDragAttached);
    if (d->active != active) {
        if (active)
            d->start(d->supportedActions);
        else
            cancel();
    }
}

/*!
    \qmlattachedproperty Object QtQuick2::Drag::source

    This property holds an object that is identified to recipients of drag events as
    the source of the events.  By default this is the item Drag property is attached to.

    Changes to source while a Drag is active don't take effect until a new drag is started.
*/

QObject *QSGDragAttached::source() const
{
    Q_D(const QSGDragAttached);
    return d->source;
}

void QSGDragAttached::setSource(QObject *item)
{
    Q_D(QSGDragAttached);
    if (d->source != item) {
        d->source = item;
        emit sourceChanged();
    }
}

void QSGDragAttached::resetSource()
{
    Q_D(QSGDragAttached);
    if (d->source != d->attachedItem) {
        d->source = d->attachedItem;
        emit sourceChanged();
    }
}

/*!
    \qmlattachedproperty Object QtQuick2::Drag::target

    While a drag is active this property holds the last object to accept an
    enter event from the dragged item, if the current drag position doesn't
    intersect any accepting targets it is null.

    When a drag is not active this property holds the object that accepted
    the drop event that ended the drag, if no object accepted the drop or
    the drag was cancelled the target will then be null.
*/

QObject *QSGDragAttached::target() const
{
    Q_D(const QSGDragAttached);
    return d->target;
}

/*!
    \qmlattachedproperty QPointF QtQuick2::Drag::hotSpot

    This property holds the drag position relative to the top left of the item.

    By default this is (0, 0).

    Changes to hotSpot will take effect when the next event is sent.
*/

QPointF QSGDragAttached::hotSpot() const
{
    Q_D(const QSGDragAttached);
    return d->hotSpot;
}

void QSGDragAttached::setHotSpot(const QPointF &hotSpot)
{
    Q_D(QSGDragAttached);
    if (d->hotSpot != hotSpot) {
        d->hotSpot = hotSpot;
        emit hotSpotChanged();
        // Send a move event if active?
    }
}

/*!
    \qmlattachedproperty stringlist QtQuick2::Drag::keys

    This property holds a list of keys that can be used by a DropArea to filter drag events.

    Changes to keys while a Drag is active don't take effect until a new drag is started.
*/

QStringList QSGDragAttached::keys() const
{
    Q_D(const QSGDragAttached);
    return d->keys;
}

void QSGDragAttached::setKeys(const QStringList &keys)
{
    Q_D(QSGDragAttached);
    if (d->keys != keys) {
        d->keys = keys;
        emit keysChanged();
    }
}

/*!
    \qmlattachedproperty flags QtQuick2::Drag::supportedActions

    This property holds return values of Drag.drop() supported by the drag source.

    Changes to supportedActions while a Drag is active don't take effect
    until a new drag is started.
*/

Qt::DropActions QSGDragAttached::supportedActions() const
{
    Q_D(const QSGDragAttached);
    return d->supportedActions;
}

void QSGDragAttached::setSupportedActions(Qt::DropActions actions)
{
    Q_D(QSGDragAttached);
    if (d->supportedActions != actions) {
        d->supportedActions = actions;
        emit supportedActionsChanged();
    }
}

/*!
    \qmlattachedproperty enumeration QtQuick2::Drag::proposedAction

    This property holds an action that is recommended by the drag source as a
    return value from Drag.drop().

    Changes to proposedAction will take effect when the next event is sent.
*/

Qt::DropAction QSGDragAttached::proposedAction() const
{
    Q_D(const QSGDragAttached);
    return d->proposedAction;
}

void QSGDragAttached::setProposedAction(Qt::DropAction action)
{
    Q_D(QSGDragAttached);
    if (d->proposedAction != action) {
        d->proposedAction = action;
        emit proposedActionChanged();
        // send a move event with the new default action if active?
    }
}

void QSGDragAttachedPrivate::start(Qt::DropActions supportedActions)
{
    Q_Q(QSGDragAttached);
    Q_ASSERT(!active);

    if (QSGCanvas *canvas = attachedItem ? attachedItem->canvas() : 0) {
        if (!mimeData)
            mimeData = new QSGDragMimeData;
        if (!listening) {
            QSGItemPrivate::get(attachedItem)->addItemChangeListener(this, QSGItemPrivate::Geometry);
            listening = true;
        }

        mimeData->m_source = source;
        mimeData->m_supportedActions = supportedActions;
        mimeData->m_keys = keys;
        active = true;

        QPoint scenePos = attachedItem->mapToScene(hotSpot).toPoint();
        QDragEnterEvent event(scenePos, supportedActions, mimeData, Qt::NoButton, Qt::NoModifier);
        QSGDropEventEx::setProposedAction(&event, proposedAction);
        QSGCanvasPrivate::get(canvas)->deliverDragEvent(&dragGrabber, &event);

        emit q->activeChanged();
        if (target != dragGrabber.target()) {
            target = dragGrabber.target();
            emit q->targetChanged();
        }
    }
}

/*!
    \qmlattachedmethod void QtQuick2::Drag::start(flags supportedActions)

    Starts sending drag events.

    The optional \a supportedActions argument can be used to override the \l supportedActions
    property for the started sequence.
*/

void QSGDragAttached::start(QDeclarativeV8Function *args)
{
    Q_D(QSGDragAttached);
    if (d->active)
        cancel();

    Qt::DropActions supportedActions = d->supportedActions;
    // check arguments for supportedActions, maybe data?
    if (args->Length() >= 1) {
        v8::Local<v8::Value> v = (*args)[0];
        if (v->IsInt32())
            supportedActions = Qt::DropActions(v->Int32Value());
    }

    d->start(supportedActions);
}

/*!
    \qmlattachedmethod enum QtQuick2::Drag::drop()

    Ends a drag sequence by sending a drop event to the target item.

    Returns the action accepted by the target item.  If the target item or a parent doesn't accept
    the drop event then Qt.IgnoreAction will be returned.

    The returned drop action may be one of:

    \list
    \o Qt.CopyAction Copy the data to the target
    \o Qt.MoveAction Move the data from the source to the target
    \o Qt.LinkAction Create a link from the source to the target.
    \o Qt.IgnoreAction Ignore the action (do nothing with the data).
    \endlist

*/

int QSGDragAttached::drop()
{
    Q_D(QSGDragAttached);
    Qt::DropAction acceptedAction = Qt::IgnoreAction;

    if (!d->active)
        return acceptedAction;

    QObject *target = 0;

    if (QSGCanvas *canvas = d->attachedItem->canvas()) {
        QPoint scenePos = d->attachedItem->mapToScene(d->hotSpot).toPoint();

        QDropEvent event(
                scenePos, d->mimeData->m_supportedActions, d->mimeData, Qt::NoButton, Qt::NoModifier);
        QSGDropEventEx::setProposedAction(&event, d->proposedAction);
        QSGCanvasPrivate::get(canvas)->deliverDragEvent(&d->dragGrabber, &event);

        if (event.isAccepted()) {
            acceptedAction = event.dropAction();
            target = d->dragGrabber.target();
        }
    }

    d->active = false;
    if (d->target != target) {
        d->target = target;
        emit targetChanged();
    }

    emit activeChanged();
    return acceptedAction;
}

/*!
    \qmlattachedmethod void QtQuick2::Drag::cancel()

    Ends a drag sequence.
*/

void QSGDragAttached::cancel()
{
    Q_D(QSGDragAttached);
    if (!d->active)
        return;

    if (QSGCanvas *canvas = d->attachedItem->canvas()) {
        QDragLeaveEvent event;
        QSGCanvasPrivate::get(canvas)->deliverDragEvent(&d->dragGrabber, &event);
    }

    d->active = false;
    if (d->target) {
        d->target = 0;
        emit targetChanged();
    }
    emit activeChanged();
}

QT_END_NAMESPACE
