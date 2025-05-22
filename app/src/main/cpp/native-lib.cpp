#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <setjmp.h>
#include <stdio.h>
#include "ffmpeg.h"

#define LOG_TAG "FFMPEG_LOG"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Định nghĩa hàm redirect output
void redirectOutputToLogcat() {
    int pipefd[2];
    pipe(pipefd);

    dup2(pipefd[1], STDOUT_FILENO); // redirect stdout
    dup2(pipefd[1], STDERR_FILENO); // redirect stderr
    close(pipefd[1]);

    std::thread([=]() {
        char buffer[1024];
        ssize_t count;
        while ((count = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[count] = '\0';
            LOGI("%s", buffer);
        }
    }).detach();
}

extern "C" void custom_ffmpeg_log_callback(void* ptr, int level, const char* fmt, va_list vl) {
    char line[1024];
    vsnprintf(line, sizeof(line), fmt, vl);
    __android_log_print(ANDROID_LOG_INFO, "FFMPEG_LOG", "%s", line);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_luongtd14_ffmpeg_1app_MainActivity_runFFmpegCmd(JNIEnv *env, jobject /* this */,
                                                         jobjectArray cmds) {
    redirectOutputToLogcat();
    av_log_set_callback(custom_ffmpeg_log_callback);

    jsize argc = env->GetArrayLength(cmds);
    char **argv = new char *[argc+1];

    for (jsize i = 0; i < argc; ++i) {
        jstring jstr = (jstring) env->GetObjectArrayElement(cmds, i);
        const char *cstr = env->GetStringUTFChars(jstr, nullptr);
        argv[i] = new char [strlen(cstr)+1];
        strcpy(argv[i], cstr);
        env->ReleaseStringUTFChars(jstr, cstr);
        env->DeleteLocalRef(jstr);
    }
    argv[argc]= nullptr;

    int ret = ffmpeg_main(argc, argv);
    for(int i = 0; i < argc; i++){
        delete[] argv[i];
    }
    delete[] argv;
    return env->NewStringUTF(reinterpret_cast<const char *>(ret));
}
