#pragma once

class Renderer
{
public:
	static void Init();
	static void Terminate();
	static void Draw();
	static class WindowsWindow& GetMainWindow();
};