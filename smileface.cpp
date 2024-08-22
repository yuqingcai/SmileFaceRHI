#include "smileface.h"
#include <QFile>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static QShader getShader(const QString &name)
{
    QFile f(name);
    return f.open(QIODevice::ReadOnly) ? QShader::fromSerialized(f.readAll()) : QShader();
}

static float vertexData[] = {
    // positions         // colors
   -100.0f, -100.0f, -100.0f,  1.0f, 0.0f, 0.0f,
    100.0f, -100.0f, -100.0f,  1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,  1.0f, 0.0f, 0.0f,
    100.0f,  100.0f, -100.0f,  1.0f, 0.0f, 0.0f,
   -100.0f,  100.0f, -100.0f,  1.0f, 0.0f, 0.0f,
   -100.0f, -100.0f, -100.0f,  1.0f, 0.0f, 0.0f,

   -100.0f, -100.0f,  100.0f,  0.0f, 1.0f, 0.0f,
    100.0f, -100.0f,  100.0f,  0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,  0.0f, 1.0f, 0.0f,
    100.0f,  100.0f,  100.0f,  0.0f, 1.0f, 0.0f,
   -100.0f,  100.0f,  100.0f,  0.0f, 1.0f, 0.0f,
   -100.0f, -100.0f,  100.0f,  0.0f, 1.0f, 0.0f,

   -100.0f,  100.0f,  100.0f,  0.0f, 0.0f, 1.0f,
   -100.0f,  100.0f, -100.0f,  0.0f, 0.0f, 1.0f,
   -100.0f, -100.0f, -100.0f,  0.0f, 0.0f, 1.0f,
   -100.0f, -100.0f, -100.0f,  0.0f, 0.0f, 1.0f,
   -100.0f, -100.0f,  100.0f,  0.0f, 0.0f, 1.0f,
   -100.0f,  100.0f,  100.0f,  0.0f, 0.0f, 1.0f,

   100.0f,  100.0f,  100.0f,   1.0f, 1.0f, 0.0f,
   100.0f,  100.0f, -100.0f,   1.0f, 1.0f, 0.0f,
   100.0f, -100.0f, -100.0f,   1.0f, 1.0f, 0.0f,
   100.0f, -100.0f, -100.0f,   1.0f, 1.0f, 0.0f,
   100.0f, -100.0f,  100.0f,   1.0f, 1.0f, 0.0f,
   100.0f,  100.0f,  100.0f,   1.0f, 1.0f, 0.0f,

   -100.0f, -100.0f, -100.0f,  0.0f, 1.0f, 1.0f,
    100.0f, -100.0f, -100.0f,  0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,  0.0f, 1.0f, 1.0f,
    100.0f, -100.0f,  100.0f,  0.0f, 1.0f, 1.0f,
   -100.0f, -100.0f,  100.0f,  0.0f, 1.0f, 1.0f,
   -100.0f, -100.0f, -100.0f,  0.0f, 1.0f, 1.0f,

   -100.0f,  100.0f, -100.0f,  1.0f, 0.0f, 1.0f,
    100.0f,  100.0f, -100.0f,  1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,  1.0f, 0.0f, 1.0f,
    100.0f,  100.0f,  100.0f,  1.0f, 0.0f, 1.0f,
   -100.0f,  100.0f,  100.0f,  1.0f, 0.0f, 1.0f,
   -100.0f,  100.0f, -100.0f,  1.0f, 0.0f, 1.0f,


   -110.0f, -110.0f, -50.0f,  1.0f, 0.0f, 0.0f,
    110.0f, -110.0f, -50.0f,  1.0f, 0.0f, 0.0f,
    110.0f,  110.0f, -50.0f,  1.0f, 0.0f, 0.0f,
    110.0f,  110.0f, -50.0f,  1.0f, 0.0f, 0.0f,
   -110.0f,  110.0f, -50.0f,  1.0f, 0.0f, 0.0f,
   -110.0f, -110.0f, -50.0f,  1.0f, 0.0f, 0.0f,

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

        // ONE_UBUF_SIZE 必须根据实际硬件定义的方式对齐
        const int UB_SIZE = 64;
        ONE_UBUF_SIZE = m_rhi->ubufAligned(UB_SIZE);
        m_ubuf.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                      QRhiBuffer::UniformBuffer,
                                      ONE_UBUF_SIZE*2));
        m_ubuf->create();

        m_ubuf2.reset(m_rhi->newBuffer(QRhiBuffer::Dynamic,
                                       QRhiBuffer::UniformBuffer,
                                       ONE_UBUF_SIZE * N));
        m_ubuf2->create();

        m_srb.reset(m_rhi->newShaderResourceBindings());

        // uniform 缓冲区使用 uniformBufferWithDynamicOffset 函数声明绑定
        m_srb->setBindings({
            QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
                0,
                QRhiShaderResourceBinding::VertexStage,
                m_ubuf.get(),
                ONE_UBUF_SIZE),
            QRhiShaderResourceBinding::uniformBufferWithDynamicOffset(
                1,
                QRhiShaderResourceBinding::VertexStage,
                m_ubuf2.get(),
                ONE_UBUF_SIZE),

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
            { 6 * sizeof(float) }
        });
        inputLayout.setAttributes({
            { 0, 0, QRhiVertexInputAttribute::Float3, 0 },
            { 0, 1, QRhiVertexInputAttribute::Float3, 3 * sizeof(float) }
        });
        m_pipeline->setSampleCount(m_sampleCount);
        m_pipeline->setVertexInputLayout(inputLayout);
        m_pipeline->setShaderResourceBindings(m_srb.get());
        m_pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());
        m_pipeline->setDepthTest(true);
        m_pipeline->setDepthWrite(true);
        m_pipeline->create();

        QRhiResourceUpdateBatch *resourceUpdates = m_rhi->nextResourceUpdateBatch();
        resourceUpdates->uploadStaticBuffer(m_vbuf.get(), vertexData);
        cb->resourceUpdate(resourceUpdates);

    }

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
}

