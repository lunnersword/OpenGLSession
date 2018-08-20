//
//  GLContext_Android.cpp
//  GPUImage-X-iOS
//
//  Created by ZhangXiaoJun on 2017/11/26.
//

#include "GLContext_Android.hpp"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>


MTCB::GLContext_Android::GLContext_Android(MTCB::GLRenderAPI renderAPI,
                                           const void *sharedObject,
                                           ANativeWindow *window):GLContext(renderAPI, sharedObject) {

    // EGL config attributes
    EGLint confAttr[15] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,    // very important!
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,          // we will create a pixelbuffer surface
            EGL_RED_SIZE,   8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE,  8,
            EGL_ALPHA_SIZE, 8,     // if you need the alpha channel
            EGL_DEPTH_SIZE, 16,    // if you need the depth buffer
            EGL_NONE
    };


    // EGL context attributes
    EGLint ctxAttr[3] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,              // very important!
            EGL_NONE
    };

    switch (renderAPI) {
        case GLRenderAPIES2 :
            confAttr[1] = EGL_OPENGL_ES2_BIT;
            ctxAttr[1] = 2;
            break;
        case GLRenderAPIES3:
            confAttr[1] = EGL_OPENGL_ES3_BIT_KHR;
            ctxAttr[1] = 3;
            break;
        default:
            break;
    }


    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    checkEglError("eglCreateWindowSurface");

    if (display == EGL_NO_DISPLAY) {
        throw "eglGetDisplay failed";
    }


    EGLint eglMajVers, eglMinVers;
    EGLBoolean initResult = eglInitialize(display, &eglMajVers, &eglMinVers);

    assert(initResult);

    EGLConfig config = NULL;
    EGLint numConfigs = 0;

    if (!eglChooseConfig(display, confAttr, &config, 1, &numConfigs) || numConfigs != 1){

        if (renderAPI == GLRenderAPIES3) {
            confAttr[1] = EGL_OPENGL_ES2_BIT;
            ctxAttr[1] = 2;
            mRenderAPI = GLRenderAPIES2;

            if (!eglChooseConfig(display, confAttr, &config, 1, &numConfigs) || numConfigs != 1) {
                assert(false);
            }
        } else{
            assert(false);
        }
    }


    // Create a window surface, and attach it to the Surface we received.
    int surfaceAttribs[] = {
            EGL_NONE
    };
    EGLSurface surface = eglCreateWindowSurface(display, config, window, surfaceAttribs);

    checkEglError("eglCreateWindowSurface");

    EGLContext sharedGLContext = (EGLContext)sharedObject;
    EGLContext context = eglCreateContext(display, config, sharedGLContext, ctxAttr);

    if (context == EGL_NO_CONTEXT) {
        assert(false);
    }

    mWindow = window;
    mSurface = surface;
    mContext = context;
    mDisplay = display;
    mConfig = config;
}

MTCB::GLContext_Android::GLContext_Android(MTCB::GLRenderAPI renderAPI,
                                           const void *sharedObject,
                                           int width,
                                           int height) :GLContext(renderAPI, sharedObject){
    // EGL config attributes
    EGLint confAttr[15] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,    // very important!
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,          // we will create a pixelbuffer surface
            EGL_RED_SIZE,   8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE,  8,
            EGL_ALPHA_SIZE, 8,     // if you need the alpha channel
            EGL_DEPTH_SIZE, 16,    // if you need the depth buffer
            EGL_NONE
    };


    // EGL context attributes
    EGLint ctxAttr[3] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,              // very important!
            EGL_NONE
    };

    switch (renderAPI) {
        case GLRenderAPIES2 :
            confAttr[1] = EGL_OPENGL_ES2_BIT;
            ctxAttr[1] = 2;
            break;
        case GLRenderAPIES3:
            confAttr[1] = EGL_OPENGL_ES3_BIT_KHR;
            ctxAttr[1] = 3;
            break;
        default:
            break;
    }


    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    checkEglError("eglCreateWindowSurface");

    if (display == EGL_NO_DISPLAY) {
        throw "eglGetDisplay failed";
    }


    EGLint eglMajVers, eglMinVers;
    EGLBoolean initResult = eglInitialize(display, &eglMajVers, &eglMinVers);

    assert(initResult);

    EGLConfig config = NULL;
    EGLint numConfigs = 0;

    if (!eglChooseConfig(display, confAttr, &config, 1, &numConfigs) || numConfigs != 1){

        if (renderAPI == GLRenderAPIES3) {
            confAttr[1] = EGL_OPENGL_ES2_BIT;
            ctxAttr[1] = 2;
            mRenderAPI = GLRenderAPIES2;

            if (!eglChooseConfig(display, confAttr, &config, 1, &numConfigs) || numConfigs != 1) {
                assert(false);
            }
        } else{
            assert(false);
        }
    }

    // Create a window surface, and attach it to the Surface we received.
    int surfaceAttribs[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE};

    EGLSurface surface = eglCreatePbufferSurface(display, config, surfaceAttribs);

    checkEglError("eglCreatePbufferSurface");

    EGLContext sharedGLContext = (EGLContext)sharedObject;
    EGLContext context = eglCreateContext(display, config, sharedGLContext, ctxAttr);


    if (context == EGL_NO_CONTEXT) {
        assert(false);
    }

    mSurface = surface;
    mContext = context;
    mDisplay = display;
    mConfig = config;
}

