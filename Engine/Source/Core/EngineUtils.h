#pragma once
#include "Logger/Logger.h"


#ifndef NDEBUG
//#define ASSERT(x, ...){if(!(x){__debugbreak();}}
#define ASSERT(x, ...)										\
 {															\
 if(!(x))													\
 {Logger::Get().Print("Assertion Failed");					\
__debugbreak();}											\
}

#else
#define ASSERT(x, ...)
#endif