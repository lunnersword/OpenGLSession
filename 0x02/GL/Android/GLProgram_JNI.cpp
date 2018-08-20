//
// Created by Zed on 2018/6/4.
//

#include "GLProgram.hpp"
#include <jni.h>

using namespace MTCB;

#ifdef __cplusplus
extern "C" {
#endif

void Java_com_meitu_cplusplusbase_GLProgram_nativeUse(JNIEnv *env,
                                                      jclass clazz,
                                                      jlong nativeProgram) {
    GLProgram *program = reinterpret_cast<GLProgram *>(nativeProgram);
    program->use();
}

#ifdef __cplusplus
};
#endif

