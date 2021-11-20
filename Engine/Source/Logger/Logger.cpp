#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <type_traits>
#include "Logger/Logger.h"

// Fmt logger is a good source for this


Logger::Logger()
{
	static bool bLoggerExisting = false;

	assert(!bLoggerExisting);

	bLoggerExisting = true;

//     Typed format test
//     int32_t t1 = 1;
//     const char* t2 = "test param";
//     TypedArgsFormatTest("test arguments {0} {1}", t1, t2);
}

Logger::~Logger() = default;

void Logger::Print(const char* inFormat, ...)
{
 #ifdef NDEBUG
     return;
 #endif

	// Try to use a stack allocated buffer
 	constexpr int32_t bufferSize = 512;
  	char stackArray[bufferSize];
 
  	va_list argumentList;
  
  	va_start(argumentList, inFormat);
  
  	int32_t result = vsprintf_s(stackArray, bufferSize, inFormat, argumentList);
  
  	va_end(argumentList);
  
    // Means our allocated buffer is not enough, the log to be printed is too big
  	assert(result != -1);
  
  
     std::cout << stackArray << std::endl;
}

Logger Logger::Instance;
