#pragma once
#include "Logger/Logger.h"


#ifndef NDEBUG
#define ASSERT_MSG(x, ...)									\
 {															\
 if(!(x))													\
 {LOG_ERROR("Assertion failed: %s", __VA_ARGS__);			\
__debugbreak();}											\
 }

#define ENSURE_MSG(x, ...)									\
  (!!x) && ([x](){											\
  if(!(x))													\
  {LOG_ERROR("Assertion failed: %s", __VA_ARGS__);			\
 __debugbreak();}											\
 return true;												\
  }())														\

#define ASSERT(x)											\
 {															\
 if(!(x))													\
 {	LOG_ERROR("Assertion failed");							\
	__debugbreak();}										\
 }															\

 #define ENSURE(x)											\
   (!!x) && ([x](){											\
   if(!(x))													\
   {														\
  __debugbreak();}											\
  return true;												\
   }())														\

#else
#define ASSERT_MSG(x, ...)
#define ENSURE_MSG(x, ...) x
#define ENSURE(x) x
#endif

#define OUT