


#include "Enemy/EnemyCharacter.h"
#include "Component/StatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Unreal_Cpp/Unreal_Cpp.h"
#include "CommonHeader/ItemDropTable.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"
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
	if (IsValid(ItemDropTable))
	{
		float RandNum = FMath::FRandRange(0.0, 1.0);
		for (const auto& [Key, Value] : ItemDropTable->GetRowMap())
		{
			FItemDropTableRow* DropTableRow = reinterpret_cast<FItemDropTableRow*>(Value);
			if (!DropTableRow) continue;

			UGameInstance* GameInstance = GetGameInstance();

			float currentItemSpawnRate = DropTableRow->DropRate;
			if (currentItemSpawnRate < RandNum)
			{

				FActorSpawnParameters SpawnParam;
				SpawnParam.Owner = nullptr;
				SpawnParam.ObjectFlags = RF_Transient;
				FTransform NewTransform = GetTransform();
				FVector NewLocation = NewTransform.GetLocation();
				NewLocation.X += FMath::FRandRange(-100.0, 100.0);
				NewLocation.Y += FMath::FRandRange(-100.0, 100.0);
				NewTransform.SetLocation(NewLocation);
				//AActor* Spawned = GetWorld()->SpawnActor<AActor>(PickUpWeponActor, NewTransform, SpawnParam);
				//APickupWeapon* SpawnedWeaponPickUp = Cast<APickupWeapon>(Spawned);
				//SpawnedWeaponPickUp->Init(DropTableRow->DropItemAsset.LoadSynchronous());

				if (!GameInstance) continue;
				UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
				APickupWeapon* SpawnedWeaponPickUp = SubSystem->Spawn(PickUpWeponActor, NewTransform);
				SpawnedWeaponPickUp->Init(DropTableRow->DropItemAsset.LoadSynchronous());
				UE_LOG(LogTemp, Log, TEXT("%s Spawned, spawn rate : %f"), *(DropTableRow->DropItemAsset.GetAssetName()), currentItemSpawnRate);
			}

		}

	}

	return;
}


