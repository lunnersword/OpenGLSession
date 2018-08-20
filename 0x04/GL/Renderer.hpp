//
//  Renderer.hpp
//  0x00
//
//  Created by ZhangXiaoJun on 2018/6/26.
//  Copyright © 2018年 meitu. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "GLContext.hpp"
#include "GLProgram.hpp"
#include "Texture.hpp"
#include <math.h>

extern const char kVertexShader[];
extern const char kFragmentShader[];

extern const char kDebugVertexShader[];
extern const char kDebugFragmentShader[];

extern const char kCircleVertexShader[];
extern const char kCircleFragmentShader[];

extern const char kTextureRendererVertexShader[];
extern const char kTextureRendererFragmentShader[];
extern const char kTextureRendererRepeatFragmentShader[];
extern const char kTextureRendererMirroredRepeatFragmentShader[];
extern const char kEffectRendererMosaicFragmentShader[];
extern const char kEffectRendererSaturationFragmentShader[];

struct _Size {
    float width;
    float height;

    _Size(float w = 0, float h = 0){
        width = w;
        height = h;
    }
};

class Renderer {
protected:
    const GLfloat *convertTextureSizeToViewPosition(const _Size &textureSize,
                                                    const _Size &viewSize) const {
        static GLfloat position[8];
        
        GLfloat viewAspectRatio = viewSize.width / viewSize.height;
        GLfloat textureAspectRatio = textureSize.width / textureSize.height;
        
        GLfloat widthScaling = 1;
        GLfloat heightScaling = 1;
        if (viewAspectRatio < textureAspectRatio) {
            GLfloat height = (viewSize.width / textureSize.width) * textureSize.height;
            heightScaling = height / viewSize.height;
        }else{
            GLfloat width = (viewSize.height / textureSize.height) * textureSize.width;
            widthScaling = width / viewSize.width;
        }
        
        position[0] = -widthScaling;
        position[1] = -heightScaling;
        position[2] = widthScaling;
        position[3] = -heightScaling;
        position[4] = -widthScaling;
        position[5] = heightScaling;
        position[6] = widthScaling;
        position[7] = heightScaling;
        return position;
    }
public:
    virtual void render(const _Size &size) = 0;
    virtual ~Renderer(){};
};

class ClearRenderer : public Renderer{
    float mR = 0,mG = 0,mB = 0,mA = 0;
public:
    ClearRenderer(float r = 1, float g = 0, float b = 0, float a = 1){mR = r; mG = g; mB = b; mA = a;};
    void render(const _Size &size){
        glClearColor(mR, mG, mB, mA);
        glClear(GL_COLOR_BUFFER_BIT);
    }
};

class DebugRenderer : public Renderer {
    MTCB::GLProgram *mProgram = NULL;
    GLuint mVertexAttribute = 0;
    GLuint mColorAttribute = 0;

    const float mImmutableLines[24] = {0, 0,
                                       0, 0,
                                       0, 0.85f,
                                       0, 0.85f,
                                       0, 0,
                                       0.85f, 0,
                                       0, 0,
                                       0.85f, 0,
                                       0.5f, 0.5f,
                                       0.5f, 0.5f,
                                       0.5f, 0.5f,
                                       0.5f, 0.5f};
    
