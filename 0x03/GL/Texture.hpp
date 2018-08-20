//
//  Texture.hpp
//  0x00
//
//  Created by ZhangXiaoJun on 2018/7/17.
//  Copyright © 2018年 meitu. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <string>
#include "gl.h"


class Texture {
    GLuint mTexture = 0;
    void init(const void *mem, int width, int height, GLuint format, GLuint internalFormat);
    int mWidth = 0, mHeight = 0;
public:
    Texture(const std::string &path);
    Texture(const void *mem, int width, int height, GLuint format, GLuint internalFormat);
    ~Texture();
    
    GLuint texture() const;
    int width() const;
    int height() const;
};

#endif /* Texture_hpp */
