

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/StatHolder.h"
#include "CommonHeader/ItemDropTable.h"
#include "EnemyCharacter.generated.h"

class UStatComponent;
class APickupWeapon;

UCLASS()
class UNREAL_CPP_API AEnemyCharacter : public ACharacter, public IStatHolder
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UFUNCTION(BlueprintCallable)
	virtual UStatComponent* GetStatComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnDeath();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

private:
	void DropItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent> StatComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> ItemDropTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APickupWeapon> PickUpWeponActor;
	
};
