

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

private:
	void MoveupDown(float InVal);
	void MoveToPlayer(float InVal);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWeaponDataAsset> WeaponData = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> PickSphereCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* MyCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* PosCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* HeightCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* SclaeCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveWidth = 100.0f;

private:
	float Elapsed = 0;
	FVector InitPos;
	bool bFollow = false;
	AActor* Target;
	float popHeight = 50.0f;
	
};
