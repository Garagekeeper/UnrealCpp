

#pragma once

#include "CoreMinimal.h"
#include "Item/PickupBase.h"
#include "PickupEffect.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API APickupEffect : public APickupBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnPickUp(AActor* InTarget) override;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 30.0f;
	
};
