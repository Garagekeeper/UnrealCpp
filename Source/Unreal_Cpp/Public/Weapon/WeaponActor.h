
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class UCapsuleComponent;

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
	void InitalizeWeapon(UWeaponDataAsset* InData);

	UFUNCTION(BlueprintCallable)
	void DropWeapon();

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
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> HitArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName AttachSocketName = NAME_None; //hand_rSocket
	FName AttachSocketName = TEXT("hand_rSocket");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UWeaponDataAsset> WeaponData;

private:
	// 이 무기를 장착한 캐릭터
	TWeakObjectPtr<ACharacter> OwnerCharacter;
};