    const float mLineColors[36] = {1, 0, 0,1, 0, 0,1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    float mLines[24] = {};
    
    float mArrows[12] = {};
public:
    float lineWidth = 0;
    
    ~DebugRenderer(){
        if (mProgram) {
            delete mProgram;
            mProgram = nullptr;
        }
    }
    
    DebugRenderer(float nLineWidth = 0.06){
        lineWidth = nLineWidth;
        mProgram = new MTCB::GLProgram(kDebugVertexShader, kDebugFragmentShader);
        mProgram->addAttribute("vertex");
        mProgram->addAttribute("color");
        
        if (!mProgram->link()) {
            std::string progLog = mProgram->programLog();
            printf("Program link log: %s", progLog.c_str());
            std::string fragLog = mProgram->fragmentShaderLog();
            printf("Fragment shader compile log: %s", fragLog.c_str());
            std::string vertLog = mProgram->vertexShaderLog();
            printf("Vertex shader compile log: %s", vertLog.c_str());
            mProgram = NULL;


            assert(false);
        }
        mVertexAttribute = mProgram->attributeIndex("vertex");
        mColorAttribute = mProgram->attributeIndex("color");
    }
    
    void render(const _Size &size){
        mProgram->use();
        glViewport(0, 0, size.width, size.height);
        
        auto halfLineWidth = lineWidth / 2;
        auto aspectRatio = size.width / size.height;
        
        memcpy(mLines, mImmutableLines, sizeof(mImmutableLines));
        
        for (int i = 0; i < 4; i++) {
            auto &firstLineX = mLines[i * 2];
            const auto &immutableFirstLineX = mImmutableLines[i * 2];
            firstLineX = immutableFirstLineX + (i % 2 == 0 ? -halfLineWidth : halfLineWidth);
            
            if (size.height > size.width) {
                if (i > 1){
                    auto &firstLineY = mLines[i * 2 + 1];
                    const auto &immutableFirstLineY = mImmutableLines[i * 2 + 1];
                    auto shortLineRealPixelY = immutableFirstLineY * size.width;
                    auto actualFirstLineY = (size.height - (size.width - shortLineRealPixelY)) / size.height;
                    firstLineY = actualFirstLineY;
                }
            }else{
                if (i  % 2 == 1) {
                    auto &secondLineX = mLines[i * 2 + 8];
                    const auto &immutableSecondLineX = mImmutableLines[i * 2 + 8];
                    auto shortLineRealPixelX = immutableSecondLineX * size.height;
                    auto actualSecondLineX = (size.width - (size.height - shortLineRealPixelX)) / size.width;
                    secondLineX = actualSecondLineX;
                }
            }


            auto &secondLineY = mLines[i * 2 + 8 + 1];
            const auto &immutableSecondLineY = mImmutableLines[i * 2 + 8 + 1];
            secondLineY = immutableSecondLineY + (i > 1 ? -halfLineWidth : halfLineWidth) * aspectRatio;
            
            // cube
            auto &x1 = mLines[i * 2 + 16];
            const auto &immutableX1 = mImmutableLines[i * 2 + 16];
            x1 = immutableX1 + (i % 2 == 0 ? -halfLineWidth : halfLineWidth);
            
            auto &y1 = mLines[i * 2 + 16 + 1];
            const auto &immutableY1 = mImmutableLines[i * 2 + 16 + 1];
            y1 = immutableY1 + (i < 2 ? -halfLineWidth : halfLineWidth) * aspectRatio;
        }

        static const auto kArrowOffset = 0.03f;
        
        auto firstLineEndY = mLines[5];
        mArrows[0] = 0;
        mArrows[1] = firstLineEndY;
        mArrows[2] = mLines[2] + kArrowOffset;
        mArrows[3] = firstLineEndY;
        mArrows[4] = mArrows[0];
        mArrows[5] = 1.0f;
        
        auto firstLineEndX = mLines[10];
        mArrows[6] = firstLineEndX;
        mArrows[7] = 0;
        mArrows[8] = 1;
        mArrows[9] = 0;
        mArrows[10] = mArrows[6];
        mArrows[11] = mLines[9] + kArrowOffset * aspectRatio;


        for (int i = 0; i < 12; ++i) {
            mLines[i * 2] = mLines[i * 2] * 2 - 1;
            mLines[i * 2 + 1] = mLines[i * 2 + 1] * 2 - 1;
            
            mArrows[i] = mArrows[i] * 2 - 1;
        }


        glEnableVertexAttribArray(mVertexAttribute);
        glEnableVertexAttribArray(mColorAttribute);
        
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, mLines);
        glVertexAttribPointer(mColorAttribute, 3, GL_FLOAT, GL_FALSE, 0, mLineColors);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, mArrows);
        glVertexAttribPointer(mColorAttribute, 3, GL_FLOAT, GL_FALSE, 0, mLineColors);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
        
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, &mLines[8]);
        glVertexAttribPointer(mColorAttribute, 3, GL_FLOAT, GL_FALSE, 0, &mLineColors[12]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, &mArrows[6]);
        glVertexAttribPointer(mColorAttribute, 3, GL_FLOAT, GL_FALSE, 0, &mLineColors[12]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
        
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, &mLines[16]);
        glVertexAttribPointer(mColorAttribute, 3, GL_FLOAT, GL_FALSE, 0, &mLineColors[24]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        
        glDisableVertexAttribArray(mColorAttribute);
        glDisableVertexAttribArray(mVertexAttribute);
    }
};



#if !defined(NDEBUG) && !defined(RELEASE)
#define IS_DEBUG 1
#else
#define IS_DEBUG 0
#endif

