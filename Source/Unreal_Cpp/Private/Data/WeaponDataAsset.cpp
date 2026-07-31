


#include "Data/WeaponDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/SkeletalMesh.h"

TSharedPtr<FStreamableHandle> UWeaponDataAsset::RequestDataLoad(FStreamableDelegate InDelegate)
{
	// 로드할 에셋들의 경로를 TArray에 담기
	TArray<FSoftObjectPath> TargetsToLoad;
	TargetsToLoad.Add(Mesh.ToSoftObjectPath());
	TargetsToLoad.Add(WeaponTrailVFX.ToSoftObjectPath());

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	return Streamable.RequestAsyncLoad(TargetsToLoad, MoveTemp(InDelegate));
}

bool UWeaponDataAsset::IsLoaded() const
{
	return Mesh.IsValid() && WeaponTrailVFX.IsValid();
}
