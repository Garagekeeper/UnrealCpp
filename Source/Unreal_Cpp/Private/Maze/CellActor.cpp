


#include "Maze/CellActor.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACellActor::ACellActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	SetRootComponent(FloorMesh);
	FloorMesh->SetCollisionProfileName(TEXT("BlockAll"));

	UStaticMeshComponent* Wall;
	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall_North"));
	Wall->SetupAttachment(GetRootComponent());
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	Wall->SetRelativeLocationAndRotation(FVector::ForwardVector * (CellHalfSize - WallHalfThickness), FRotator::ZeroRotator);
	WallMeshes.Add(Wall);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall_East"));
	Wall->SetupAttachment(GetRootComponent());
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	Wall->SetRelativeLocationAndRotation(FVector::RightVector * (CellHalfSize - WallHalfThickness), FRotator(0,90,0));
	WallMeshes.Add(Wall);

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall_South"));
	Wall->SetupAttachment(GetRootComponent());
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	Wall->SetRelativeLocationAndRotation(FVector::BackwardVector * (CellHalfSize - WallHalfThickness), FRotator::ZeroRotator);
	WallMeshes.Add(Wall);


	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall_West"));
	Wall->SetupAttachment(GetRootComponent());
	Wall->SetCollisionProfileName(TEXT("BlockAll"));
	Wall->SetRelativeLocationAndRotation(FVector::LeftVector * (CellHalfSize - WallHalfThickness), FRotator(0, -90, 0));
	WallMeshes.Add(Wall);


	UStaticMeshComponent* Gate;
	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate_North"));
	Gate->SetupAttachment(GetRootComponent());
	Gate->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Gate->SetRelativeLocationAndRotation(FVector::ForwardVector * (CellHalfSize - WallHalfThickness), FRotator::ZeroRotator);
	GateMeshes.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate_East"));
	Gate->SetupAttachment(GetRootComponent());
	Gate->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Gate->SetRelativeLocationAndRotation(FVector::RightVector * (CellHalfSize - WallHalfThickness), FRotator(0, 90, 0));
	GateMeshes.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate_South"));
	Gate->SetupAttachment(GetRootComponent());
	Gate->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Gate->SetRelativeLocationAndRotation(FVector::BackwardVector * (CellHalfSize - WallHalfThickness), FRotator::ZeroRotator);
	GateMeshes.Add(Gate);


	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate_West"));
	Gate->SetupAttachment(GetRootComponent());
	Gate->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Gate->SetRelativeLocationAndRotation(FVector::LeftVector * (CellHalfSize - WallHalfThickness), FRotator(0, -90, 0));
	GateMeshes.Add(Gate);

	UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("North_Arrow"));
	Arrow->SetupAttachment(GetRootComponent());
	Arrow->SetRelativeLocation(FVector(0, 0, 200));

	// TakeDamage의 대상에서도 뺀다
	this->SetCanBeDamaged(false);
}

void ACellActor::InitializeCell(FCellData* InCellData)
{
	if (!InCellData) return;
	Path = static_cast<int32>(InCellData->PathCanGo);
	OpenGate();
}

// Called when the game starts or when spawned
void ACellActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACellActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACellActor::TestPath()
{
	UE_LOG(LogTemp, Log, TEXT("TestPath"));
	OpenGate();
}

void ACellActor::OpenGate()
{
	constexpr int DirCnt = 4;
	for (int i = 0; i < 4; i++)
	{
		EDirType Dir = static_cast<EDirType>(1 << i);
		if (IsPath(Dir))
		{
			GateMeshes[i]->SetVisibility(false);
			GateMeshes[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			GateMeshes[i]->SetVisibility(true);
			GateMeshes[i]->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}
	}
}

bool ACellActor::IsPath(EDirType InDir)
{
	return (static_cast<EDirType>(Path) & InDir) != EDirType::None;
}

