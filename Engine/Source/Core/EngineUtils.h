#pragma once
#include "Logger/Logger.h"


#ifndef NDEBUG
#define ASSERT(x, ...)										\
 {															\
 if(!(x))													\
 {LOG_ERROR("Assertion Failed");							\
__debugbreak();}											\
}

#else
#define ASSERT(x, ...)
#endif