package com.meitu.android;

public class TextureRenderer extends Renderer {
    public TextureRenderer(Texture texture){
        mNativeHandler = createNativeTextureRenderer(texture.getNativeTexture());
    }

    private native long createNativeTextureRenderer(long nativeTexture);
    protected native void render(long nativeRender, float width, float height);
}
