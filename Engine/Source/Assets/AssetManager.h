#pragma once

class AssetManager
{
private:
	AssetManager();
	~AssetManager();

public:
	static void LoadAssets();

private:
	static void LoadMaterials();
};