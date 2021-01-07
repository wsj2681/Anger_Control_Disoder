#pragma once

// 전체화면을 사용합니다.
//#define _WITH_SWAPCHAIN_FULLSCREEN_STATE

#define CLIENT_BUFFER_WIDTH 1280
#define CLIENT_BUFFER_HEIGHT 1024

// HRESULT전용 ASSERT입니다.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x) \
      if (FAILED((x))) \
      { \
         LPVOID errorLog = nullptr; \
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, \
            nullptr, (x), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
            reinterpret_cast<LPWSTR>(&errorLog), 0, nullptr); \
         fprintf(stderr, "%s", static_cast<char*>(errorLog)); \
         LocalFree(errorLog); \
         __debugbreak(); \
      }
#endif
#else
#ifndef HR
#define   HR(x) (x);
#endif
#endif

// 사용자정의 ASSERT입니다.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef ASSERT
#define ASSERT(expr, msg) \
      if (!(expr)) \
      { \
         fprintf(stderr, "%s, %s, %s(%d)", (msg), (#expr), __FILE__, __LINE__); \
         __debugbreak(); \
      }
#endif
#else
#ifndef ASSERT
#define ASSERT(expr, msg) ((void)0)
#endif

#endif

#define SAFE_RELEASE(x) if(x) x->Release();
#define RELEASE_UPLOADBUFFERS(x) if((x)) (x)->ReleaseUploadBuffers();

#define ASPECT_RATIO (float(CLIENT_BUFFER_WIDTH) / float(CLIENT_BUFFER_HEIGHT))
#define RANDOM_COLOR XMFLOAT4(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX))
