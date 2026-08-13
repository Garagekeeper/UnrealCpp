


#include "Enemy/EnemyCharacter.h"
#include "Component/StatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Unreal_Cpp/Unreal_Cpp.h"
#include "CommonHeader/ItemDropTable.h"
#include "Framework/SubSystem/PickupFactorySubsystem.h"
#include "Item/PickupWeapon.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Enemy);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UStatComponent* AEnemyCharacter::GetStatComponent() const
{
	return StatComp;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(StatComp))
	StatComp->OnDeath.AddDynamic(this, &AEnemyCharacter::OnDeath);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::OnDeath()
{
	UE_LOG(LogTemp, Log, TEXT("Enemy Char Death"));
	DropItem();
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (UStatComponent* Stat = GetStatComponent())
	{
		IHealthInterface::Execute_ApplyDamage(Stat, Damage);
	}
	return Damage;
}



void AEnemyCharacter::DropItem()
{
	if (!IsValid(ItemDropTable)) return;

	float RandNum = FMath::FRandRange(0.0, 1.0);
	for (const auto& [Key, Value] : ItemDropTable->GetRowMap())
	{
		FItemDropTableRow* DropTableRow = reinterpret_cast<FItemDropTableRow*>(Value);
		if (!DropTableRow) continue;
		float currentItemSpawnRate = DropTableRow->DropRate;
		if (currentItemSpawnRate >= RandNum) continue;


		UItemDataAsset* PickupData = DropTableRow->DropItemAsset;

		UGameInstance* GameInstance = GetGameInstance();
		if (!GameInstance) return;
		//UPickupFactorySubsystem* SubSystem = GameInstance->GetSubsystem<UPickupFactorySubsystem>();
		UPickupFactorySubsystem* SubSystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>();

		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = nullptr;
		SpawnParam.ObjectFlags = RF_Transient;
		FTransform NewTransform = GetTransform();
		FVector NewLocation = NewTransform.GetLocation();
		NewLocation.X += FMath::FRandRange(-100.0, 100.0);
		NewLocation.Y += FMath::FRandRange(-100.0, 100.0);
		NewTransform.SetLocation(NewLocation);

		FOnpickupSpawned Delegate;
		Delegate.BindDynamic(this, &AEnemyCharacter::OnItemSpawnd);

		if (SubSystem)
		{
			SubSystem->SpawnPickupAsync(PickupData, NewTransform, Delegate);
		}

		//if (!PickupData->IsLoaded())
		//{
		//	PickupData->RequestDataLoad(
		//		FStreamableDelegate::CreateWeakLambda(
		//			this,
		//			[this, SubSystem, PickupData, NewTransform, Delegate]()
		//			{
		//				SubSystem->SpawnPickupAsync(PickupData, NewTransform, Delegate);
		//			}
		//		)
		//	);
		//}
		//else
		//{
		//	//SubSystem->SpawnPickup(PickupData, NewTransform);
		//}

	}
}

void AEnemyCharacter::OnItemSpawnd(APickupBase* Spawned)
{
	
}




