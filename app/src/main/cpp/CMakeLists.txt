cmake_minimum_required(VERSION 3.22.1)

set(NDK_PATH $ENV{ANDROID_NDK})
set(ANDROID_PLATFORM 34)
#set(ANDROID_STL none)
set(ANDROID_ABI arm64-v8a)

set(CMAKE_TOOLCHAIN_FILE: "/Users/tranluong/Library/Android/sdk/ndk/27.0.12077973/build/cmake")
project("ffmpeg_app")

set(CMAKE_CXX_STANDARD 17)
file(REAL_PATH . PROJECT_PATH)

set(EXTERNAL_INCLUDE ${PROJECT_PATH}/mmfw_include)
set(EXTERNAL_LIBRARY ${PROJECT_PATH}/mmfw_library)

add_library(${CMAKE_PROJECT_NAME} SHARED native-lib.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${EXTERNAL_INCLUDE}/include/arm64-v8a
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libavcodec.so
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libavdevice.so
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libavfilter.so
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libavformat.so
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libavutil.so
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libpostproc.so
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libswresample.so
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libswscale.so
        ${EXTERNAL_LIBRARY}/sharedLibraries/arm64-v8a/libsecffmpeg.so
)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_PATH}/../../jniLibs/${ANDROID_ABI}
)