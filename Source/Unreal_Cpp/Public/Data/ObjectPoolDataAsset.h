

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CommonHeader/ObjectPoolEnums.h"
#include "ObjectPoolDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UObjectPoolDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> ActorClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0"))
	int32 InitialSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0"))
	int32 MaxSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EObjectPoolPolicy PoolPolicy = EObjectPoolPolicy::ForceGrow;
	
};
