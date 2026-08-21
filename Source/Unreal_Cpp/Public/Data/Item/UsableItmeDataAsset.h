

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/MiscItemDataAsset.h"
#include "Data/Item/Action/ItemAction.h"
#include "UsableItmeDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UUsableItmeDataAsset : public UMiscItemDataAsset
{
	GENERATED_BODY()
	
public: 
	// UItem의 UPROPERTY 설정으로 인해 에디터 인라인 생성 맟 직렬화 자동 보장
	UPROPERTY(EditAnywhere, Category = "ItemData|Action")
	TObjectPtr<UItemAction> ItemAciton = nullptr;
	
	
};
