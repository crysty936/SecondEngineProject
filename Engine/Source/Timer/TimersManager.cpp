#include "TimersManager.h"

TimersManager* TimersManager::Instance = nullptr;

TimersManager::TimersManager() = default;
TimersManager::~TimersManager() = default;

void TimersManager::Init()
{
	Instance = new TimersManager{};
}

void TimersManager::Terminate()
{
	Get().Timers.clear();
}

void TimersManager::AddTimer(eastl::shared_ptr<TimerBase> inTimer)
{
	Timers.push_back(inTimer);

	inTimer->Start();
}

void TimersManager::TickTimers(float inDeltaT)
{
	using timersIter = eastl::vector<eastl::shared_ptr<TimerBase>>::iterator;

	eastl::vector<timersIter> toRemove;

	for (const eastl::shared_ptr<TimerBase>& timer : Timers)
	{
		if (timer->GetTimeLeft() < 0.f)
		{
			timer->End();
			timersIter currentTimer = eastl::find(Timers.begin(), Timers.end(), timer);

			toRemove.push_back(currentTimer);

			continue;
		}

		timer->Tick(inDeltaT);
	}

 	for (const timersIter& iter : toRemove)
 	{
 		Timers.erase(iter);
 	}
}
