#pragma once


class Logger
{
private:
	Logger();
	~Logger();

public:

	inline static Logger& GetLogger() { return Instance; }
	void Log(const char* inFormat, ...);



private:


	static Logger Instance;


};

