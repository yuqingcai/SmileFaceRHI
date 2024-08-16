#include "smileface.h"
#include <QFile>

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
}

static float vertexData[] = {
    0.0f,   0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  -0.5f,   0.0f, 1.0f, 0.0f,
    0.5f,  -0.5f,   0.0f, 0.0f, 1.0f,
};

void SmileFaceRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        m_rhi = rhi();
        m_pipeline.reset();

        qDebug("pipline reset");
        qDebug("Backend: %s", m_rhi->backendName());
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

    if (!m_pipeline) {
        m_pipeline.reset(m_rhi->newGraphicsPipeline());

        m_vbuf.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                      QRhiBuffer::VertexBuffer,
                                      sizeof(vertexData)));
        m_vbuf->create();

        m_ubuf.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                      QRhiBuffer::UniformBuffer,
                                      64));
        m_ubuf->create();


        m_srb.reset(m_rhi->newShaderResourceBindings());
        m_srb->setBindings({
            QRhiShaderResourceBinding::uniformBuffer(0,
                QRhiShaderResourceBinding::VertexStage,
                m_ubuf.get()),
        });
        m_srb->create();

        m_pipeline->setShaderStages({
            {
                QRhiShaderStage::Vertex,
                getShader(QLatin1String(":/SmileFaceRHI/shaders/color.vert.qsb"))
            },
            {
                QRhiShaderStage::Fragment,
                getShader(QLatin1String(":/SmileFaceRHI/shaders/color.frag.qsb"))
            }
        });
        QRhiVertexInputLayout inputLayout;
        inputLayout.setBindings({
            { 5 * sizeof(float) }
        });
        inputLayout.setAttributes({
            { 0, 0, QRhiVertexInputAttribute::Float2, 0 },
            { 0, 1, QRhiVertexInputAttribute::Float3, 2 * sizeof(float) }
        });
        m_pipeline->setSampleCount(m_sampleCount);
        m_pipeline->setVertexInputLayout(inputLayout);
        m_pipeline->setShaderResourceBindings(m_srb.get());
        m_pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
        m_pipeline->create();

        QRhiResourceUpdateBatch *resourceUpdates = m_rhi->nextResourceUpdateBatch();
        resourceUpdates->uploadStaticBuffer(m_vbuf.get(), vertexData);
        cb->resourceUpdate(resourceUpdates);

    }

    const QSize outputSize = renderTarget()->pixelSize();
    m_viewProjection = m_rhi->clipSpaceCorrMatrix();
    m_viewProjection.perspective(45.0f,
                                 outputSize.width() / (float) outputSize.height(),
                                 0.01f, 1000.0f);
    m_viewProjection.translate(0, 0, -4);

}

void SmileFaceRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    SmileFace *item = static_cast<SmileFace *>(rhiItem);
    if (item->angle() != m_angle)
        m_angle = item->angle();
    if (item->backgroundAlpha() != m_alpha)
        m_alpha = item->backgroundAlpha();
}

void SmileFaceRenderer::render(QRhiCommandBuffer *cb)
{
    QRhiResourceUpdateBatch *resourceUpdates = m_rhi->nextResourceUpdateBatch();
    QMatrix4x4 modelViewProjection = m_viewProjection;
    modelViewProjection.rotate(m_angle, 0, 1, 0);
    resourceUpdates->updateDynamicBuffer(m_ubuf.get(), 0, 64, modelViewProjection.constData());

    // Qt Quick expects premultiplied alpha
    const QColor clearColor = QColor::fromRgbF(0.5f * m_alpha, 0.5f * m_alpha, 0.7f * m_alpha, m_alpha);
    cb->beginPass(renderTarget(), clearColor, { 1.0f, 0 }, resourceUpdates);

    cb->setGraphicsPipeline(m_pipeline.get());
    const QSize outputSize = renderTarget()->pixelSize();
    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));
    cb->setShaderResources();
    const QRhiCommandBuffer::VertexInput vbufBinding(m_vbuf.get(), 0);
    cb->setVertexInput(0, 1, &vbufBinding);
    cb->draw(3);

    cb->endPass();
}


SmileFace::SmileFace()
{

}


QQuickRhiItemRenderer* SmileFace::createRenderer()
{
    return new SmileFaceRenderer();
}

void SmileFace::setAngle(float a)
{
    if (m_angle == a)
        return;

    m_angle = a;
    emit angleChanged();
    update();
}

void SmileFace::setBackgroundAlpha(float a)
{
    if (m_alpha == a)
        return;

    m_alpha = a;
    emit backgroundAlphaChanged();
    update();
}
