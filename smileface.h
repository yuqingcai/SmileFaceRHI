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
};

class SmileFace: public QQuickRhiItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(SmileFace)

public:
    SmileFace();
    QQuickRhiItemRenderer *createRenderer() override;
};

#endif // SMILEFACE_H
