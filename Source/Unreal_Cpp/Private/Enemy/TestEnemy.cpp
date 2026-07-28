


#include "Enemy/TestEnemy.h"
#include "component/StatComponent.h"
#include "Weapon/WeaponActor.h"

// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

}

UStatComponent* ATestEnemy::GetStatComponent() const
{
	return StatComponent;
}

// Called when the game starts or when spawned
void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}
	
}

// Called every frame
void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ATestEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	IHealthInterface::Execute_ApplyDamage(StatComponent, DamageAmount);
	if (IHealthInterface::Execute_GetCurrentHealth(StatComponent) <= 0)
	{
		Weapon->Destroy();
		this->Destroy();
	}
	return IHealthInterface::Execute_GetCurrentHealth(StatComponent);
}

void ATestEnemy::OnWeaponAttackState(bool bEnable)
{
	OnWeaponAttackStateChaned.ExecuteIfBound(bEnable);
}

void ATestEnemy::Attack()
{
	if (!AnimInstance->IsAnyMontagePlaying())
	{
		PlayAnimMontage(AttackMontage);
	}
}


