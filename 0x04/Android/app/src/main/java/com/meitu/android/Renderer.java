package com.meitu.android;

public abstract class Renderer {
    protected long mNativeHandler = 0;
    public void render(_Size size){
        render(mNativeHandler, size.width(), size.height());
    }

    protected native void render(long nativeRender, float width, float height);
}
