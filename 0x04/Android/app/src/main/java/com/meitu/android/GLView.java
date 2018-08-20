package com.meitu.android;

import android.content.Context;
import android.opengl.GLES20;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class GLView extends SurfaceView implements SurfaceHolder.Callback {

    private GLContext mGLContext = null;
    private HandlerThread mGLViewHandlerThread = null;
    private android.os.Handler mRenderHandler = null;

    private final int kOnUpdate = -9999;
    private final int kOnSurfaceCreated = -9998;
    private final int kOnSurfaceDestroyed= -9997;
    private final int kOnLayout = -9996;

    public Renderer renderer = null;
    public GLViewListener listener = null;

    public interface GLViewListener {
        void onViewDidCreateGlResource(GLView view);
    }


    public GLView(Context context, AttributeSet attrs){
        super(context, attrs);
        init();
    }

    public GLView(Context context, AttributeSet attrs, int defStyle){
        super(context, attrs, defStyle);
        init();
    }

    public GLView(Context context) {
        super(context);
        init();
    }

    private void init(){
        getHolder().addCallback(this);
        mGLViewHandlerThread = new HandlerThread("com.meitu.GLView");
        mGLViewHandlerThread.start();

        mRenderHandler =  new Handler(mGLViewHandlerThread.getLooper(), new Handler.Callback() {
            @Override
            public boolean handleMessage(Message msg) {
                switch (msg.what) {
                    case kOnUpdate:

                        if (mGLContext != null) {
                            mGLContext.use();
                            GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);

                            if (renderer != null) {
                                renderer.render(new _Size(GLView.this.getWidth(), GLView.this.getHeight()));
                            }
                            mGLContext.swapToScreen();

                        }

                        Message reMsg = new Message();
                        reMsg.what = kOnUpdate;
                        mRenderHandler.sendMessageDelayed(reMsg,16);
                        break;
                    case kOnLayout:

                        break;
                    case kOnSurfaceCreated:
                        mGLContext = new GLContext(2, GLView.this.getHolder().getSurface());

                        if (listener != null) {
                            mGLContext.use();
                            listener.onViewDidCreateGlResource(GLView.this);
                        }

                        break;
                    case kOnSurfaceDestroyed:
                        mGLContext.dispose();
                        mGLContext = null;
                        break;
                }
                return false;
            }
        });

        Message msg = new Message();
        msg.what = kOnUpdate;
        mRenderHandler.sendMessage(msg);
    }

    public GLContext glContext(){
        return mGLContext;
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Message msg = new Message();
        msg.what = kOnSurfaceCreated;
        mRenderHandler.sendMessage(msg);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Message msg = new Message();
        msg.what = kOnLayout;
        mRenderHandler.sendMessage(msg);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Message msg = new Message();
        msg.what = kOnSurfaceDestroyed;
        mRenderHandler.sendMessage(msg);
    }
}
