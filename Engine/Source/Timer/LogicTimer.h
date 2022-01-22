#pragma once
#include "TimerBase.h"
#include "EventSystem/EventSystem.h"

// DeltaT, TimeLeft
using LogicDelegate = Delegate<void, float, float>;

/**
 * Timer that calls custom logic each frame
 */
class LogicTimer : public TimerBase
{
public:
	LogicTimer(float inDeltaT, LogicDelegate inOnLogic);
	virtual ~LogicTimer();

	virtual void Tick(float inDeltaT) override;

private:
	LogicDelegate OnLogicDelegate;
};