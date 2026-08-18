


#include "Item/PickupMisc.h"
#include "Data/Item/MiscItemDataAsset.h"
#include "Component/InventoryCommandTypes.h"
#include "Interface/InventoryUserInterface.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"
#include "Framework/SubSystem/PickupFactorySubsystem.h"


APickupMisc::APickupMisc()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}


void APickupMisc::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (MiscData.IsValid())
	{
		// 여기서는 바로 로딩되는게 좋음
		// 내부에서 get에서 있으면 바로 리턴 없으면 로드
		if (UStaticMesh* StaticMeshData = MiscData->Mesh.LoadSynchronous())
		{
			Mesh->SetStaticMesh(StaticMeshData);
			Mesh->SetRelativeLocation(OffsetInitBP + MiscData->SpwanLocationOffset);
		}
	}
}

void APickupMisc::Init(const UItemDataAsset* asset)
{
	Super::Init(asset);
	if (!asset) return;
	if (const UMiscItemDataAsset* Casted = Cast<UMiscItemDataAsset>(asset))
	{
		MiscData = Casted;
		if (UStaticMesh* SkeletalData = MiscData->Mesh.LoadSynchronous())
		{
			Mesh->SetStaticMesh(SkeletalData);
			Mesh->SetWorldLocation(FVector::ZeroVector);
			Mesh->SetRelativeLocation(OffsetInitBP + MiscData->SpwanLocationOffset);
		}
	}
}

UMeshComponent* APickupMisc::GetMesh() const
{
	return Mesh;
}

void APickupMisc::MoveToplayerWithTimerDone()
{
	Super::MoveToplayerWithTimerDone();
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return;
	if (!GetWorld()) return;
	if (!Target.IsValid()) return;

	UObjectPoolSubsystem* PoolSubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
	UPickupFactorySubsystem* FactorySubSystem = GetWorld()->GetSubsystem<UPickupFactorySubsystem>();
	
	FVector NewLocation = Target.Get()->GetActorLocation() +  FVector(FMath::RandPointInCircle(300.0f), 0.0f);

	if (IInventoryUserInterface* InvecUser = Cast< IInventoryUserInterface>(Target))
	{
		FCommandResult Result;
		if (!InvecUser->ExecuteInventoryCommand(FInventoryCommand::MakeAdd(MiscData.Get(), 1), Result))
		{
			FactorySubSystem->SpawnPickupAsync(MiscData.Get(), FTransform(FRotator::ZeroRotator, NewLocation));
		}
		PoolSubSystem->ReturnPool(this);
	}
}

