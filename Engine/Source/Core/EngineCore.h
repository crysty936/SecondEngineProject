#pragma once

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

public:
	void Init();
	void Run();

};

void StopEngineRunning();

extern EngineCore Engine;
