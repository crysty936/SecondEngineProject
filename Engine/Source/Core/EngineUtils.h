#pragma once
#include "Logger/Logger.h"


#ifndef NDEBUG
#define ASSERT(x, ...)										\
 {															\
 if(!(x))													\
 {LOG_ERROR("Assertion failed: %s", __VA_ARGS__);			\
__debugbreak();}											\
 }

#define ENSURE(x, ...)										\
  (!!x) && ([x](){											\
  if(!(x))													\
  {LOG_ERROR("Assertion failed: %s", __VA_ARGS__);			\
 __debugbreak();}											\
 return true;												\
  }())														\

#else
#define ASSERT(x, ...)
#define ENSURE(x, ...) x
#endif

#define OUT