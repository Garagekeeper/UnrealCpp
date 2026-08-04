

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/Poolable.h"
#include "DamagepopupActor.generated.h"

class UWidgetComponent;
class UDamagePopupWidget;

UCLASS()
class UNREAL_CPP_API ADamagepopupActor : public AActor, public IPoolable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagepopupActor();

	UFUNCTION(BlueprintCallable)
	void OnPopupStart(float InDamage);

	/*---------------------------
	*	Poolabel Interface
	---------------------------*/
	
	UFUNCTION(BlueprintCallable)
	virtual void ReturnPoolObject() override;

	void Onspawn_Implementation();

	void OnReturn_Implementation();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> DamagePopupWidgetComponent;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDamagePopupWidget> DamagePopupWidget = nullptr;
	
};
