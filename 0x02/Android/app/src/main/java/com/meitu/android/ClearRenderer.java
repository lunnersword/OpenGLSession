package com.meitu.android;

public class ClearRenderer extends Renderer{

    public ClearRenderer(){
        mNativeHandler = createNativeClearRenderer(1, 0, 0, 1);
    }

    public ClearRenderer(float r, float g, float b, float a){
        mNativeHandler = createNativeClearRenderer(r, g, b, a);
    }

    private native long createNativeClearRenderer(float r, float g, float b, float a);
    protected native void render(long nativeRender, float width, float height);
}