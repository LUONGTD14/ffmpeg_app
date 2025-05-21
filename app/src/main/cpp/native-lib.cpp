#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <setjmp.h>
#include <stdio.h>

#define LOG_TAG "FFMPEG_APP"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" {
#include "cmdutils.h"
void log_callback_android(void* ptr, int level, const char* fmt, va_list vl);
int ffmpeg_main(int argc, char **argv);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_luongtd14_ffmpeg_1app_MainActivity_runFFmpegCmd(JNIEnv *env, jobject /* this */,
                                                         jstring cmd_) {
    clear_output_buffer();
    static int pipe_fd[2];
    pipe(pipe_fd);
    dup2(pipe_fd[1], STDERR_FILENO); // redirect stderr to pipe

    std::thread([]() {
        char buffer[1024];
        while (true) {
            ssize_t len = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
            if (len > 0) {
                buffer[len] = '\0';
                __android_log_print(ANDROID_LOG_ERROR, "ffmpeg_stderr", "%s", buffer);
            }
        }
    }).detach();
    av_log_set_callback(log_callback_android);
    av_log_set_level(AV_LOG_VERBOSE);
    av_log(NULL, AV_LOG_INFO, "Log callback setup OK!\n");

    if (setjmp(ffmpeg_jmp_buf)) {
        __android_log_print(ANDROID_LOG_ERROR, "ffmpeg_app_ltd", "FFmpeg exited with error\n");
        return (jstring) "-1";
    }

    const char* cmd_cstr = env->GetStringUTFChars(cmd_, NULL);
    if (!cmd_cstr) {
        return env->NewStringUTF("-1"); // error
    }

    // Duplicate string so we can tokenize safely
    char *cmd_copy = strdup(cmd_cstr);
    env->ReleaseStringUTFChars(cmd_, cmd_cstr);

    // Tokenize command string by space to build argv array
    std::vector<char*> argv_vec;
    char *token = strtok(cmd_copy, " ");
    while (token != NULL) {
        argv_vec.push_back(token);
        token = strtok(NULL, " ");
    }
    int argc = (int)argv_vec.size();

    // argv array for ffmpeg_main
    char **argv = argv_vec.data();
    int ret = ffmpeg_main(argc, argv);
    printf("ffmpeg app ltd ret = %d", ret);
    return env->NewStringUTF(get_output_buffer());
}
