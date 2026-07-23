// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTimer.h"

// Sets default values
ATestTimer::ATestTimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestTimer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestTimer::Test_TimerOnce()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerRepeat : Start"));
	// 현재 레벨을 가져오는 것
	UWorld* World = GetWorld();

	FTimerManager& TimerManager = World -> GetTimerManager();

	//틱 단위인데 사실 필요가 없다네여
	//TimerManager.SetTimerForNextTick

	TimerManager.SetTimer(
		TestOnce, //핸들
		this,// 타이머 오브젝트
		&ATestTimer::Test_MessagePrint1,// 대상 함수
		3.0f,// 얼마 뒤에 (3초 뒤에)
		false// 반복 여부
		);
}

void ATestTimer::Test_TimerRepeat()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerRepeat : Start"));
	// 현재 레벨을 가져오는 것
	UWorld* World = GetWorld();

	FTimerManager& TimerManager = World->GetTimerManager();

	//틱 단위인데 사실 필요가 없다네여
	//TimerManager.SetTimerForNextTick

	TimerManager.SetTimer(
		TestRepeat, //핸들
		this,// 타이머 오브젝트
		&ATestTimer::Test_MessagePrint2,// 대상 함수
		3.0f,// 얼마 뒤에 (3초 뒤에)
		true// 반복 여부
	);
}

void ATestTimer::Test_TimerDelayRepeat()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerDelayRepeat : Start"));
	// 현재 레벨을 가져오는 것
	UWorld* World = GetWorld();

	FTimerManager& TimerManager = World->GetTimerManager();

	//틱 단위인데 사실 필요가 없다네여
	//TimerManager.SetTimerForNextTick

	TimerManager.SetTimer(
		TestDelay, //핸들
		this,// 타이머 오브젝트
		&ATestTimer::Test_MessagePrint3,// 대상 함수
		3.0f,// 얼마 뒤에 (3초 뒤에)
		true,// 반복 여부
		5.0f//처음 선딜
	);
}

void ATestTimer::Test_TimerLamda()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerLamda : Start"));
	// 현재 레벨을 가져오는 것
	UWorld* World = GetWorld();

	FTimerManager& TimerManager = World->GetTimerManager();

	//틱 단위인데 사실 필요가 없다네여
	//TimerManager.SetTimerForNextTick

	TimerManager.SetTimer(
		TestLamda, //핸들
		FTimerDelegate::CreateLambda(
			[]()
			{
				UE_LOG(LogTemp, Log, TEXT("Test_TimerLamda : TimerInvoke"));
			}
		) ,
		3.0f,// 얼마 뒤에 (3초 뒤에)
		true// 반복 여부
	);
}

void ATestTimer::Test_TimerAllClear()
{
	// 현재 레벨을 가져오는 것
	UWorld* World = GetWorld();

	FTimerManager& TimerManager = World->GetTimerManager();

	TimerManager.ClearTimer(TestOnce);
	TimerManager.ClearTimer(TestRepeat);
	TimerManager.ClearTimer(TestDelay);
	TimerManager.ClearTimer(TestLamda);
}

void ATestTimer::Test_MessagePrint1()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerOnce : OnceInvoke"));
}

void ATestTimer::Test_MessagePrint2()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerRepeat : TimerInvoke"));
}

void ATestTimer::Test_MessagePrint3()
{
	UE_LOG(LogTemp, Log, TEXT("Test_TimerDelayRepeat : TimerInvoke"));
}

