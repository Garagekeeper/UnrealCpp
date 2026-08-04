
#include "Enemy/DummyActor.h"
#include "Framework/SubSystem/ObjectPoolSubsystem.h"
#include "Enemy/DamagePopupActor.h"
#include "Widget/DamagePopupWidget.h"
#include "Test/NiagaraSpawnActor.h"

// Sets default values
ADummyActor::ADummyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	PopupTransform = CreateDefaultSubobject<USceneComponent>(TEXT("PopupTransform"));
	PopupTransform->SetupAttachment(Mesh);
	PopupTransform->SetRelativeLocation(FVector(0, 0, 70));

}

// Called when the game starts or when spawned
void ADummyActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADummyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ADummyActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UObjectPoolSubsystem* SubSystem = GameInstance->GetSubsystem<UObjectPoolSubsystem>();
		AActor* SpawnedActor = SubSystem->Spawn(DamagePopup->GetDefaultObject<AActor>(),PopupTransform->GetComponentTransform());
		ADamagepopupActor* Popup = Cast<ADamagepopupActor>(SpawnedActor);
		if(Popup)
			Popup->OnPopupStart(Damage);

		SpawnedActor = SubSystem->Spawn(NiagaraPopup->GetDefaultObject<AActor>(), PopupTransform->GetComponentTransform());
		ANiagaraSpawnActor* Mia = Cast<ANiagaraSpawnActor>(SpawnedActor);
		if (Mia)
			Mia->OnEffectStart(Damage);
	}

	return Damage;
}

