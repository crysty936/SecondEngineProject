#include "InputSystem/InputEventType.h"

const char* ToString(InputEventType inEvent)
{
	switch (inEvent)
	{
	case InputEventType::InputPress:
		return "InputPress";
		break;
	case InputEventType::InputRelease:
		return "InputRelease";
		break;
	case InputEventType::InputRepeat:
		return "InputRepeat";	
		break;
	default:
		return "Missing Input String";
	}
}
