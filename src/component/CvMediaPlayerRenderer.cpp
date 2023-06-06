/**
 * Created by hachikuji on 2021/12/28.
 */

#include "CvMediaPlayerRenderer.h"

CvMediaPlayerRenderer::CvMediaPlayerRenderer() {

    initializeOpenGl();

}

CvMediaPlayerRenderer::~CvMediaPlayerRenderer() {

    delete shader;
    delete texture;
}

void CvMediaPlayerRenderer::render() {

    updateTexture();
    renderOpenGl();
}

QOpenGLFramebufferObject *CvMediaPlayerRenderer::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

/**
 *  initialize OpenGL Shader Texture
 */
void CvMediaPlayerRenderer::initializeOpenGl() {

    initializeOpenGLFunctions();

    /**
     * 设置着色器程序，绑定数值
     */

    shader = new QOpenGLShaderProgram();

    if (!shader->addCacheableShaderFromSourceFile(
            QOpenGLShader::Vertex, ":/assets/glsl/shader.vert")) {
        qDebug() << "Compiler vertex error";
        exit(0);
    }
    if (!shader->addCacheableShaderFromSourceFile(
            QOpenGLShader::Fragment, ":/assets/glsl/shader.frag")) {
        qDebug() << "Compiler fragment error";
        exit(0);
    }

    shader->link();

    float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
    };
    float texCoords[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
    };

    pos = shader->attributeLocation("aPos");
    texCoord = shader->attributeLocation("aTexCoord");
    texType = shader->uniformLocation("texType");


    for (int i = 0; i < sizeof(vertices) / sizeof(float); i += 3)
        posList.push_back(QVector3D(vertices[i], vertices[i + 1], vertices[i + 2]));


    for (int i = 0; i < sizeof(texCoords) / sizeof(float); i += 2)
        texCoordList.push_back(QVector2D(texCoords[i], texCoords[i + 1]));

    shader->enableAttributeArray(pos);
    shader->enableAttributeArray(texCoord);
    shader->setAttributeArray(pos, posList.constData());
    shader->setAttributeArray(texCoord, texCoordList.constData());
    shader->disableAttributeArray(pos);

}

void CvMediaPlayerRenderer::renderOpenGl() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    shader->bind();
    shader->setUniformValue(texType, 1);
    texture->bind(1);
    shader->enableAttributeArray(pos);
    shader->enableAttributeArray(texCoord);
    shader->setAttributeArray(pos, posList.constData());
    shader->setAttributeArray(texCoord, texCoordList.constData());
    glDrawArrays(GL_TRIANGLES, 0, posList.size());
    shader->disableAttributeArray(pos);
    texture->release();
    shader->release();

    window->resetOpenGLState();
}

/**
 *  convert cv::Mat to OpenGL texture
 */
void CvMediaPlayerRenderer::updateTexture() {

    if (texture != nullptr) {
        texture->destroy();
        delete texture;
    }

    if (frameCache->empty())
        return;


    texture = new QOpenGLTexture(*frameCache->top(),
                                 QOpenGLTexture::GenerateMipMaps);

    if (!texture->isCreated()) {
        qDebug() << "Failed to load texture";
        exit(0);
    }

    // set the texture wrapping parameters
    texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    // set texture filtering parameters
    texture->setMinificationFilter(QOpenGLTexture::Linear);

}


