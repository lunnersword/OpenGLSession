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

extern const char kVertexShader[];
extern const char kFragmentShader[];

extern const char kDebugVertexShader[];
extern const char kDebugFragmentShader[];

struct _Size {
    float width;
    float height;

    _Size(float w = 0, float h = 0){
        width = w;
        height = h;
    }
};

class Renderer {
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

#define ENABLE_DEBUG_RENDERER IS_DEBUG && 1

class TriangleRenderer : public Renderer {
    MTCB::GLProgram *mProgram = NULL;
    const float mVertex[6] = {-1,-1, 1,-1, 0,1};
    GLuint mVertexAttribute = 0;
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
        
        if (!mProgram->link()) {
            assert(false);
        }
        mVertexAttribute = mProgram->attributeIndex("vertex");
    }
    
    void render(const _Size &size){
        mProgram->use();
        glViewport(0, 0, size.width, size.height);
        
        mClear.render(size);
        glEnableVertexAttribArray(mVertexAttribute);
        glVertexAttribPointer(mVertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, mVertex);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
        glDisableVertexAttribArray(mVertexAttribute);
        
#if ENABLE_DEBUG_RENDERER
        mDebugRender.render(size);
#endif
    }
};



#endif /* Renderer_hpp */
