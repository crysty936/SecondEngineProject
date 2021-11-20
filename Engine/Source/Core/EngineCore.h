#pragma once
#include <memory>

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

public:
	void Init();
	void Run();

private:
	std::shared_ptr<class Window> MainWindow;


};

extern EngineCore Engine;