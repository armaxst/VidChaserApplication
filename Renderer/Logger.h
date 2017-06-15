#pragma  once

#ifndef LOG_TAG
#define  LOG_TAG "NativeRenderer"
#endif

#ifdef __ANDROID__
#include <android/log.h>

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#elif defined(_WIN32)
#define  LOGE(...) printf(LOG_TAG, __VA_ARGS__)
#define  LOGI(...) printf(LOG_TAG, __VA_ARGS__)
#define  LOGD(...) printf(LOG_TAG, __VA_ARGS__)
#else
#define LOGE(...) {printf(__VA_ARGS__); printf("\n");}
#define LOGI(...) {printf(__VA_ARGS__); printf("\n");}
#define LOGD(...) {printf(__VA_ARGS__); printf("\n");}
#endif
