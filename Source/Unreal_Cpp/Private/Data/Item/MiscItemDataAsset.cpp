


#include "Data/Item/MiscItemDataAsset.h"


bool UMiscItemDataAsset::IsLoaded() const
{
	return Super::IsLoaded() && Mesh.IsValid();
}

void UMiscItemDataAsset::OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const
{
	Super::OnAsyncRequest(InOutArray);
	InOutArray.Add(Mesh.ToSoftObjectPath());
}