void MTCB::GLContext_Android::checkEglError(std::string msg) {
    int error;
    if ((error = eglGetError()) != EGL_SUCCESS) {
        throw msg + ": EGL error: 0x" +   std::to_string(error);
    }
}

MTCB::GLContext_Android::~GLContext_Android() {
    eglDestroySurface(mDisplay, mSurface);
    eglDestroyContext(mDisplay, mContext);
    eglTerminate(mDisplay);
    eglReleaseThread();

    if (mWindow) {
        ANativeWindow_release(mWindow);
    }
}

void MTCB::GLContext_Android::swapToScreen() {
    eglSwapBuffers(mDisplay, mSurface);
}

void MTCB::GLContext_Android::use() {
    eglMakeCurrent(mDisplay, mSurface, mSurface , mContext);
}

bool MTCB::GLContext_Android::isCurrentContext() {
    return eglGetCurrentContext() == mContext;
}

EGLContext MTCB::GLContext_Android::eglContext() const {
    return mContext;
}

void MTCB::GLContext_Android::resetSurface(ANativeWindow *window) {

    if (!window) {
        return;
    }

    if (mSurface) {
        if (!isCurrentContext()) {
            use();
        }

        eglDestroySurface(mDisplay, mSurface);
        // Create a window surface, and attach it to the Surface we received.
        int surfaceAttribs[] = {
                EGL_NONE
        };
        mSurface = eglCreateWindowSurface(mDisplay, mConfig, window, surfaceAttribs);
        use();
    }
}

#ifdef __cplusplus
extern "C"{
#endif
bool currentContextIsExist(){
    return eglGetCurrentContext() != NULL;
}


jlong Java_com_meitu_android_GLContext_createNativeGLContext(JNIEnv * env, jobject obj, jint glVersion, jobject surface) {
    MTCB::GLRenderAPI api = MTCB::GLRenderAPIES2;
    switch (glVersion) {
        case 1:
            api = MTCB::GLRenderAPIES1;
            break;
        case 2:
            api = MTCB::GLRenderAPIES2;
            break;
        case 3:
            api = MTCB::GLRenderAPIES3;
            break;
        default:break;
    }
    return reinterpret_cast<long>(new MTCB::GLContext_Android(api, NULL, ANativeWindow_fromSurface(env, surface)));
}

void Java_com_meitu_android_GLContext_use(JNIEnv * env, jobject obj, jlong nativeGLContext) {
    MTCB::GLContext_Android *glContext = reinterpret_cast<MTCB::GLContext_Android *>(nativeGLContext);

    if (glContext) {
        glContext->use();
    }
}

void Java_com_meitu_android_GLContext_dispose(JNIEnv * env, jobject obj, jlong nativeGLContext) {
    MTCB::GLContext_Android *glContext = reinterpret_cast<MTCB::GLContext_Android *>(nativeGLContext);

    if (glContext) {
        delete glContext;
    }
}

void Java_com_meitu_android_GLContext_swapToScreen(JNIEnv * env, jobject obj, jlong nativeGLContext) {
    MTCB::GLContext_Android *glContext = reinterpret_cast<MTCB::GLContext_Android *>(nativeGLContext);

    if (glContext) {
        glContext->swapToScreen();
    }
}

#ifdef __cplusplus
}
#endif