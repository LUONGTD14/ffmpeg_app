#include <jni.h>
#include <string>
#include <android/log.h>

// Bạn có thể khai báo ffmpeg_main ở đây nếu bạ n build toàn bộ ffmpeg.c
extern "C" {
int ffmpeg_main(int argc, char** argv);
}

#define LOG_TAG "FFMPEG"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT jint JNICALL
Java_com_luongtd14_ffmpeg_1app_MainActivity_runFFmpegCmd(JNIEnv *env, jobject thiz, jobjectArray cmds) {
    int argc = env->GetArrayLength(cmds);
    char **argv = new char *[argc];

    for (int i = 0; i < argc; ++i) {
        jstring jstr = (jstring) env->GetObjectArrayElement(cmds, i);
        const char *nativeString = env->GetStringUTFChars(jstr, 0);
        argv[i] = strdup(nativeString);
        env->ReleaseStringUTFChars(jstr, nativeString);
    }

    LOGI("Running ffmpeg_main with %d args", argc);
    int result = ffmpeg_main(argc, argv);

    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    delete[] argv;

    return result;
}
