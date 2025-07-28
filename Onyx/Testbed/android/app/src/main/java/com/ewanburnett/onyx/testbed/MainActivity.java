package com.ewanburnett.onyx.testbed;

import android.util.Log;
import android.view.View;

import androidx.annotation.NonNull;

import com.google.androidgamesdk.GameActivity;

public class MainActivity extends GameActivity {

    static {
        Log.d("Initialization", "Loading libraries...\n");
        try {
            System.loadLibrary("Testbed");
        } catch (RuntimeException e) {
            Log.e("Initialization", e.getMessage().toString());
            throw new RuntimeException(e);
        }
        /*


        System.loadLibrary("assimp");
        System.loadLibrary("glm");
        System.loadLibrary("imgui_lib");
        System.loadLibrary("imguizmo");
        System.loadLibrary("implot");
        System.loadLibrary("openal");

         */
        Log.d("Initialization", "Finished Loading Libraries!\n");

    }



    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            hideSystemUi();
        }
    }

    private void hideSystemUi() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
        );
    }
}