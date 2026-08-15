
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class UNREAL_CPP_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	UFUNCTION(BlueprintCallable)
	void AttackEnable(bool bEnable);


	UFUNCTION(BlueprintCallable)
	void OnEquippedToTarget(AActor* InOwner, ECollisionChannel TargetChanel);


	UFUNCTION(BlueprintCallable)
	void InitalizeWeapon(const UWeaponDataAsset* InData);

	UFUNCTION(BlueprintCallable)
	void DropWeapon();

	UFUNCTION(BlueprintCallable)
	void RestUseCnt();

	UFUNCTION(BlueprintCallable)
	FVector GetWeaponImpactLocation() const;

	void SetActivator(bool bActive);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnEquipped(AActor* InOwner, ECollisionChannel TargetChanel);


	UFUNCTION()
	void OnHitAreaBeginOverlap(
		UPrimitiveComponent*	InOverlappedComponent,
		AActor*					InOtherActor,
		UPrimitiveComponent*	InOtherComp,
		int32					InOtherBodyIndex,
		bool					bFromSweep,
		const FHitResult&		InSweepResult
	);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> HitArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName AttachSocketName = NAME_None; //hand_rSocket
	FName AttachSocketName = TEXT("hand_rSocket");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const UWeaponDataAsset> WeaponData;

	// 드랍 직후에 플레이어와 물리 상호작용이 안되는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicsDelay = 0.8f;

	// 드랍 후 사라질때 까지의 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropLifeSpan = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> TrailVFX;


private:
	// 이 무기를 장착한 캐릭터
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	FTimerHandle PhysicsDelayTimerHandle;

	// 사용횟수, -1이면 무한사용
	// 양수면 0도달시 파괴됨
	int UsageCount = -1;
};
