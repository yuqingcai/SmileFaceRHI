#include "smileface.h"
#include <QFile>
#include <QCursor>
#include <QRandomGenerator>
#include <QtMath>

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
}

static float vertexData[] = {
    // positions                // colors
    -100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    -100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,

    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    -100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,

    -100.0f,  100.0f,  100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,   0.0f, 0.0f, 1.0f,

    100.0f,  100.0f,  100.0f,    1.0f, 1.0f, 0.0f,
    100.0f,  100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,    1.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,    1.0f, 1.0f, 0.0f,

    -100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,

    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,







    -100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    -100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,

    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    -100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,

    -100.0f,  100.0f,  100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,   0.0f, 0.0f, 1.0f,

    100.0f,  100.0f,  100.0f,    1.0f, 1.0f, 0.0f,
    100.0f,  100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,    1.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,    1.0f, 1.0f, 0.0f,

    -100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,

    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,







    -100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 0.0f,
    -100.0f, -100.0f, -100.0f,   1.0f, 0.0f, 0.0f,

    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    -100.0f,  100.0f,  100.0f,   0.0f, 1.0f, 0.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 0.0f,

    -100.0f,  100.0f,  100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,   0.0f, 0.0f, 1.0f,

    100.0f,  100.0f,  100.0f,    1.0f, 1.0f, 0.0f,
    100.0f,  100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f, -100.0f,    1.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,    1.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,    1.0f, 1.0f, 0.0f,

    -100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f,  100.0f,   0.0f, 1.0f, 1.0f,
    -100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 1.0f,

    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    -100.0f,  100.0f,  100.0f,   1.0f, 0.0f, 1.0f,
    -100.0f,  100.0f, -100.0f,   1.0f, 0.0f, 1.0f,



    -100.0f, -100.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    100.0f, -100.0f, 0.0f,   1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, 0.0f,   1.0f, 0.0f, 0.0f,

};

SmileFaceRenderer::SmileFaceRenderer()
{
    m_models = new glm::mat4[m_instances];
    for (int i = 0; i < m_instances; i ++) {
        m_models[i] = glm::identity<glm::mat4>();
    }
}

SmileFaceRenderer::~SmileFaceRenderer()
{
    if (m_models) {
        delete m_models;
    }
}

void SmileFaceRenderer::initialize(QRhiCommandBuffer *cb)
{
    if (m_rhi != rhi()) {
        m_rhi = rhi();
        m_pipeline.reset();

        qDebug("pipline reset");
        qDebug("Backend: %s",                       m_rhi->backendName());
        qDebug("TextureSizeMin: %d",                m_rhi->resourceLimit(QRhi::TextureSizeMin));
        qDebug("TextureSizeMax: %d",                m_rhi->resourceLimit(QRhi::TextureSizeMax));
        qDebug("MaxColorAttachments: %d",           m_rhi->resourceLimit(QRhi::MaxColorAttachments));
        qDebug("FramesInFlight: %d",                m_rhi->resourceLimit(QRhi::FramesInFlight));
        qDebug("MaxAsyncReadbackFrames: %d",        m_rhi->resourceLimit(QRhi::MaxAsyncReadbackFrames));
        qDebug("MaxThreadGroupsPerDimension: %d",   m_rhi->resourceLimit(QRhi::MaxThreadGroupsPerDimension));
        qDebug("MaxThreadsPerThreadGroup: %d",      m_rhi->resourceLimit(QRhi::MaxThreadsPerThreadGroup));
        qDebug("MaxThreadGroupX: %d",               m_rhi->resourceLimit(QRhi::MaxThreadGroupX));
        qDebug("MaxThreadGroupY: %d",               m_rhi->resourceLimit(QRhi::MaxThreadGroupY));
        qDebug("MaxThreadGroupZ: %d",               m_rhi->resourceLimit(QRhi::MaxThreadGroupZ));
        qDebug("TextureArraySizeMax: %d",           m_rhi->resourceLimit(QRhi::TextureArraySizeMax));
        qDebug("MaxUniformBufferRange: %d",         m_rhi->resourceLimit(QRhi::MaxUniformBufferRange));
        qDebug("MaxVertexInputs: %d",               m_rhi->resourceLimit(QRhi::MaxVertexInputs));
        qDebug("MaxVertexOutputs: %d",              m_rhi->resourceLimit(QRhi::MaxVertexOutputs));
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

        m_vectexBuffer.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                              QRhiBuffer::VertexBuffer,
                                              sizeof(vertexData)));
        m_vectexBuffer->create();


        m_modelBuffer.reset(m_rhi->newBuffer(QRhiBuffer::Immutable,
                                             QRhiBuffer::VertexBuffer,
                                             m_instances*sizeof(glm::mat4)));
        m_modelBuffer->create();


        // uniformbuffer1 的每个block包含两个矩阵，view matrix 和 projection matrix
        // 每个block必须根据硬件进行对齐。“对齐”是为了在绘图的时候可以通过字节偏移量动态的把
        // 缓冲区里的某个block值映射到 Shader 里的 uniform block，而偏移量必须是“对齐字节
        // 数”的整数倍。
        int blockSize = 64*2;
        int bufferSize = 0;
        m_uniformBufferBlockSize = m_rhi->ubufAligned(blockSize);
        bufferSize = m_uniformBufferBlockSize * m_uniformBufferBlockCount;
        m_uniformBuffer.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                               QRhiBuffer::UniformBuffer,
                                               bufferSize));
        m_uniformBuffer->create();



        m_srb.reset(m_rhi->newShaderResourceBindings());

        // uniform 缓冲区使用 uniformBufferWithDynamicOffset 函数声明绑定
        m_srb->setBindings({
            QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
                0,
                QRhiShaderResourceBinding::VertexStage,
                m_uniformBuffer.get(),
                bufferSize),
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
                                 // PerVertex 表示该顶点属性的索引是以“顶点”进行递增的
                                 { 6 * sizeof(float), QRhiVertexInputBinding::PerVertex },
                                 // PerInstance 表示该顶点属性的索引是以“实例”进行递增的
                                 { sizeof(glm::mat4), QRhiVertexInputBinding::PerInstance },
                                 });

        inputLayout.setAttributes({
                                   { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
                                   { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) },
                                   // binding1
                                   // model矩阵由4个 vec4构成，每个vec4代表一列，从location2开始，每个列对应一个location。
                                   { 1, 2, QRhiVertexInputAttribute::Float4, 0 },
                                   { 1, 3, QRhiVertexInputAttribute::Float4, 4 * sizeof(float) },
                                   { 1, 4, QRhiVertexInputAttribute::Float4, 8 * sizeof(float) },
                                   { 1, 5, QRhiVertexInputAttribute::Float4, 12 * sizeof(float) },
                                   });
        m_pipeline->setSampleCount(m_sampleCount);
        m_pipeline->setVertexInputLayout(inputLayout);
        m_pipeline->setShaderResourceBindings(m_srb.get());
        m_pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
        m_pipeline->setDepthTest(true);
        m_pipeline->setDepthWrite(true);
        m_pipeline->create();

        QRhiResourceUpdateBatch *resourceUpdates = m_rhi->nextResourceUpdateBatch();
        resourceUpdates->uploadStaticBuffer(m_vectexBuffer.get(), vertexData);
        resourceUpdates->uploadStaticBuffer(m_modelBuffer.get(), m_models);


        cb->resourceUpdate(resourceUpdates);

    }

}

void SmileFaceRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    m_projection = m_rhi->clipSpaceCorrMatrix();

    m_projection.perspective(45.0f,
                             outputSize.width() / (float) outputSize.height(),
                             10.0f,
                             1000000.0f);

    // 所谓投影是指场景里被显示在Viewport里的内容，投影区域有“大小”和“中心点”这两个属性，
    // 从人的视觉角度上说，这两个属性决定了看到什么物体以及物体的大小。
    // 使用正交投影，left + right 和 bottom + top决定投影的大小 ，相应的，left和right
    // 决定了投影的水平中心, bottom 和 top 则决定了投影的垂直中心。
    //
    // m_projection.ortho(-outputSize.width()/2.0 + m_orthoX,
    //                    outputSize.width()/2.0 + m_orthoX,
    //                    -outputSize.height()/2.0 + m_orthoY,
    //                    outputSize.height()/2.0 + m_orthoY,
    //                    -200.0f, 10000.0f);

    // 使用透视投影，相机的z轴位置决定了场景投影范围的“大小”，相机的目标则决定了投影“中心点”。
    QVector3D cameraPos(0.0f, 200.0f, 800.0f + m_zoom);
    QVector3D cameraTarget(m_focus.rx(), m_focus.ry(), 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    m_view.setToIdentity();
    m_view.lookAt(cameraPos, cameraTarget, cameraUp);

    cb->beginPass(renderTarget(), Qt::white, { 1.0f, 0 });

    cb->setGraphicsPipeline(m_pipeline.get());
    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));

    const QRhiCommandBuffer::VertexInput vbufBinding(m_vectexBuffer.get(), 0);
    cb->setVertexInput(0, 1, &vbufBinding);

    const QRhiCommandBuffer::VertexInput modelInput(m_modelBuffer.get(), 0);
    cb->setVertexInput(1, 1, &modelInput);

    // 批量更新 uniform 缓冲区
    QRhiResourceUpdateBatch *batch = m_rhi->nextResourceUpdateBatch();
    batch->updateDynamicBuffer(m_uniformBuffer.get(),
                               0,
                               64,
                               m_view.constData());
    batch->updateDynamicBuffer(m_uniformBuffer.get(),
                               64,
                               64,
                               m_projection.constData());
    for (int i = 0; i < m_instances; i ++) {
        QMatrix4x4 model;
        model.setToIdentity();
        model.scale(1.0);
        model.translate(i* 400, 0, 0);
        model.rotate(m_angle, 0, 1, 0);
        batch->updateDynamicBuffer(m_modelBuffer.get(),
                                   i * 64,
                                   64,
                                   model.constData());
        // m_models[i] = glm::translate(glm::mat4(1.0f),
        //                              glm::vec3(i* 400, 0, 0));
        // m_models[i] = glm::rotate(m_models[i],
        //                           qDegreesToRadians(m_angle),
        //                           glm::vec3(1.0f, 1.0f, 0.0f));
        // batch->updateDynamicBuffer(m_modelBuffer.get(),
        //                                      i * sizeof(glm::mat4),
        //                                      sizeof(glm::mat4),
        //                                      &m_models[i]);
    }

    // 更新
    cb->resourceUpdate(batch);
    cb->setShaderResources(m_srb.get());

    // 绘制实例，由36个顶点构成，顶点属性数据从偏移量0开始，实例id为0。这里需要注意，实例
    // id是很重要的一个参数，它用于着色器索引 model 矩阵的数据。在本例中 model 矩阵数据
    // 是以顶点属性数组绑定到着色器，并且被声明为以PerInstance模式进行绑定，意思是每绘制
    // 完一个实例（而不是每绘制完一个顶点），model顶点属性的索引才会进行一次递增，这样就能
    // 做到每个绘制的一个实例对应 model 顶点属性数组里的一个 model 矩阵。
    cb->draw(36, 1, 0, 0);

    // 绘制其他实例，注意实例id的参数指定。
    cb->draw(36, 1, 36 * 1, 1);
    cb->draw(36, 1, 36 * 2, 2);
    cb->draw(3,  1, 36 * 3, 3);

    cb->endPass();
}