#define ENABLE_DEBUG_RENDERER IS_DEBUG && 0

class TriangleRenderer : public Renderer {
    MTCB::GLProgram *mProgram = NULL;
    const float mVertex[8] = {-0.5,-0.5, 0.5,-0.5, -0.5,0.5,0.5,0.5};
    const float mColor[12] = {1,0,0,0,1,0,0,0,1,1,1,1};
    GLuint mVertexAttribute = 0;
    GLuint mColorAttribute = 0;
    ClearRenderer mClear{1,1,0,1};

#if ENABLE_DEBUG_RENDERER
    DebugRenderer mDebugRender;
#endif

public:
    ~TriangleRenderer(){
        if (mProgram){
            delete mProgram;
            mProgram = nullptr;
        }
    }
    
    TriangleRenderer(){
        mProgram = new MTCB::GLProgram(kVertexShader, kFragmentShader);
        mProgram->addAttribute("vertex");
        mProgram->addAttribute("color");
        
        if (!mProgram->link()) {
            assert(false);
        }
        mVertexAttribute = mProgram->attributeIndex("vertex");
        mColorAttribute = mProgram->attributeIndex("color");
    }
    
    void render(const _Size &size){
        mProgram->use();
        glViewport(0, 0, size.width, size.height);
        
        mClear.render(size);
        glEnableVertexAttribArray(mVertexAttribute);
        glEnableVertexAttribArray(mColorAttribute);
        
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, mVertex);
        glVertexAttribPointer(mColorAttribute, 3, GL_FLOAT, GL_FALSE, 0, mColor);
        
        static const GLubyte sss[] = {0,1,2,0,2,3};
//
        glDrawElements(GL_TRIANGLE_STRIP, sizeof(sss) / sizeof(GLubyte), GL_UNSIGNED_BYTE, sss);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(mVertexAttribute);
        
#if ENABLE_DEBUG_RENDERER
        mDebugRender.render(size);
#endif
    }
};

class CircleRenderer : public Renderer {
    Texture *mTexture = NULL;
    
    MTCB::GLProgram *mProgram = NULL;
    float mVertex[8] = {-1,-0.5, 1,-0.5, -1,0.5,1,0.5};
    const float mTexcoord[8] = {0,0,1,0,0,1,1,1};
    GLuint mVertexAttribute = 0;
    GLuint mTexcoordAttribute = 0;
    GLuint mTextureUniform = 0;
    GLuint mAAAUniform = 0;
    ClearRenderer mClear{1,1,0,1};
public:
    ~CircleRenderer(){
        delete mTexture;
        delete mProgram;
    }
    
    CircleRenderer(){
        mProgram = new MTCB::GLProgram(kCircleVertexShader, kCircleFragmentShader);
        mProgram->addAttribute("vertex");
        mProgram->addAttribute("texcoord");
        
        if (!mProgram->link()) {
            assert(false);
        }
        mVertexAttribute = mProgram->attributeIndex("vertex");
        mTexcoordAttribute = mProgram->attributeIndex("texcoord");
        mTextureUniform = mProgram->uniformIndex("texture");
        mAAAUniform = mProgram->uniformIndex("aaa");
        
        const int diameter = 500;
        const int radius = diameter / 2;
        
        unsigned char *circleData = new unsigned char[diameter * diameter];
        
        for (int y = 0; y < diameter; ++y) {
            for (int x = 0; x < diameter; ++x) {
                int x1 = x - radius;
                int y1 = y - radius;
                bool inCircle = sqrt(x1 * x1 + y1 * y1) < radius;
                circleData[x + diameter * y] = inCircle ? 255 : 0;
            }
        }
        
        mTexture = new Texture((const void *)circleData, diameter, diameter, GL_ALPHA, GL_ALPHA);
        
        

        delete [] circleData;
    };
    
    void render(const _Size &size){
        mProgram->use();
        glViewport(0, 0, size.width, size.height);
        
//        float bottomLeftX = -(500 / size.width) / 2;
//        float bottomLeftY = -(500 / size.width) / 2;
        
        
        mClear.render(size);
        glEnableVertexAttribArray(mVertexAttribute);
        glEnableVertexAttribArray(mTexcoordAttribute);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture->texture());
        glUniform1i(mTextureUniform, 0);
        
//        glUniform1f(mAAAUniform, 0.0);
        glUniform3f(mAAAUniform, 1.0, 0.0, 1.0);
        
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, mVertex);
        glVertexAttribPointer(mTexcoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, mTexcoord);
        
