#pragma once

#include "CoreMinimal.h"
#include "Item/PickupBase.h"
#include "Data/WeaponDataAsset.h"
#include "PickupWeapon.generated.h"


class AWeaponActor;
class USphereComponent;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API APickupWeapon : public APickupBase
{
	GENERATED_BODY()

public:
	APickupWeapon();

	void SetDataAssetToPickup(UWeaponDataAsset* asset)
	{
		WeaponData = asset;
	}
	virtual void Init(UPrimaryDataAsset* asset) override;
	
protected:
	virtual void OnPickUp(AActor* OtherActor) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	void DetectPickUp();


	UFUNCTION()
	void OnHitAreaBeginOverlap(
		UPrimitiveComponent* InOverlappedComponent,
		AActor* InOtherActor,
		UPrimitiveComponent* InOtherComp,
		int32					InOtherBodyIndex,
		bool					bFromSweep,
		const FHitResult& InSweepResult
	);

	virtual void MoveToPlayerWithTimer();
	virtual void MoveToplayerWithTimerDone();

private:
	void MoveToPlayerWithTick();
	bool IsAssetReady() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> PickSphereCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Transform")
	TObjectPtr<UCurveFloat> PosCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Transform")
	TObjectPtr<UCurveFloat> HeightCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Transform")
	TObjectPtr<UCurveFloat> ScaleCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Data")
	TObjectPtr<UWeaponDataAsset> WeaponData = nullptr;






private:
	TWeakObjectPtr<AActor> Target;
	FTimerHandle PickupEffectTimerHandle;
	

	
	float popHeight = 50.0f;

	const float TimerInterval = 0.02f;
	float ElapsedForTimer = 0;
	float PickUpEffectDuration = 0.5f;
	
};
