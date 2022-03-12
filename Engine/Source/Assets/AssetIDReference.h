#pragma once

template<class T>
class AssetIDReference
{
public:
	AssetIDReference();
	~AssetIDReference();



private:
	eastl::string AssetId{};
	eastl::shared_ptr<T> AssetPtr{};
};