// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qsgdefaultglyphnode_p.h"
#include "qsgdefaultglyphnode_p_p.h"

#include <private/qrawfont_p.h>

QT_BEGIN_NAMESPACE

QSGDefaultGlyphNode::QSGDefaultGlyphNode(QSGRenderContext *context)
    : m_context(context)
    , m_glyphNodeType(RootGlyphNode)
    , m_dirtyGeometry(false)
    , m_preferredAntialiasingMode(DefaultAntialiasing)
{
    setFlag(UsePreprocess);
}

QSGDefaultGlyphNode::~QSGDefaultGlyphNode()
{
    if (m_glyphNodeType == SubGlyphNode)
        return;

    qDeleteAll(m_nodesToDelete);
    m_nodesToDelete.clear();
}

void QSGDefaultGlyphNode::setPreferredAntialiasingMode(AntialiasingMode mode)
{
    m_preferredAntialiasingMode = mode;
}

void QSGDefaultGlyphNode::setMaterialColor(const QColor &color)
{
    static_cast<QSGTextMaskMaterial *>(m_material)->setColor(color);
}

void QSGDefaultGlyphNode::setGlyphs(const QPointF &position, const QGlyphRun &glyphs)
{
    QSGBasicGlyphNode::setGlyphs(position, glyphs);
    m_dirtyGeometry = true;
}

void QSGDefaultGlyphNode::update()
{
    QRawFont font = m_glyphs.rawFont();
    QMargins margins(0, 0, 0, 0);

    const auto *fontEngine = QRawFontPrivate::get(font)->fontEngine;
    const bool isColorFont = fontEngine->glyphFormat == QFontEngine::Format_ARGB;

    if (m_style == QQuickText::Normal || isColorFont) {
        QFontEngine::GlyphFormat glyphFormat;

        // Don't try to override glyph format of color fonts
        if (isColorFont) {
            glyphFormat = QFontEngine::Format_None;
        } else {
            switch (m_preferredAntialiasingMode) {
            case GrayAntialiasing:
                glyphFormat = QFontEngine::Format_A8;
                break;
            case HighQualitySubPixelAntialiasing:
            case LowQualitySubPixelAntialiasing:
                glyphFormat = QFontEngine::Format_A32;
                break;
            default:
                glyphFormat = QFontEngine::Format_None;
                break;
            }
        }

        const auto rgbColor = m_color.toRgb();
        m_material = new QSGTextMaskMaterial(m_context, QVector4D(rgbColor.redF(), rgbColor.greenF(), rgbColor.blueF(), rgbColor.alphaF()), font, glyphFormat);
    } else if (m_style == QQuickText::Outline) {
        QSGOutlinedTextMaterial *material = new QSGOutlinedTextMaterial(m_context, font);
        material->setStyleColor(m_styleColor);
        m_material = material;
        margins = QMargins(1, 1, 1, 1);
    } else {
        QSGStyledTextMaterial *material = new QSGStyledTextMaterial(m_context, font);
        if (m_style == QQuickText::Sunken) {
            material->setStyleShift(QVector2D(0, -1));
            margins.setTop(1);
        } else if (m_style == QQuickText::Raised) {
            material->setStyleShift(QVector2D(0, 1));
            margins.setBottom(1);
        }
        material->setStyleColor(m_styleColor);
        m_material = material;
    }

    QSGTextMaskMaterial *textMaskMaterial = static_cast<QSGTextMaskMaterial *>(m_material);
    textMaskMaterial->setColor(m_color);

    QRectF boundingRect;
    textMaskMaterial->populate(m_position, m_glyphs.glyphIndexes(), m_glyphs.positions(), geometry(),
                               &boundingRect, &m_baseLine, margins);
    setBoundingRect(boundingRect);

    setMaterial(m_material);
    markDirty(DirtyGeometry);
}

void QSGDefaultGlyphNode::preprocess()
{
    qDeleteAll(m_nodesToDelete);
    m_nodesToDelete.clear();

    if (m_dirtyGeometry)
        updateGeometry();
}

