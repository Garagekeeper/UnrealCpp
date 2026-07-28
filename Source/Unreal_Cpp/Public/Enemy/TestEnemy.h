

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/StatHolder.h"
#include "Interface/WeaponUserInterface.h"
#include "TestEnemy.generated.h"

class UStatComponent;
class AWeaponActor;

UCLASS()
class UNREAL_CPP_API ATestEnemy : public ACharacter, public IStatHolder, public IWeaponUserInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestEnemy();

	virtual UStatComponent* GetStatComponent() const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	virtual void OnWeaponAttackState(bool bEnable) override;
	virtual FOnWeaponAttackStateChanged& GetWeaponAttackStateChangedDelegate() override
	{
		return OnWeaponAttackStateChaned;
	};

	virtual void SetWeapon(AWeaponActor* InWeapon) override
	{
		Weapon = InWeapon;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Attack();



public:
	FOnWeaponAttackStateChanged OnWeaponAttackStateChaned;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStatComponent> StatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AWeaponActor> Weapon = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance = nullptr;


	
	
};
