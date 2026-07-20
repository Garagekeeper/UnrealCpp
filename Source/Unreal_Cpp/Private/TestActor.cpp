// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("언리얼 시작"));
	UE_LOG(LogTemp, Log, TEXT("BeginPlay : Log"));
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay : Warning"));
	UE_LOG(LogTemp, Error, TEXT("BeginPlay : Err"));

	Test_NormalFnuction();
	Test_UFunction();
	Test_ImplementableFunc();
	Test_NativeEnventFunc();
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MainMesh->AddRelativeLocation(FVector(0, 0, 1)* MoveSpeed * DeltaTime);
}

void ATestActor::Test_NormalFnuction()
{
	UE_LOG(LogTemp, Warning, TEXT("Test_NormalFnuction : Warning"));
}

void ATestActor::Test_UFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Test_UFunction : Warning"));
}

void ATestActor::Test_NativeEnventFunc_Implementation()
{
}