//        static const GLubyte sss[] = {0,1,2,1,2,3};
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//        glDrawElements(GL_TRIANGLE_STRIP, sizeof(sss) / sizeof(GLubyte), GL_UNSIGNED_BYTE, sss);
        glDisableVertexAttribArray(mVertexAttribute);
    }
};

class TextureRenderer : public Renderer {
protected:
    Texture *mTexture = NULL;
    MTCB::GLProgram *mProgram = NULL;
    GLuint mVertexAttribute = 0;
    GLuint mTexcoordAttribute = 0;
    GLuint mTextureUniform = 0;
    ClearRenderer mClear{1,1,0,1};
    const float mTexcoord[8] = {0,0,1.0,0,0,1.0,1.0,1.0};
public:

    TextureRenderer(Texture *texture, const std::string &fragmentShader = kTextureRendererMirroredRepeatFragmentShader){
        mTexture = texture;

        mProgram = new MTCB::GLProgram(kTextureRendererVertexShader, fragmentShader);
        mProgram->addAttribute("vertex");
        mProgram->addAttribute("texcoord");

        if (!mProgram->link()) {
            assert(false);
        }
        mVertexAttribute = mProgram->attributeIndex("vertex");
        mTexcoordAttribute = mProgram->attributeIndex("texcoord");
        mTextureUniform = mProgram->uniformIndex("texture");
    }

    TextureRenderer(const std::string &imagePath) : TextureRenderer(new Texture(imagePath)){}
    
    void render(const _Size &size){
        mProgram->use();
        glViewport(0, 0, size.width, size.height);

        mClear.render(size);
        glEnableVertexAttribArray(mVertexAttribute);
        glEnableVertexAttribArray(mTexcoordAttribute);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture->texture());
        glUniform1i(mTextureUniform, 0);
        
        float rotateTexcoord[8];
        rotateTexcoord[0] = mTexcoord[4];
        rotateTexcoord[1] = mTexcoord[5];
        rotateTexcoord[2] = mTexcoord[6];
        rotateTexcoord[3] = mTexcoord[7];
        rotateTexcoord[4] = mTexcoord[0];
        rotateTexcoord[5] = mTexcoord[1];
        rotateTexcoord[6] = mTexcoord[2];
        rotateTexcoord[7] = mTexcoord[3];
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, convertTextureSizeToViewPosition(_Size(mTexture->width(),mTexture->height()), size));
        glVertexAttribPointer(mTexcoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateTexcoord);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(mVertexAttribute);
        glDisableVertexAttribArray(mTexcoordAttribute);
    }
};

class EffectRenderer : public TextureRenderer {
    float mValue = 0.f;
    GLuint mProgressUniform = 0;
public:
    
    void onValueChange(float value){
        mValue = value;
    }
    
    EffectRenderer(Texture *texture, const std::string &fragmentShader = kEffectRendererMosaicFragmentShader) : TextureRenderer(texture, fragmentShader){
        mProgressUniform = mProgram->uniformIndex("progress");
    }
    
    EffectRenderer(const std::string &imagePath) : EffectRenderer(new Texture(imagePath)){}
    
    void render(const _Size &size){
        mProgram->use();
        glViewport(0, 0, size.width, size.height);
        
        mClear.render(size);
        glEnableVertexAttribArray(mVertexAttribute);
        glEnableVertexAttribArray(mTexcoordAttribute);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture->texture());
        glUniform1i(mTextureUniform, 0);
        glUniform1f(mProgressUniform, mValue);
        
        float rotateTexcoord[8];
        rotateTexcoord[0] = mTexcoord[4];
        rotateTexcoord[1] = mTexcoord[5];
        rotateTexcoord[2] = mTexcoord[6];
        rotateTexcoord[3] = mTexcoord[7];
        rotateTexcoord[4] = mTexcoord[0];
        rotateTexcoord[5] = mTexcoord[1];
        rotateTexcoord[6] = mTexcoord[2];
        rotateTexcoord[7] = mTexcoord[3];
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, convertTextureSizeToViewPosition(_Size(mTexture->width(), mTexture->height()), size));
        glVertexAttribPointer(mTexcoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateTexcoord);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(mVertexAttribute);
        glDisableVertexAttribArray(mTexcoordAttribute);
    }
};


#endif /* Renderer_hpp */
