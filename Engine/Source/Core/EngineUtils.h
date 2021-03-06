#pragma once
#include "Logger/Logger.h"
#include <stdlib.h>
#include <iostream>

#ifndef NDEBUG
#define ASSERT_MSG(x, inMessage, ...)						\
 {															\
 if(!(x))													\
 {LOG_ERROR(inMessage, __VA_ARGS__);						\
__debugbreak(); std::cin.get();}							\
 }

#define ENSURE_MSG(x, inMessage, ...)						\
  (!!x) && ([&, x](){										\
  if(!(x))													\
  {LOG_ERROR(inMessage, __VA_ARGS__);						\
 __debugbreak();}											\
 return true;												\
  }())														\

#define ASSERT(x)											\
 {															\
 if(!(x))													\
 {															\
	__debugbreak(); std::cin.get();}						\
 }															\

 #define ENSURE(x)											\
   (!!x) && ([x](){											\
   if(!(x))													\
   {														\
  __debugbreak();}											\
  return true;												\
   }())														\

#else
#define ASSERT_MSG(x, ...)	(!!(x))
#define ASSERT(x, ...)		(!!(x))
#define ENSURE_MSG(x, ...)	(!!(x))
#define ENSURE(x)			(!!(x))
#endif

#define OUT