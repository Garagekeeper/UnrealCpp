


#include "Data/Item/ItemDataAsset.h"
#include "Engine/AssetManager.h"

TSharedPtr<FStreamableHandle> UItemDataAsset::RequestDataLoad(FStreamableDelegate InDelegate) const
{
	TArray<FSoftObjectPath> TargetsToLoad;
	OnAsyncRequest(TargetsToLoad);

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	return Streamable.RequestAsyncLoad(TargetsToLoad, MoveTemp(InDelegate));
}

bool UItemDataAsset::IsLoaded() const
{
	return PickUpActor.IsValid() && (ItemIcon.IsNull() || ItemIcon.IsValid());
}

void UItemDataAsset::OnAsyncRequest(TArray<FSoftObjectPath>& InOutArray) const
{
	InOutArray.Add(PickUpActor.ToSoftObjectPath());
	InOutArray.Add(ItemIcon.ToSoftObjectPath());

	// 상속받은 클래스에서 추가 요소들 추가 등록
}




