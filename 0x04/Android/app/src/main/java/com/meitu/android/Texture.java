package com.meitu.android;

import android.graphics.Bitmap;
import android.opengl.GLES20;
import android.util.Log;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public class Texture {

    private long mNativeTexture = 0;

    public Texture(Bitmap bitmap){
        Bitmap.Config config = bitmap.getConfig();

        int internalFormat = GLES20.GL_RGBA;
        switch (config) {
            case ALPHA_8:
                internalFormat = GLES20.GL_ALPHA;
                break;
            case ARGB_4444:
                internalFormat = GLES20.GL_RGBA4;
                break;
            case ARGB_8888:
                internalFormat = GLES20.GL_RGBA;
                break;
            case RGB_565:
                internalFormat = GLES20.GL_RGB565;
                break;
            case RGBA_F16:
            case HARDWARE:
            default:
                break;
        }

        ByteBuffer buffer = ByteBuffer.allocateDirect(bitmap.getRowBytes() * bitmap.getHeight());
        bitmap.copyPixelsToBuffer(buffer);
        mNativeTexture = createNativeTexture(buffer, bitmap.getWidth(), bitmap.getHeight(), internalFormat, internalFormat);
    }

    long getNativeTexture(){
        return mNativeTexture;
    }

    /**
     * Converts bitmap to byte array in PNG format
     * @param bitmap source bitmap
     * @return result byte array
     */
    private static byte[] convertBitmapToByteArray(Bitmap bitmap){
        ByteArrayOutputStream baos = null;
        try {
            baos = new ByteArrayOutputStream();
            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, baos);
            return baos.toByteArray();
        }finally {
            if(baos != null){
                try {
                    baos.close();
                } catch (IOException e) {
                    Log.e("Texture", "ByteArrayOutputStream was not closed");
                }
            }
        }
    }

    private native long createNativeTexture(ByteBuffer buffer, int width, int height, int format, int internalFormat);
}
