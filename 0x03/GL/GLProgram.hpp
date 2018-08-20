//
//  GLProgram.hpp
//  GPUImage-X-iOS
//
//  Created by ZhangXiaoJun on 2017/11/26.
//

#ifndef GLProgram_hpp
#define GLProgram_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "gl.h"

namespace MTCB {
    class GLProgram {
    protected:
        GLuint mProgram, mVertShader, mFragShader;
        std::string mProgramLog, mVertexShaderLog,mFragmentShaderLog;
        bool mInitialized = false;
        std::vector<const std::string> mAttributes;
        bool compileShader(GLuint *shader,
                           GLenum type,
                           const std::string &shaderString);
    public:
        GLProgram(const std::string &vertexShaderString, const std::string &fragmentShaderString);
        GLProgram(const GLProgram& program) = delete;
        
        const std::string &vertexShaderLog(){return mVertexShaderLog;};
        const std::string &fragmentShaderLog(){return mFragmentShaderLog;};
        const std::string &programLog(){return mProgramLog;};
        void addAttribute(const std::string &attributeName);
        GLuint attributeIndex(const std::string &attributeName);
        GLuint uniformIndex(const std::string &uniformName);
        bool link();
        void validate();
        bool initialized(){return mInitialized;};
        void use() const;
        ~GLProgram();
    };
}

#endif /* GLProgram_hpp */
