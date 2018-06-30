#pragma once

#include <cassert>


#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

#define LS_ASSUME(expr) if(!(expr)) { __builtin_unreachable(); }

#elif defined(_MSC_VER)

#define LS_ASSUME(expr) (__assume(expr))

#else

#define LS_ASSUME(expr) ((void)0)

#endif


#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

#define LS_FORCEINLINE_ACTUAL __attribute__((always_inline))

#elif defined(_MSC_VER)

#define LS_FORCEINLINE_ACTUAL __forceinline

#else

#define LS_FORCEINLINE_ACTUAL inline

#endif


#ifdef NDEBUG

#define LS_ASSERT(expr) LS_ASSUME(expr)

#else

#define LS_ASSERT(expr) (assert(expr));

#endif


#ifdef NDEBUG

#define LS_FORCEINLINE LS_FORCEINLINE_ACTUAL

#else

#define LS_FORCEINLINE

#endif


#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)

#define LS_UNREACHABLE() (__builtin_unreachable())

#elif defined(_MSC_VER)

#define LS_UNREACHABLE() (__assume(0))

#else

#define LS_UNREACHABLE() ((void)0)

#endif
