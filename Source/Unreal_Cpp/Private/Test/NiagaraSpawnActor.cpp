


#include "Test/NiagaraSpawnActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"



// Sets default values
ANiagaraSpawnActor::ANiagaraSpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	PopupTransform = CreateDefaultSubobject<USceneComponent>(TEXT("NiagaraTransform"));
}

// Called when the game starts or when spawned
void ANiagaraSpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANiagaraSpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANiagaraSpawnActor::OnEffectStart(float InDamage)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		this,
		DamagePopupWidgetComponent, PopupTransform->GetComponentTransform().GetLocation());

	GetWorldTimerManager().SetTimer(
		MyTimerHandle,
		this,
		&ANiagaraSpawnActor::ReturnPoolObject,
		1,
		false
	);
}


void ANiagaraSpawnActor::ReturnPoolObject()
{

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
		SubSystem->ReturnPool(this);
	}
}

void ANiagaraSpawnActor::Onspawn_Implementation()
{
	SetActorHiddenInGame(false);
}

void ANiagaraSpawnActor::OnReturn_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorLocation(FVector(0, 0, -10000.0f));
}
