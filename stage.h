#ifndef STAGE_H
#define STAGE_H

#include <QQmlEngine>
#include <QQuickRhiItem>
#include <rhi/qrhi.h>


class StageRenderer : public QQuickRhiItemRenderer
{
public:
    void initialize(QRhiCommandBuffer *cb) override;
    void synchronize(QQuickRhiItem *item) override;
    void render(QRhiCommandBuffer *cb) override;

private:
    QRhi *m_rhi = nullptr;
    int m_sampleCount = 1;
    QRhiTexture::Format m_textureFormat = QRhiTexture::RGBA8;
    std::unique_ptr<QRhiGraphicsPipeline> m_pipeline;
    std::unique_ptr<QRhiBuffer> m_vbuf;
    std::unique_ptr<QRhiBuffer> m_ubuf;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;

    int m_uBufferSlotSize = 0;
};


class Stage : public QQuickRhiItem
{
    Q_OBJECT
    QML_ELEMENT

public:
    Stage();

    QQuickRhiItemRenderer *createRenderer() override;
    float getOrthoX();
    float getOrthoY();

protected:
    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


private:
    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;
    float m_zoom = 0.0f;
};

#endif // STAGE_H
