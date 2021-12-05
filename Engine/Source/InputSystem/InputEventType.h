#pragma once

enum class InputEventType
{
	None,
  	InputPress,
	InputRelease,
	InputRepeat
};

const char* ToString(InputEventType inEvent);