//
//  GLContext_Android.hpp
//  GPUImage-X-iOS
//
//  Created by ZhangXiaoJun on 2017/11/26.
//

#ifndef GLContext_Android_hpp
#define GLContext_Android_hpp

#include <stdio.h>
#include <string>
#include "../GLContext.hpp"

namespace MTCB {

    class GLContext_Android : public  GLContext {
    protected:
        EGLContext mContext = 0;
        EGLDisplay mDisplay = 0;
        EGLSurface mSurface = 0;
        EGLConfig  mConfig = 0;
        ANativeWindow *mWindow = 0;
        void checkEglError(std::string msg);
    public:
        GLContext_Android(GLRenderAPI renderAPI = GLRenderAPIES2, const void *sharedContext = NULL ,ANativeWindow *window = NULL);
        GLContext_Android(GLRenderAPI renderAPI = GLRenderAPIES2, const void *sharedContext = NULL , int width = 0, int height = 0);
        EGLContext eglContext() const;
        ~GLContext_Android();
        bool isCurrentContext();
        void use();
        void swapToScreen();
        void resetSurface(ANativeWindow *window);
    };
}

#endif /* GLContext_Android_hpp */
