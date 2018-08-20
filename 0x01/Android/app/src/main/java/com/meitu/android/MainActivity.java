package com.meitu.android;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements GLView.GLViewListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        GLView glView = (GLView) findViewById(R.id.GLView);
        glView.listener = this;

    }

    @Override
    public void onViewDidCreateGlResource(GLView view) {
        view.renderer = new TriangleRenderer();
    }
}
