//package com.rokid.openxr.android;
//
//import android.Manifest;
//import android.app.Activity;
//import android.content.Context;
//import android.content.pm.PackageManager;
//import android.content.res.AssetManager;
//import android.media.MediaScannerConnection;
//import android.net.Uri;
//import android.os.Bundle;
//import android.util.Log;
//
//import androidx.core.app.ActivityCompat;
//
//import java.util.ArrayList;
//import java.util.List;
//
//public class MainActivity extends android.app.NativeActivity {
//
//    static {
////        System.loadLibrary("openxr_loader");
//        System.loadLibrary("openxr_demo");
//    }
//
//    public native void setNativeAssetManager(AssetManager assetManager);
//
//    @Override
//    protected void onCreate(Bundle savedInstanceState) {
//        super.onCreate(savedInstanceState);
//        setNativeAssetManager(this.getAssets());
//        getPermission(this);
//    }
//
//    private List<String> checkPermission(Context context, String[] checkList) {
//        List<String> list = new ArrayList<>();
//        for (int i = 0; i < checkList.length; i++) {
//            if (PackageManager.PERMISSION_GRANTED != ActivityCompat.checkSelfPermission(context, checkList[i])) {
//                list.add(checkList[i]);
//            }
//        }
//        return list;
//    }
//
//    private void requestPermission(Activity activity, String requestPermissionList[]) {
//        ActivityCompat.requestPermissions(activity, requestPermissionList, 100);
//    }
//
//    @Override
//    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
//        if (requestCode == 100) {
//            for (int i = 0; i < permissions.length; i++) {
//                if (permissions[i].equals(android.Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
//                    if (grantResults[i] == PackageManager.PERMISSION_GRANTED) {
//                        Log.i("RK-Openxr-App", "Successfully applied for storage permission!");
//                    } else {
//                        Log.e("RK-Openxr-App", "Failed to apply for storage permission!");
//                    }
//                }
//            }
//        }
//    }
//
//    private void getPermission(Activity activity) {
//        String[] checkList = new String[]{android.Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.MANAGE_EXTERNAL_STORAGE}; //2025-06-17 添加MANAGE_EXTERNAL_STORAGE权限检查
//        List<String> needRequestList = checkPermission(activity, checkList);
//        if (needRequestList.isEmpty()) {
//            Log.i("RK-Openxr-App", "No need to apply for storage permission!");
//        } else {
//            requestPermission(activity, needRequestList.toArray(new String[needRequestList.size()]));
//        }
//    }
//}


