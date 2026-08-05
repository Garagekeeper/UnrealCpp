#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StreamableManager.h"
#include "WeaponDataAsset.generated.h"

class USkeletalMesh;
class UNiagaraSystem;

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
	TSoftObjectPtr<USkeletalMesh> Mesh;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	int UsageCount = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> WeaponTrailVFX;

	// 무기의 범위공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AreaAttackPower = 50.0f;

	// 무기의 범위 공격의 반지름(안쪽, 이 안쪽은 100% 데미지)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AreaAttackInnerRadius = 100.0f;

	// 무기의 범위 공격의 반지름(바깥, Inner ~ Outter범위는 거리에 따라 감소)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	float AreaAttackOutterRadius = 300.0f;

	
protected:
};
