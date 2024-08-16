#include "smileface.h"
#include <QFile>

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
}

void SmileFaceRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        m_rhi = rhi();

        qDebug("%d %s", m_rhi->backend(), m_rhi->backendName());

        m_pipeline.reset();
    }

    if (m_sampleCount != renderTarget()->sampleCount()) {
        m_sampleCount = renderTarget()->sampleCount();
        m_pipeline.reset();
    }

    QRhiTexture *finalTex = m_sampleCount > 1 ? resolveTexture() : colorTexture();
    if (m_textureFormat != finalTex->format()) {
        m_textureFormat = finalTex->format();
        m_pipeline.reset();
    }

    // QShader shaderVert = getShader(QLatin1String(":/SmileFaceRHI/shaders/color.vert.qsb"));
    // QShader shaderFrag = getShader(QLatin1String(":/SmileFaceRHI/shaders/color.vert.qsb"));
}

void SmileFaceRenderer::synchronize(QQuickRhiItem *item)
{

}

void SmileFaceRenderer::render(QRhiCommandBuffer *cb)
{
}


SmileFace::SmileFace()
{

}


QQuickRhiItemRenderer* SmileFace::createRenderer()
{
    return new SmileFaceRenderer();
}
