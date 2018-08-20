//
//  GLContext_iOS.cpp
//  GPUImage-X-iOS
//
//  Created by ZhangXiaoJun on 2017/11/26.
//

#import "GLContext_iOS.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

using namespace MTCB;

GLContext_iOS::~GLContext_iOS(){
    
    if ([EAGLContext currentContext] == (__bridge id)mEAGLContext) {
        [EAGLContext setCurrentContext:nil];
    }
    
    CFRelease(mEAGLContext);
}

GLContext_iOS::GLContext_iOS(GLRenderAPI renderAPI, const void *sharedObject):GLContext(renderAPI, sharedObject)
{
    EAGLContext *context = nil;
    switch (renderAPI) {
        case GLRenderAPIES1:
            context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1 sharegroup:(__bridge EAGLSharegroup *)sharedObject];
            break;
        case GLRenderAPIES2:
            context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:(__bridge EAGLSharegroup *)sharedObject];
            break;
        case GLRenderAPIES3:
            context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3 sharegroup:(__bridge EAGLSharegroup *)sharedObject];
            
            if (!context) {
                mRenderAPI = GLRenderAPIES2;
                context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:(__bridge EAGLSharegroup *)sharedObject];
            }
            
            break;
        default:
            break;
    }

    assert(context != nil);
    mEAGLSharegroup = (__bridge_retained CFTypeRef)[context sharegroup];

    [EAGLContext setCurrentContext:context];
    mEAGLContext = (__bridge_retained CFTypeRef)context;
}

void GLContext_iOS::swapToScreen(){
    EAGLContext *context = (__bridge EAGLContext *)mEAGLContext;
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

void GLContext_iOS::use(){
    EAGLContext *context = (__bridge EAGLContext *)mEAGLContext;
    [EAGLContext setCurrentContext:context];
}

bool GLContext_iOS::isCurrentContext(){
    EAGLContext *context = (__bridge EAGLContext *)mEAGLContext;
    return [EAGLContext currentContext] == context;
}

#ifdef __cplusplus
extern "C"{
#endif
	bool currentContextIsExist(){
		return [EAGLContext currentContext] != NULL;
	}
#ifdef __cplusplus
}
#endif
