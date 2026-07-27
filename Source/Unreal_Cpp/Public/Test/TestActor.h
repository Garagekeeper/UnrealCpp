// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class UNREAL_CPP_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Test_NormalFnuction();

	UFUNCTION(BlueprintCallable)
	void Test_UFunction();

	UFUNCTION(BlueprintImplementableEvent)
	void Test_ImplementableFunc();

	UFUNCTION(BlueprintNativeEvent)
	void Test_NativeEnventFunc();


protected:
	//C++ 전용 변수
	//Unreal system에 영향을 받지 않음
	int32 Data1 = 10;

	UPROPERTY(VisibleAnywhere)
	int32 Data2_1 = 21;

	UPROPERTY(VisibleInstanceOnly)
	int32 Data2_2 = 22;

	UPROPERTY(VisibleDefaultsOnly)
	int32 Data2_3 = 23;

	UPROPERTY(EditAnywhere)
	int32 Data3_1 = 23;

	UPROPERTY(EditDefaultsOnly)
	int32 Data3_2 = 23;

	UPROPERTY(EditInstanceOnly)
	int32 Data3_3 = 23;

	UPROPERTY(BlueprintReadOnly)
	int32 Data4_1 = 41;

	UPROPERTY(BlueprintReadWrite, Category= "TestActor 변수|Visible")
	int32 Data4_2 = 42;

	UPROPERTY(BlueprintReadOnly)
	//옛날 방식
	//UStaticMeshComponent* MainMesh = nullptr;
	TObjectPtr<UStaticMeshComponent> MainMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 10.0f;
};
