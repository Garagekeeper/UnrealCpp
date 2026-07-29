 


#include "Item/PickupWeapon.h"
#include "Weapon/WeaponActor.h"
#include "Unreal_Cpp/Unreal_Cpp.h"
#include "Interface/WeaponUserInterface.h"
#include "Components/SphereComponent.h"

APickupWeapon::APickupWeapon()
{
	PickSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionTest"));
	PickSphereCollision->InitSphereRadius(10.0f);
	PickSphereCollision->SetupAttachment(RootComponent);
}

void APickupWeapon::OnPickUp(AActor* InTarget)
{
	Super::OnPickUp(InTarget);

	//FActorSpawnParameters SpawnParam;
	//SpawnParam.Owner = InTarget;
	//SpawnParam.Instigator = InTarget->GetInstigator();

	//AWeaponActor* Weapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponData, FTransform::Identity, SpawnParam);
	//Weapon->OnEquippedToTarget(InTarget, ECC_Enemy);

	//IWeaponUserInterface::Execute_

	Target = InTarget;
	DetectPickUp();
}

void APickupWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (WeaponData)
	{
		// 여기서는 바로 로딩되는게 좋음
		// 내부에서 get에서 있으면 바로 리턴 없으면 로드
		if (UStaticMesh* StaticMeshData = WeaponData->Mesh.LoadSynchronous())
		{
			Mesh->SetStaticMesh(StaticMeshData);
		}
	}
}

void APickupWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Elapsed += DeltaTime;

	if (bFollow)
	{
		MoveToPlayer(Elapsed);
	}
	else
	{
		MoveupDown(Elapsed);
	}
}

void APickupWeapon::BeginPlay()
{
	Super::BeginPlay();
	InitPos = GetActorLocation();
	PickSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickupWeapon::OnHitAreaBeginOverlap);

}

void APickupWeapon::DetectPickUp()
{
	bFollow = true;
	Elapsed = 0.0f;
	DetectSphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickupWeapon::MoveupDown(float InVal)
{
	if (Elapsed > 3)
		Elapsed -= 3.0f;
	float Dz = MyCurve->GetFloatValue(Elapsed) * MoveWidth;
	SetActorLocation(InitPos + FVector(0, 0, Dz));
}

void APickupWeapon::MoveToPlayer(float InVal)
{
	FVector NewVec = FMath::Lerp(GetActorLocation(), Target->GetActorLocation(), PosCurve->GetFloatValue(Elapsed));
	float ScaleVal = SclaeCurve->GetFloatValue(Elapsed);
	SetActorScale3D(FVector(ScaleVal));
	NewVec = FVector(NewVec.X, NewVec.Y, NewVec.Z + HeightCurve->GetFloatValue(Elapsed));
	SetActorLocation(NewVec);
}

void APickupWeapon::OnHitAreaBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, int32 InOtherBodyIndex, bool bFromSweep, const FHitResult& InSweepResult)
{
	IWeaponUserInterface::Execute_EquipWeapon(InOtherActor, WeaponData);

	Destroy();
}