#pragma  once

#ifdef __ANDROID__
#include <android/log.h>
#ifndef LOG_TAG
#define  LOG_TAG "MaxstAR"
#endif

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#elif defined(_WIN32)
#define  LOGE(...) printf("###MaxstAR Log Error : ", __VA_ARGS__)
#define  LOGI(...) printf("###MaxstAR Log Info  : ", __VA_ARGS__)
#else
#define LOGE
#define LOGI
#endif
