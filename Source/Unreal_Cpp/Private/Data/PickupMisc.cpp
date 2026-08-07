


#include "Data/PickupMisc.h"
#include "Data/Item/MiscItemDataAsset.h"

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

void APickupMisc::Init(UItemDataAsset* asset)
{
	Super::Init(asset);
	if (!asset) return;
	if (UMiscItemDataAsset* Casted = Cast<UMiscItemDataAsset>(asset))
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
