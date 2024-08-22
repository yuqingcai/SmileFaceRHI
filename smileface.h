#ifndef SMILEFACE_H
#define SMILEFACE_H

#include <QQuickRhiItem>
#include <rhi/qrhi.h>

class SmileFaceRenderer : public QQuickRhiItemRenderer
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
    std::unique_ptr<QRhiBuffer> m_ubuf2;
    std::unique_ptr<QRhiShaderResourceBindings> m_srb;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    float m_angle = 0.0f;
    float m_alpha = 1.0f;
    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;
};

class SmileFace: public QQuickRhiItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(SmileFace)
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(float backgroundAlpha READ backgroundAlpha WRITE setBackgroundAlpha NOTIFY backgroundAlphaChanged)

public:
    SmileFace();
    QQuickRhiItemRenderer *createRenderer() override;

    float angle() const { return m_angle; }
    void setAngle(float a);

    float backgroundAlpha() const { return m_alpha; }
    void setBackgroundAlpha(float a);
    float getOrthoX();
    float getOrthoY();

protected:
    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


signals:
    void angleChanged();
    void backgroundAlphaChanged();

private:
    float m_angle = 0.0f;
    float m_alpha = 1.0f;
    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;
};

#endif // SMILEFACE_H