void QSGDefaultGlyphNode::updateGeometry()
{
    // Remove previously created sub glyph nodes
    // We assume all the children are sub glyph nodes
    QSGNode *subnode = firstChild();
    while (subnode) {
        // We can't delete the node now as it might be in the preprocess list
        // It will be deleted in the next preprocess
        m_nodesToDelete.append(subnode);
        subnode = subnode->nextSibling();
    }
    removeAllChildNodes();

    GlyphInfo glyphInfo;

    const QVector<quint32> indexes = m_glyphs.glyphIndexes();
    const QVector<QPointF> positions = m_glyphs.positions();

    const int maxGlyphs = (USHRT_MAX + 1) / 4; // 16384
    const int maxVertices = maxGlyphs * 4; // 65536
    const int maxIndexes = maxGlyphs * 6; // 98304

    for (int i = 0; i < indexes.size(); ++i) {
        const int glyphIndex = indexes.at(i);
        const QPointF position = positions.at(i);

        // As we use UNSIGNED_SHORT indexing in the geometry, we overload the
        // "glyphsInOtherNodes" concept as overflow for if there are more than
        // 65536 (16384 * 4) vertices to render which would otherwise exceed
        // the maximum index size. This will cause sub-nodes to be recursively
        // created to handle any number of glyphs.
        if (i >= maxGlyphs) {
            glyphInfo.indexes.append(glyphIndex);
            glyphInfo.positions.append(position);
            continue;
        }
    }

    if (!glyphInfo.indexes.isEmpty()) {
        QGlyphRun subNodeGlyphRun(m_glyphs);
        subNodeGlyphRun.setGlyphIndexes(glyphInfo.indexes);
        subNodeGlyphRun.setPositions(glyphInfo.positions);

        QSGDefaultGlyphNode *subNode = new QSGDefaultGlyphNode(m_context);
        subNode->setGlyphNodeType(SubGlyphNode);
        subNode->setColor(m_color);
        subNode->setStyle(m_style);
        subNode->setStyleColor(m_styleColor);
        subNode->setGlyphs(m_position, subNodeGlyphRun);
        subNode->update();
        subNode->updateGeometry(); // we have to explicitly call this now as preprocess won't be called before it's rendered
        appendChildNode(subNode);

        QSGGeometry *g = geometry();

        QSGGeometry::TexturedPoint2D *vertexData = g->vertexDataAsTexturedPoint2D();
        quint16 *indexData = g->indexDataAsUShort();

        QVector<QSGGeometry::TexturedPoint2D> tempVertexData(maxVertices);
        QVector<quint16> tempIndexData(maxIndexes);

        for (int i = 0; i < maxGlyphs; i++) {
            tempVertexData[i * 4 + 0] = vertexData[i * 4 + 0];
            tempVertexData[i * 4 + 1] = vertexData[i * 4 + 1];
            tempVertexData[i * 4 + 2] = vertexData[i * 4 + 2];
            tempVertexData[i * 4 + 3] = vertexData[i * 4 + 3];

            tempIndexData[i * 6 + 0] = indexData[i * 6 + 0];
            tempIndexData[i * 6 + 1] = indexData[i * 6 + 1];
            tempIndexData[i * 6 + 2] = indexData[i * 6 + 2];
            tempIndexData[i * 6 + 3] = indexData[i * 6 + 3];
            tempIndexData[i * 6 + 4] = indexData[i * 6 + 4];
            tempIndexData[i * 6 + 5] = indexData[i * 6 + 5];
        }

        g->allocate(maxVertices, maxIndexes);
        vertexData = g->vertexDataAsTexturedPoint2D();
        indexData = g->indexDataAsUShort();

        for (int i = 0; i < maxGlyphs; i++) {
            vertexData[i * 4 + 0] = tempVertexData[i * 4 + 0];
            vertexData[i * 4 + 1] = tempVertexData[i * 4 + 1];
            vertexData[i * 4 + 2] = tempVertexData[i * 4 + 2];
            vertexData[i * 4 + 3] = tempVertexData[i * 4 + 3];

            indexData[i * 6 + 0] = tempIndexData[i * 6 + 0];
            indexData[i * 6 + 1] = tempIndexData[i * 6 + 1];
            indexData[i * 6 + 2] = tempIndexData[i * 6 + 2];
            indexData[i * 6 + 3] = tempIndexData[i * 6 + 3];
            indexData[i * 6 + 4] = tempIndexData[i * 6 + 4];
            indexData[i * 6 + 5] = tempIndexData[i * 6 + 5];
        }
    }

    m_dirtyGeometry = false;
}

QT_END_NAMESPACE
