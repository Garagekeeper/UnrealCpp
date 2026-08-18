


#include "Data/WeaponDataAsset.h"
#include "Engine/AssetManager.h"
#include "Engine/SkeletalMesh.h"
#include "NiagaraSystem.h"


bool UWeaponDataAsset::IsLoaded() const
{
	return Super::IsLoaded() && Mesh.IsValid() && WeaponTrailVFX.IsValid();
}

void UWeaponDataAsset::OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const
{
	Super::OnAsyncRequest(InOutArray);
	InOutArray.Add(Mesh.ToSoftObjectPath());
	InOutArray.Add(WeaponTrailVFX.ToSoftObjectPath());
}
