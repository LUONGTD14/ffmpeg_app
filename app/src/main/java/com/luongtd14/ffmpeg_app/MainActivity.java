package com.luongtd14.ffmpeg_app;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.luongtd14.ffmpeg_app.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'ffmpeg_app' library on application startup.
    static {
        System.loadLibrary("ffmpeg_app");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());


    }

    /**
     * A native method that is implemented by the 'ffmpeg_app' native library,
     * which is packaged with this application.
     */
    public native int runFFmpegCmd(String[] argv);
}
