

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DummyActor.generated.h"

class ADamagepopupActor;
class ANiagaraSpawnActor;

UCLASS()
class UNREAL_CPP_API ADummyActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADummyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> PopupTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ADamagepopupActor> DamagePopup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ANiagaraSpawnActor> NiagaraPopup;

};
