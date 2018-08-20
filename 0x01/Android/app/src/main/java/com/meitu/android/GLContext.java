package com.meitu.android;

import android.view.Surface;

public class GLContext {

    private long mNativeGLContext = 0;

    public GLContext(int glVersion, Surface surface){
        mNativeGLContext = createNativeGLContext(glVersion, surface);
    }

    public void use(){
        use(mNativeGLContext);
    }

    public void dispose(){
        if (mNativeGLContext != 0) {
            dispose(mNativeGLContext);
            mNativeGLContext = 0;
        }
    }

    public void swapToScreen(){
        swapToScreen(mNativeGLContext);
    }

    private native long createNativeGLContext(int glVersion, Surface surface);
    private native void use(long nativeGLContext);
    private native void dispose(long nativeGLContext);
    private native void swapToScreen(long nativeGLContext);
}
