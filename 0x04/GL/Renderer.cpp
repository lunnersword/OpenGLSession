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

uniform float ccc;

void main() {
    gl_FragColor = vec4(position.x, position.x, position.x,ccc);
})";

const char kDebugVertexShader[] = R"(
attribute vec2 vertex;
attribute vec3 color;

uniform mat4 bbb;
varying vec3 inputColor;

void main() {
    gl_Position = bbb * vec4(vertex.xy, 0.0, 1.0);
    inputColor = color;
})";

const char kDebugFragmentShader[] = R"(
precision mediump float;
varying vec3 inputColor;
void main() {
    gl_FragColor = vec4(inputColor,1.0);
})";


const char kCircleVertexShader[] = R"(
attribute vec2 vertex;
attribute vec2 texcoord;

varying vec2 position;
varying vec2 inputTexcoord;

void main() {
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    position = vertex.xy * 0.5 + 0.5;
    inputTexcoord = texcoord;
})";

const char kCircleFragmentShader[] = R"(
precision mediump float;
varying vec2 inputTexcoord;
uniform sampler2D texture;
uniform vec3 aaa;

void main() {
    vec4 color =  vec4(texture2D(texture, inputTexcoord).a);
    
//    if (color.r == 1.0) {
//
//    } else {
////        gl_FragColor = vec4(1.0,0.0,0.0,1.0);
//        discard;
//    }
    
    gl_FragColor = mix(vec4(0.0,0.0,1.0,1.0), vec4(1.0,0.0,0.0,1.0), color.r);
    
//    gl_FragColor =
})";

const char kTextureRendererVertexShader[] = R"(
attribute vec2 vertex;
attribute vec2 texcoord;

varying vec2 inputTexcoord;

void main() {
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    inputTexcoord = texcoord;
})";

const char kTextureRendererFragmentShader[] = R"(
precision mediump float;

varying vec2 inputTexcoord;
uniform sampler2D texture;

void main() {
    vec4 color = texture2D(texture, inputTexcoord);
    gl_FragColor = color;
})";


const char kTextureRendererRepeatFragmentShader[] = R"(
precision highp float;

varying vec2 inputTexcoord;
uniform sampler2D texture;

void main() {
    vec2 modTexcoord = mod(inputTexcoord, vec2(1.0));
    vec4 color = texture2D(texture, modTexcoord);
    gl_FragColor = color;
})";

const char kTextureRendererMirroredRepeatFragmentShader[] = R"(
precision highp float;

varying vec2 inputTexcoord;
uniform sampler2D texture;

void main() {
    vec2 modTexcoord = mod(inputTexcoord, vec2(1.0));
    vec2 modTexcoord1 = floor(mod(inputTexcoord, vec2(2.0)));
    vec2 texcoord = abs(modTexcoord1 - modTexcoord);
    vec4 color = texture2D(texture, texcoord);
    gl_FragColor = color;
})";

const char kEffectRendererMosaicFragmentShader[] = R"(
precision highp float;

varying vec2 inputTexcoord;
uniform sampler2D texture;
uniform float progress;

const float mosaicSize = 800.0;

void main() {
    float blockSize = max(progress * mosaicSize, 2.0);
    vec2 mosaicTexcoord = inputTexcoord * blockSize;
    mosaicTexcoord = floor(mosaicTexcoord);
    vec4 color = texture2D(texture, mosaicTexcoord / blockSize);
    gl_FragColor = color;
})";

const char kEffectRendererSaturationFragmentShader[] = R"(
precision highp float;

varying vec2 inputTexcoord;
uniform sampler2D texture;
uniform float progress;

const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);

void main()
{
    lowp vec4 textureColor = texture2D(texture, inputTexcoord);
    lowp float luminance = dot(textureColor.rgb, luminanceWeighting);
    lowp vec3 greyScaleColor = vec3(luminance);
    
    gl_FragColor = vec4(mix(greyScaleColor, textureColor.rgb, progress), textureColor.w);
    
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

long Java_com_meitu_android_CircleRenderer_createNativeCircleRenderer(JNIEnv * env, jobject obj) {
    return reinterpret_cast<long>(new CircleRenderer());
}

void Java_com_meitu_android_CircleRenderer_render(JNIEnv * env, jobject obj, jlong nativeRenderer, jfloat width, jfloat height) {

    CircleRenderer *renderer = reinterpret_cast<CircleRenderer *>(nativeRenderer);
    if (renderer) {
        renderer->render(_Size((float)width, (float)height));
    }
}

long Java_com_meitu_android_TextureRenderer_createNativeTextureRenderer(JNIEnv * env, jobject obj, jlong nativeTexture) {
    return reinterpret_cast<long>(new TextureRenderer((Texture *)nativeTexture));
}

void Java_com_meitu_android_TextureRenderer_render(JNIEnv * env, jobject obj, jlong nativeRenderer, jfloat width, jfloat height) {

    TextureRenderer *renderer = reinterpret_cast<TextureRenderer *>(nativeRenderer);
    if (renderer) {
        renderer->render(_Size((float)width, (float)height));
    }
}

long Java_com_meitu_android_EffectRenderer_createNativeEffectRenderer(JNIEnv * env, jobject obj, jlong nativeTexture) {
    return reinterpret_cast<long>(new EffectRenderer((Texture *)nativeTexture));
}

void Java_com_meitu_android_EffectRenderer_render(JNIEnv * env, jobject obj, jlong nativeRenderer, jfloat width, jfloat height) {

    EffectRenderer *renderer = reinterpret_cast<EffectRenderer *>(nativeRenderer);
    if (renderer) {
        renderer->render(_Size((float)width, (float)height));
    }
}

void Java_com_meitu_android_EffectRenderer_onValueChange(JNIEnv * env, jobject obj, jlong nativeRenderer, jfloat value) {

    EffectRenderer *renderer = reinterpret_cast<EffectRenderer *>(nativeRenderer);
    if (renderer) {
        renderer->onValueChange(value);
    }
}


};


#endif