void SmileFaceRenderer::render(QRhiCommandBuffer *cb)
{
    const QSize outputSize = renderTarget()->pixelSize();
    m_projection = m_rhi->clipSpaceCorrMatrix();

    m_projection.perspective(45.0f,
                             outputSize.width() / (float) outputSize.height(),
                             10.0f,
                             1000.0f);

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
    QVector3D cameraPos(0.0f, 0.0f, 800.0f);
    QVector3D cameraTarget(0.0f, 0.0f, 0.0f);
    QVector3D cameraUp(0.0f, 1.0f, 0.0f);
    m_view.setToIdentity();
    m_view.lookAt(cameraPos, cameraTarget, cameraUp);


    const QColor clearColor = QColor::fromRgbF(1.0f, 1.0f, 0.0, 1.0);

    cb->beginPass(renderTarget(), Qt::white, { 1.0f, 0 });

    cb->setGraphicsPipeline(m_pipeline.get());
    cb->setViewport(QRhiViewport(0, 0, outputSize.width(), outputSize.height()));
    const QRhiCommandBuffer::VertexInput vbufBinding(m_vbuf.get(), 0);
    cb->setVertexInput(0, 1, &vbufBinding);

    // 批量更新 uniform 缓冲区
    QRhiResourceUpdateBatch *resourceUpdates = m_rhi->nextResourceUpdateBatch();
    resourceUpdates->updateDynamicBuffer(m_ubuf.get(),
                                         0,
                                         64,
                                         m_view.constData());
    resourceUpdates->updateDynamicBuffer(m_ubuf.get(),
                                         64,
                                         64,
                                         m_projection.constData());

    // 第一个物件的 model 矩阵
    m_model.setToIdentity();
    m_model.rotate(m_angle, 1, 1, 1);
    resourceUpdates->updateDynamicBuffer(m_ubuf2.get(),
                                         ONE_UBUF_SIZE * 0,
                                         64,
                                         m_model.constData());
    // 第二个物件的 model 矩阵
    m_model.setToIdentity();
    m_model.rotate(20, 0, 1, 0);
    m_model.translate(-200, -200, 0);
    resourceUpdates->updateDynamicBuffer(m_ubuf2.get(),
                                         ONE_UBUF_SIZE * 1,
                                         64,
                                         m_model.constData());

    // 第三个物件的 model 矩阵
    m_model.setToIdentity();
    m_model.rotate(20, 0, 1, 0);
    m_model.translate(200, 200, 0);
    resourceUpdates->updateDynamicBuffer(m_ubuf2.get(),
                                         ONE_UBUF_SIZE * 2,
                                         64,
                                         m_model.constData());

    // 第四个物件的 model 矩阵
    m_model.setToIdentity();
    m_model.rotate(30, 0, 1, 0);
    m_model.translate(200, -200, 0);
    resourceUpdates->updateDynamicBuffer(m_ubuf2.get(),
                                         ONE_UBUF_SIZE * 3,
                                         64,
                                         m_model.constData());
    // 更新
    cb->resourceUpdate(resourceUpdates);
    for (int i = 0; i < N - 1; i ++) {
        // 对当前物件渲染指定 uniform 缓冲区偏移
        QRhiCommandBuffer::DynamicOffset dynOfs[] = { { 1, i * ONE_UBUF_SIZE } };
        cb->setShaderResources(m_srb.get(), 1, dynOfs);
        cb->draw(36);
    }

    QRhiCommandBuffer::DynamicOffset dynOfs[] = { { 1, 3 * ONE_UBUF_SIZE } };
    cb->setShaderResources(m_srb.get(), 1, dynOfs);
    cb->draw(6, 1, 36);


    cb->endPass();

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
    qDebug() << event->position();
    return QQuickRhiItem::hoverMoveEvent(event);
}

void SmileFace::mousePressEvent(QMouseEvent *event)
{
    // qDebug() << event->position();
    return QQuickRhiItem::mousePressEvent(event);
}

void SmileFace::wheelEvent(QWheelEvent *event)
{
    qDebug() << "Mouse wheel delta: "
             << event->angleDelta();
    if (event->angleDelta().y() > 0) {
        m_orthoY += 10.0;
    }
    else if (event->angleDelta().y() < 0) {
       m_orthoY -= 10.0;
    }
    return QQuickRhiItem::wheelEvent(event);
}


void SmileFace::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        m_orthoY -= 10.0;
    }
    else if (event->key() == Qt::Key_Down) {
        m_orthoY += 10.0;
    }
    else if (event->key() == Qt::Key_Left) {
        m_orthoX += 10.0;
    }
    else if (event->key() == Qt::Key_Right) {
        m_orthoX -= 10.0;
    }

    return QQuickRhiItem::keyPressEvent(event);
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
