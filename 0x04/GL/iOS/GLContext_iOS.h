//
//  GLContext_iOS.hpp
//  GPUImage-X-iOS
//
//  Created by ZhangXiaoJun on 2017/11/26.
//

#ifndef GLContext_iOS_hpp
#define GLContext_iOS_hpp

#include <stdio.h>
#include "GLContext.hpp"
#import <CoreFoundation/CoreFoundation.h>
#import <CoreVideo/CoreVideo.h>
namespace MTCB {
    class GLContext_iOS : public GLContext {
        CFTypeRef mEAGLContext = NULL;
		CFTypeRef mEAGLSharegroup = NULL;
    public:
        ~GLContext_iOS();
        CFTypeRef EAGLCtx() const {
            return mEAGLContext;
        }
		
		CFTypeRef EAGLShareObj() const{
			return mEAGLSharegroup;
		}
		
        GLContext_iOS(GLRenderAPI renderAPI = GLRenderAPIES2, const void *sharedObject = NULL);
        GLContext_iOS(const GLContext_iOS &context) = delete;
        bool isCurrentContext();
        void use();
        void swapToScreen();
    };
}

#endif /* GLContext_iOS_hpp */
