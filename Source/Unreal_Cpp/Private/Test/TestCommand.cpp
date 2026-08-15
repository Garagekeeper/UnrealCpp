


#include "Test/TestCommand.h"
#include "Player/ActionCharacter.h"
#include "Component/InventoryCommandTypes.h"

// Sets default values
ATestCommand::ATestCommand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestCommand::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATestCommand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestCommand::Move()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	AActionCharacter* Player = Cast<AActionCharacter>(PC->GetCharacter());

	if (IInventoryUserInterface* InvecUser = Cast< IInventoryUserInterface>(Player))
	{
		FCommandResult Result;
		InvecUser->ExecuteInventoryCommand(FInventoryCommand::MakeMove(1,2), Result);

	}
}

void ATestCommand::Use()
{

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	AActionCharacter* Player = Cast<AActionCharacter>(PC->GetCharacter());

	if (IInventoryUserInterface* InvecUser = Cast< IInventoryUserInterface>(Player))
	{
		FCommandResult Result;
		InvecUser->ExecuteInventoryCommand(FInventoryCommand::MakeUse(1), Result);

	}
}

void ATestCommand::Drop()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	AActionCharacter* Player = Cast<AActionCharacter>(PC->GetCharacter());

	if (IInventoryUserInterface* InvecUser = Cast< IInventoryUserInterface>(Player))
	{
		FCommandResult Result;
		InvecUser->ExecuteInventoryCommand(FInventoryCommand::MakeDrop(1, FVector(500,500,50)), Result);

	}
}

