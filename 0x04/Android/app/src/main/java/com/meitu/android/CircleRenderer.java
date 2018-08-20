package com.meitu.android;

public class CircleRenderer extends Renderer {
    public CircleRenderer(){
        mNativeHandler = createNativeCircleRenderer();
    }

    private native long createNativeCircleRenderer();
    protected native void render(long nativeRender, float width, float height);
}
