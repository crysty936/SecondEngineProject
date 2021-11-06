#pragma once


class Logger
{
private:
	Logger();
	~Logger();

public:

	inline static Logger& GetLogger() { return Instance; }
	void Log(const char* inFormat, const Args&... inArgs);



private:


	static Logger Instance;


};

