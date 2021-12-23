#pragma once

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

public:
	static void Init();
	static void Terminate();
	void Run();
};

void StopEngineRunning();

extern EngineCore* Engine;
