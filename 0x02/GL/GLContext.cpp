//
//  GLContext.cpp
//  GPUImage-X-iOS
//
//  Created by ZhangXiaoJun on 2017/11/26.
//

#include "GLContext.hpp"

using namespace MTCB;

GLContext::GLContext(GLRenderAPI renderAPI, const void *sharedObject){
    mRenderAPI = renderAPI;
    mSharedObject = sharedObject;
}

const void *GLContext::sharedObject() const{
    return mSharedObject;
}

GLRenderAPI GLContext::renderAPI() const {
    return mRenderAPI;
}


