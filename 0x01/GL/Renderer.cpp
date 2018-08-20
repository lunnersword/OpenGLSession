//
//  Renderer.cpp
//  0x00
//
//  Created by ZhangXiaoJun on 2018/6/26.
//  Copyright © 2018年 meitu. All rights reserved.
//

#include "Renderer.hpp"

const char kVertexShader[] = R"(
attribute vec2 vertex;
attribute vec3 color;

varying vec3 inColor;
varying vec2 position;

void main() {
    gl_Position = vec4(vertex.xy, 0, 1);
    position = vertex.xy * 0.5 + 0.5;
    inColor = color;
    gl_PointSize = 10.0;
})";

const char kFragmentShader[] = R"(
precision mediump float;
varying vec3 inColor;
varying vec2 position;

void main() {
    gl_FragColor = vec4(position.x, position.x, position.x,1.0);
})";

const char kDebugVertexShader[] = R"(
attribute vec2 vertex;
attribute vec3 color;
varying vec3 inputColor;

void main() {
    gl_Position = vec4(vertex.xy, 0, 1);
    inputColor = color;
})";

const char kDebugFragmentShader[] = R"(
precision mediump float;
varying vec3 inputColor;
void main() {
    gl_FragColor = vec4(inputColor,1.0);
})";


#ifdef MTCB_PLATFORM_ANDROID

#include <jni.h>

extern "C"{

void Java_com_meitu_android_Renderer_render(JNIEnv * env, jobject obj, jlong nativeRenderer, jfloat width, jfloat height) {
    assert(false);
}

long Java_com_meitu_android_ClearRenderer_createNativeClearRenderer(JNIEnv * env, jobject obj, jfloat r, jfloat g, jfloat b, jfloat a) {
    return reinterpret_cast<long>(new ClearRenderer(r, g, b, a));
}

void Java_com_meitu_android_ClearRenderer_render(JNIEnv * env, jobject obj, jlong nativeRenderer, jfloat width, jfloat height) {

    ClearRenderer *renderer = reinterpret_cast<ClearRenderer *>(nativeRenderer);
    if (renderer) {
        renderer->render(_Size((float)width, (float)height));
    }
}

long Java_com_meitu_android_TriangleRenderer_createNativeTriangleRenderer(JNIEnv * env, jobject obj) {
    return reinterpret_cast<long>(new TriangleRenderer());
}

void Java_com_meitu_android_TriangleRenderer_render(JNIEnv * env, jobject obj, jlong nativeRenderer, jfloat width, jfloat height) {

    TriangleRenderer *renderer = reinterpret_cast<TriangleRenderer *>(nativeRenderer);
    if (renderer) {
        renderer->render(_Size((float)width, (float)height));
    }
}

};


#endif
