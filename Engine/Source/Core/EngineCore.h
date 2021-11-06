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

extern EngineCore Engine;