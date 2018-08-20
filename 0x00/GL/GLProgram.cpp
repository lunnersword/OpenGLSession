//
//  GLProgram.cpp
//  GPUImage-X-iOS
//
//  Created by ZhangXiaoJun on 2017/11/26.
//

#include "GLProgram.hpp"
#include "GLContextCInterface.h"

using namespace MTCB;

GLProgram::GLProgram(const std::string &vertexShaderString, const std::string &fragmentShaderString)
{
    mProgram = glCreateProgram();
    
    if (!compileShader(&mVertShader,
                       GL_VERTEX_SHADER,
                       vertexShaderString)) {
        printf("Failed to compile vertex shader \n");
    }
    
    if (!compileShader(&mFragShader,
                       GL_FRAGMENT_SHADER,
                       fragmentShaderString)) {
        printf("Failed to compile fragment shader \n");
    }
    
    glAttachShader(mProgram, mVertShader);
    glAttachShader(mProgram, mFragShader);
}

bool GLProgram::compileShader(GLuint *shader, GLenum type, const std::string &shaderString)
{
    //    CFAbsoluteTime startTime = CFAbsoluteTimeGetCurrent();
    
    GLint status;
    const GLchar *source;
    
    source = shaderString.c_str();
    if (!source)
    {
        printf("Failed to load vertex shader \n");
        return false;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    
    if (status != GL_TRUE)
    {
        GLint logLength = 0;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = new GLchar[logLength];
            glGetShaderInfoLog(*shader, logLength, &logLength, log);
            char logBuffer[logLength];
            sprintf(logBuffer,"%s", log);
            if (shader == &mVertShader)
            {
                mVertexShaderLog = logBuffer;
            }
            else
            {
                mFragmentShaderLog = logBuffer;
            }
            
            delete [] log;
        }
    }
    
    //    CFAbsoluteTime linkTime = (CFAbsoluteTimeGetCurrent() - startTime);
    //    NSLog(@"Compiled in %f ms", linkTime * 1000.0);
    
    return status == GL_TRUE;
}

void GLProgram::addAttribute(const std::string &attributeName){
    auto findObj = std::find(mAttributes.begin(), mAttributes.end(), attributeName);
    if (findObj == mAttributes.end()) {
        auto index = (GLuint)(findObj - mAttributes.begin());
        mAttributes.push_back(attributeName);
        glBindAttribLocation(mProgram,
                             index,
                             attributeName.c_str());
    }
}

// END:addattribute
// START:indexmethods
GLuint GLProgram::attributeIndex(const std::string &attributeName){
    auto findObj = std::find(mAttributes.begin(), mAttributes.end(), attributeName);
    return (GLuint)(findObj - mAttributes.begin());
}

GLuint GLProgram::uniformIndex(const std::string &uniformName){
 	return glGetUniformLocation(mProgram, uniformName.c_str());
}
// END:indexmethods
#pragma mark -
// START:link
bool GLProgram::link()
{
    //    CFAbsoluteTime startTime = CFAbsoluteTimeGetCurrent();
    
    GLint status;
    
    glLinkProgram(mProgram);
    
    glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        return false;
    
    if (mVertShader)
    {
        glDeleteShader(mVertShader);
        (mVertShader) = 0;
    }
    if ((mFragShader))
    {
        glDeleteShader((mFragShader));
        (mFragShader) = 0;
    }
    
    mInitialized = true;
    
    //    CFAbsoluteTime linkTime = (CFAbsoluteTimeGetCurrent() - startTime);
    //    NSLog(@"Linked in %f ms", linkTime * 1000.0);
    
    return true;
}

// END:link
// START:use
void GLProgram::use() const
{
    glUseProgram(mProgram);
}

// END:use
#pragma mark -

void GLProgram::validate()
{
    GLint logLength;
    
    glValidateProgram(mProgram);
    glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log =  new GLchar[logLength];
        glGetProgramInfoLog(mProgram, logLength, &logLength, log);
        char logBuffer[logLength];
        sprintf(logBuffer,"%s", log);
        mProgramLog = logBuffer;
        delete [] log;
    }
}

GLProgram::~GLProgram(){
    if (mVertShader)
        glDeleteShader(mVertShader);
    
    if (mFragShader)
        glDeleteShader(mFragShader);
    
    if (mProgram)
        glDeleteProgram(mProgram);
}