package com.rokid.openxr.android;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.SurfaceTexture;
import android.hardware.camera2.*;
import android.os.Bundle;
import android.util.Log;
import android.util.Size;
import android.os.Environment;
import android.content.Intent;
import android.provider.Settings;
import android.net.Uri;
import android.view.Surface;
import android.view.TextureView;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    static {
//        System.loadLibrary("openxr_loader");
        System.loadLibrary("openxr_demo");
    }

    public native void setNativeAssetManager(AssetManager assetManager);

    public native void onAppInit();

    public native void onCameraImageUpdated(ByteBuffer buffer, int width, int height);
    private static final int REQUEST_CAMERA_PERMISSION = 200;

    private TextureView textureView1;
    private ImageView imageView2;

    private CameraDevice cameraDevice;
    private CameraCaptureSession cameraCaptureSession;
    private CaptureRequest.Builder captureRequestBuilder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        textureView1 = findViewById(R.id.textureView1);
        imageView2 = findViewById(R.id.imageView2);
        textureView1.setSurfaceTextureListener(textureListener);

        setNativeAssetManager(this.getAssets());
        getPermission(this);

        onAppInit();
    }

    private final TextureView.SurfaceTextureListener textureListener = new TextureView.SurfaceTextureListener() {
        @Override
        public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
            openCamera();
        }

        @Override public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {}

        @Override public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) { return false; }

        @Override public void onSurfaceTextureUpdated(SurfaceTexture surface) {
            Bitmap bitmap = textureView1.getBitmap();
            if (bitmap != null) {
                Bitmap mutable = bitmap.copy(Bitmap.Config.ARGB_8888, true);

                ByteBuffer buffer = ByteBuffer.allocateDirect(mutable.getByteCount());
                mutable.copyPixelsToBuffer(buffer);

                onCameraImageUpdated(buffer, bitmap.getWidth(), bitmap.getHeight());

                buffer.rewind();
                mutable.copyPixelsFromBuffer(buffer);
                imageView2.setImageBitmap(mutable);
            }
        }
    };

    private void openCamera() {
        CameraManager manager = (CameraManager) getSystemService(CAMERA_SERVICE);
        try {
            String cameraId = manager.getCameraIdList()[0]; // 使用后置摄像头

            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, REQUEST_CAMERA_PERMISSION);
                return;
            }

            manager.openCamera(cameraId, stateCallback, null);

        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    private final CameraDevice.StateCallback stateCallback = new CameraDevice.StateCallback() {
        @Override public void onOpened(@NonNull CameraDevice camera) {
            cameraDevice = camera;
            createCameraPreview();
        }

        @Override public void onDisconnected(@NonNull CameraDevice camera) {
            camera.close();
        }

        @Override public void onError(@NonNull CameraDevice camera, int error) {
            camera.close();
            cameraDevice = null;
        }
    };

    private void createCameraPreview() {
        try {
            SurfaceTexture texture = textureView1.getSurfaceTexture();
            texture.setDefaultBufferSize(640, 480);
            Surface surface = new Surface(texture);

            captureRequestBuilder = cameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
            captureRequestBuilder.addTarget(surface);

            cameraDevice.createCaptureSession(Arrays.asList(surface), new CameraCaptureSession.StateCallback() {
                @Override public void onConfigured(@NonNull CameraCaptureSession session) {
                    if (cameraDevice == null) return;
                    cameraCaptureSession = session;
                    try {
                        cameraCaptureSession.setRepeatingRequest(captureRequestBuilder.build(), null, null);
                    } catch (CameraAccessException e) {
                        e.printStackTrace();
                    }
                }

                @Override public void onConfigureFailed(@NonNull CameraCaptureSession session) {
                    Toast.makeText(MainActivity.this, "配置失败", Toast.LENGTH_SHORT).show();
                }
            }, null);

        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }

    // 权限请求回调
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_CAMERA_PERMISSION) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                openCamera();
            } else {
                Toast.makeText(this, "需要摄像头权限", Toast.LENGTH_LONG).show();
                finish();
            }
        }
        if (requestCode == 100) {
            for (int i = 0; i < permissions.length; i++) {
                if (permissions[i].equals(android.Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                    if (grantResults[i] == PackageManager.PERMISSION_GRANTED) {
                        Log.i("AndroidTest", "Successfully applied for storage permission!");
                    } else {
                        Log.e("AndroidTest", "Failed to apply for storage permission!");
                    }
                }
            }
        }
    }

    @Override
    protected void onPause() {
        if (cameraDevice != null) {
            cameraDevice.close();
            cameraDevice = null;
        }
        super.onPause();
    }

    private List<String> checkPermission(Context context, String[] checkList) {
        List<String> list = new ArrayList<>();
        for (int i = 0; i < checkList.length; i++) {
            if (PackageManager.PERMISSION_GRANTED != ActivityCompat.checkSelfPermission(context, checkList[i])) {
                list.add(checkList[i]);
            }
        }
        return list;
    }

    private void requestPermission(Activity activity, String requestPermissionList[]) {
        ActivityCompat.requestPermissions(activity, requestPermissionList, 100);
    }

    private void getPermission(Activity activity) {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.R) {
            if (!Environment.isExternalStorageManager()) {
                Log.i("AndroidTest", "请授予“所有文件访问”权限");
                Toast.makeText(this, "请授予“所有文件访问”权限", Toast.LENGTH_LONG).show();
                try {
                    Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                    intent.setData(Uri.parse("package:" + getPackageName()));
                    startActivity(intent);
                } catch (Exception e) {
                    Intent intent = new Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                    startActivity(intent);
                }
            }
            else {
                Log.i("AndroidTest", "已获得 MANAGE_EXTERNAL_STORAGE 权限");
            }
        }
        else { // 对于 Android 10 及以下，使用传统的权限申请方式
            String[] checkList = new String[]{
                    Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.READ_EXTERNAL_STORAGE
            };
            List<String> needRequestList = checkPermission(activity, checkList);
            if (needRequestList.isEmpty()) {
                Log.i("AndroidTest", "No need to apply for storage permission!");
            } else {
                requestPermission(activity, needRequestList.toArray(new String[0]));
            }
        }
    }

}
