//
//  Texture.cpp
//  0x00
//
//  Created by ZhangXiaoJun on 2018/7/17.
//  Copyright © 2018年 meitu. All rights reserved.
//

#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#ifdef MTCB_PLATFORM_ANDROID
#include <jni.h>
#endif

Texture::~Texture(){
    if (mTexture) {
        glDeleteTextures(1, &mTexture);
    }
}

Texture::Texture(const std::string &path){
    
    int width = 0;
    int height = 0;
    int imageFormat = 0;
    void *imageData = stbi_load(path.c_str(),
                                &width,
                                &height,
                                &imageFormat,
                                STBI_default);
    
    int alignment = 1;
    GLuint internalFormat = GL_RGBA;
    switch (imageFormat) {
        case STBI_grey:
            internalFormat = GL_ALPHA;
            break;
        case STBI_grey_alpha:
            internalFormat = GL_RG;
            break;
        case STBI_rgb:
            internalFormat = GL_RGB;
            break;
        case STBI_rgb_alpha:
        default:
            alignment = 4;
            break;
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    init(imageData, width, height, internalFormat, internalFormat);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    STBI_FREE(imageData);
}

Texture::Texture(const void *mem, int width, int height, GLuint format, GLuint internalFormat){
    init(mem, width, height, format, internalFormat);
}

void Texture::init(const void *mem, int width, int height, GLuint format, GLuint internalFormat){
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, mem);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // This is necessary for non-power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    mWidth = width;
    mHeight = height;
}

GLuint Texture::texture() const{
    return mTexture;
}

int Texture::width() const{
    return mWidth;
}
int Texture::height() const{
    return mHeight;
}

#ifdef MTCB_PLATFORM_ANDROID

extern "C"{

jlong Java_com_meitu_android_Texture_createNativeTexture(JNIEnv *env, jobject thiz, jobject buffer, jint width, jint height, jint format, jint internalFormat){
    jbyte *b = (jbyte *)env->GetDirectBufferAddress(buffer);
    Texture *texture = new Texture(b, width, height, format, internalFormat);
    return reinterpret_cast<jlong>(texture);
}
};

#endif
