#pragma once


enum FormatValueType : char
{
	intType,
	uintType,
	floatType,
	doubleType,
	stringType
};

union allFormatValues
{
	allFormatValues(int32_t inValue) : int_value{ inValue } {}
	allFormatValues(uint32_t inValue) : uint_value{ inValue } {}
	allFormatValues(float inValue) : float_value{ inValue } {}
	allFormatValues(double inValue) : double_value{ inValue } {}
	allFormatValues(const char* inValue) : string_value{ inValue } {}


	int32_t int_value;
	uint32_t uint_value;
	float float_value;
	double double_value;
	const char* string_value;
};

class ArgsContainer
{
public:

	ArgsContainer(int32_t inValue)
		:value{ inValue }, Type{ FormatValueType::intType } {}

	ArgsContainer(uint32_t inValue)
		:value{ inValue }, Type{ FormatValueType::uintType } {}

	ArgsContainer(float inValue)
		:value{ inValue }, Type{ FormatValueType::floatType } {}

	ArgsContainer(double inValue)
		:value{ inValue }, Type{ FormatValueType::doubleType } {}

	ArgsContainer(const char* inValue)
		:value{ inValue }, Type{ FormatValueType::stringType } {}

	allFormatValues value;
	FormatValueType Type;
};


class Logger
{
private:
	Logger();
	~Logger();

public:

	inline static Logger& Get() { return Instance; }
	void Print(const char* inFormat, ...);

private:
	// To be developed
 	template<typename... Args>
 	void TypedArgsFormatTest(const char* inFormat, const Args&... inArgs)
 	{

 		ArgsContainer args[] = { inArgs... };

		ArgsContainer& arg1 = args[0];

		if (arg1.Type == FormatValueType::intType)
		{
			int32_t value = arg1.value.int_value;

			int a = 0;
		}

 		ArgsContainer& arg2 = args[1];
 
 		if (arg2.Type == FormatValueType::stringType)
 		{
 			const char* value = arg2.value.string_value;
 
 			int a = 0;
 		}
 	}

private:
	static Logger Instance;


};

