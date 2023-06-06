/**
 * Created by hachikuji on 2021/12/28.
 */

#ifndef OBJECT_MEASURE_CAMERARENDERER_H
#define OBJECT_MEASURE_CAMERARENDERER_H

#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickWindow>
#include <QMutex>
#include <QImage>

#include <stack>

/**
 * OpenGL 3.3 核心配置文件
 */

#include <QOpenGLFunctions_3_3_Core>

class CvMediaPlayerRenderer : public QQuickFramebufferObject::Renderer,
                              protected QOpenGLFunctions_3_3_Core {
public:
    CvMediaPlayerRenderer();

    ~CvMediaPlayerRenderer() override;

    void render() override;

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

    void setWindow(QQuickWindow *window) { this->window = window; }

    void setFrameCache(std::stack<QImage *> *cache) { frameCache = cache; }

private:

    void initializeOpenGl();

    void renderOpenGl();

    void updateTexture();


private:

    int pos{};
    int texCoord{};
    int texType{};

    QVector<QVector3D> posList;
    QVector<QVector2D> texCoordList;

    QOpenGLShaderProgram *shader{nullptr};
    QOpenGLTexture *texture{nullptr};
    QQuickWindow *window{nullptr};
    std::stack<QImage *> *frameCache{};

};


#endif