void SmileFaceRenderer::synchronize(QQuickRhiItem *rhiItem)
{
    SmileFace *item = static_cast<SmileFace *>(rhiItem);
    if (item->angle() != m_angle)
        m_angle = item->angle();
    if (item->backgroundAlpha() != m_alpha)
        m_alpha = item->backgroundAlpha();

    m_orthoX = item->getOrthoX();
    m_orthoY = item->getOrthoY();
    m_zoom = item->getZoom();
    m_focus = item->getFocus();
}

SmileFace::SmileFace()
{
    setFocusPolicy(Qt::ClickFocus);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
}

void SmileFace::hoverMoveEvent(QHoverEvent *event)
{
    if (m_spaceButtonDown) {

        int offsetX = (int)event->position().x() - m_mosePosition0.x() - this->width()/2;
        int offsetY = this->height()/2 - (int)event->position().y() - m_mosePosition0.y();

        m_focus.setX(offsetX);
        m_focus.setY(offsetY);

        // qDebug() << m_focus << m_mosePosition0 << event->position();
    }
    return QQuickRhiItem::hoverMoveEvent(event);
}

void SmileFace::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_leftButtonDown) {
            m_leftButtonDown = true;
            qDebug("m_leftButtonDown true");
        }
    }
    return QQuickRhiItem::mousePressEvent(event);
}


void SmileFace::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_leftButtonDown) {
            m_leftButtonDown = false;
            qDebug("m_leftButtonDown false");
        }
    }
    return QQuickRhiItem::mouseReleaseEvent(event);
}

void SmileFace::wheelEvent(QWheelEvent *event)
{
    // qDebug() << "Mouse wheel delta: " << event->angleDelta();
    if (event->angleDelta().y() > 0) {
        m_zoom += 200.0;
    }
    else if (event->angleDelta().y() < 0) {
        m_zoom -= 200.0;
    }
    return QQuickRhiItem::wheelEvent(event);
}


void SmileFace::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        m_orthoY -= 100.0;
    }
    else if (event->key() == Qt::Key_Down) {
        m_orthoY += 100.0;
    }
    else if (event->key() == Qt::Key_Left) {
        m_orthoX += 100.0;
    }
    else if (event->key() == Qt::Key_Right) {
        m_orthoX -= 100.0;
    }
    else if (event->key() == Qt::Key_Space) {
        if (!m_spaceButtonDown) {
            m_spaceButtonDown = true;
            qDebug("m_spaceButtonDown true");
        }
    }

    return QQuickRhiItem::keyPressEvent(event);
}

void SmileFace::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (m_spaceButtonDown) {
            m_spaceButtonDown = false;
            qDebug("m_spaceButtonDown false");
        }
    }

    return QQuickRhiItem::keyReleaseEvent(event);
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

float SmileFace::getOrthoX()
{
    return m_orthoX;
}


float SmileFace::getOrthoY()
{
    return m_orthoY;
}

float SmileFace::getZoom()
{
    return m_zoom;
}

QPointF& SmileFace::getFocus()
{
    return m_focus;
}
