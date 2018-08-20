package com.meitu.android;

public class EffectRenderer extends Renderer {
    public EffectRenderer(Texture texture){
        mNativeHandler = createNativeEffectRenderer(texture.getNativeTexture());
    }

    public void onValueChange(float value){
        onValueChange(mNativeHandler, value);
    }

    private native void onValueChange(long nativeRenderer, float value);
    private native long createNativeEffectRenderer(long nativeTexture);
    protected native void render(long nativeRenderer, float width, float height);
}
