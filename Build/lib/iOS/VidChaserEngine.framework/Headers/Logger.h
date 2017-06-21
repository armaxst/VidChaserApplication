#pragma once

#ifdef __ANDROID__
#include <android/log.h>
#ifndef LOG_TAG
#define  LOG_TAG "MaxstAR-Native"
#endif
#define Log(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#elif defined(WIN32)
extern __declspec(dllexport) void Log(const char* szFormat, ...);
#elif defined(__IOS__) || defined(__MacOS__)
#define Log(...) {printf(__VA_ARGS__); printf("\n");}
#else
#define Log(...) printf(__VA_ARGS__);
#endif
