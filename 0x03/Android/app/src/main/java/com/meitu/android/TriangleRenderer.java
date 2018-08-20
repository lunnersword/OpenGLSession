package com.meitu.android;

public class TriangleRenderer extends Renderer {
    public TriangleRenderer(){
        mNativeHandler = createNativeTriangleRenderer();
    }

    private native long createNativeTriangleRenderer();
    protected native void render(long nativeRender, float width, float height);
}
