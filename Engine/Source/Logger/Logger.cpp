#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <type_traits>
#include "Logger/Logger.h"



Logger::Logger()
{
	static bool bLoggerExisting = false;

	assert(!bLoggerExisting);

	bLoggerExisting = true;
}

Logger::~Logger() = default;

void Logger::Log(const char* inFormat, ...)
{
	// Try to use a stack allocated buffer

 	constexpr int32_t bufferSize = 512;
  	char stackArray[bufferSize];
 
  	va_list argumentList;
  
  	va_start(argumentList, inFormat);
  
  	int32_t result = vsprintf_s(stackArray, bufferSize, inFormat, argumentList);
  
  	va_end(argumentList);
  
  	assert(result != -1);
  
  
     std::cout << stackArray << std::endl;
}

Logger Logger::Instance;
