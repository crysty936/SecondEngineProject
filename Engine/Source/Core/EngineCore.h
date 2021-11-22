#pragma once
#include <memory>
#include "EASTL/shared_ptr.h"

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

public:
	void Init();
	void Run();

private:
	eastl::shared_ptr<class OpenGLWindow> MainWindow;

};

extern EngineCore Engine;