package com.luongtd14.ffmpeg_app;

import static android.text.TextUtils.replace;

import androidx.appcompat.app.AppCompatActivity;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.Manifest;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.luongtd14.ffmpeg_app.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    String[] cmd = {"ffmpeg", "-encoders"};
    private static final int REQUEST_CODE_PERMISSIONS = 123;
    private static final int REQUEST_CODE_MANAGE_ALL_FILES_ACCESS_PERMISSION = 124;

    private String[] permissions = new String[]{
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE
    };


    // Used to load the 'ffmpeg_app' library on application startup.
    static {
//        System.loadLibrary("secffmpeg");
        System.loadLibrary("c++_shared");
        System.loadLibrary("ffmpeg_app");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            // Android 11 trở lên cần cấp quyền MANAGE_EXTERNAL_STORAGE qua Settings
            if (!Environment.isExternalStorageManager()) {
                Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                intent.setData(Uri.parse("package:" + getPackageName()));
                startActivityForResult(intent, REQUEST_CODE_MANAGE_ALL_FILES_ACCESS_PERMISSION);
            } else {
                // Đã có quyền MANAGE_EXTERNAL_STORAGE
                requestLegacyStoragePermissions();
            }
        } else {
            // Android dưới 11 request READ/WRITE bình thường
            requestLegacyStoragePermissions();
        }

        binding.btnFile.setOnClickListener(v -> {
            Log.e("luongtd14", " " + runFFmpegCmd(cmd));
        });
    }

    private void requestLegacyStoragePermissions() {
        // Kiểm tra quyền READ/WRITE đã cấp chưa
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED
                || ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, permissions, REQUEST_CODE_PERMISSIONS);
        } else {
            // Đã có quyền rồi, làm gì đó
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_CODE_PERMISSIONS) {
            boolean granted = true;
            for (int res : grantResults) {
                if (res != PackageManager.PERMISSION_GRANTED) {
                    granted = false;
                    break;
                }
            }
            if (granted) {
                // Đã được cấp quyền READ/WRITE
            } else {
                // Quyền bị từ chối
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE_MANAGE_ALL_FILES_ACCESS_PERMISSION) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                if (Environment.isExternalStorageManager()) {
                    // Đã cấp quyền quản lý tất cả file
                    requestLegacyStoragePermissions();
                } else {
                    // Quyền quản lý file bị từ chối
                }
            }
        }
    }

    /**
     * A native method that is implemented by the 'ffmpeg_app' native library,
     * which is packaged with this application.
     */
    public native String runFFmpegCmd(String[] cmds);
}
