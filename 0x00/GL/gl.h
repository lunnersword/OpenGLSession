#ifndef gl_h
#define gl_h

#include "platform.h"



#if defined(MTCB_PLATFORM_IOS_DEVICE) || defined(MTCB_PLATFORM_IOS_SIMULATOR)
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#elif defined(MTCB_PLATFORM_ANDROID)
#include <GLES3/gl3.h>
#define __gl2_h_
#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#endif

#define MTCB_STRINGIZE(x) #x
#define MTCB_STRINGIZE2(x) MTCB_STRINGIZE(x)
#define MTCB_SHADER_STRING(text) MTCB_STRINGIZE2(text)


#endif /* gl_h */
