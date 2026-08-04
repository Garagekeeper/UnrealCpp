

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/Poolable.h"
#include "NiagaraSpawnActor.generated.h"

class UNiagaraSystem;

UCLASS()
class UNREAL_CPP_API ANiagaraSpawnActor : public AActor, public IPoolable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANiagaraSpawnActor();


	UFUNCTION(BlueprintCallable)
	void OnEffectStart(float InDamage);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	/*---------------------------
	*	Poolabel Interface
	---------------------------*/

	UFUNCTION(BlueprintCallable)
	virtual void ReturnPoolObject() override;

	void Onspawn_Implementation();

	void OnReturn_Implementation();


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> DamagePopupWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> PopupTransform;

private:
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TObjectPtr<UDamagePopupWidget> DamagePopupWidget = nullptr;
	UPROPERTY()
	FTimerHandle MyTimerHandle;
	
};
