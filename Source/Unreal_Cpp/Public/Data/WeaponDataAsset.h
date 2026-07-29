#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StreamableManager.h"
#include "WeaponDataAsset.generated.h"

class UStaticMesh;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	TSharedPtr<FStreamableHandle> RequestDataLoad(FStreamableDelegate InDelegate);
	bool IsLoaded() const;

public:

	// Mesh Of Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TSoftObjectPtr<UStaticMesh> Mesh;
	
	// Socket to Attach
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FName AttachSocketName = TEXT("hand_rSocket");

	// Loaction in Attached Socket
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	FVector LocationOffset = FVector::ZeroVector;

	// half height of capsule
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float hitAreaHalfheight = 70.0f;

	// radius of capsule
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitArea")
	float hitAreaRad = 40.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AttackDamage = 10.0f;
	
protected:
};
