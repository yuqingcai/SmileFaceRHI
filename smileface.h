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
    std::unique_ptr<QRhiBuffer> m_vectexBuffer;
    std::unique_ptr<QRhiBuffer> m_uniformBuffer1;
    int m_uniformBuffer1BlockSize = 0;
    int m_uniformBuffer1BlockCount = 1;

    std::unique_ptr<QRhiBuffer> m_uniformBuffer2;
    int m_uniformBuffer2BlockSize = 0;
    static const int m_uniformBuffer2BlockCount = 256;
    QMatrix4x4 m_model2s[m_uniformBuffer2BlockCount];


    std::unique_ptr<QRhiBuffer> m_uniformBuffer3;
    int m_uniformBuffer3BlockSize = 0;
    static const int m_uniformBuffer3BlockCount = 256;
    QMatrix4x4 m_model3s[m_uniformBuffer2BlockCount];

    std::unique_ptr<QRhiShaderResourceBindings> m_srb;

    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    float m_angle = 0.0f;
    float m_alpha = 1.0f;
    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;
    float m_zoom = 1.0f;
    QPointF m_focus = {0.0f, 0.0f};

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
    float getZoom();
    QPointF& getFocus();

protected:
    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;


signals:
    void angleChanged();
    void backgroundAlphaChanged();

private:
    float m_angle = 0.0f;
    float m_alpha = 1.0f;
    float m_orthoX = 0.0f;
    float m_orthoY = 0.0f;
    float m_zoom = 1.0f;
    QPointF m_focus = {0.0f, 0.0f};
    bool m_spaceButtonDown = false;
    bool m_leftButtonDown = false;

    QPoint m_mosePosition0 = {0, 0};
    QPoint m_mosePosition1 = {0, 0};
};

#endif // SMILEFACE_H
