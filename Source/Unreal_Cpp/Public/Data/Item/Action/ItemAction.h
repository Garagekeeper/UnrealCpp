

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemAction.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class UNREAL_CPP_API UItemAction : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "ItemAction")
	void ExecuteAction(AActor* InInstigator, AActor* InTarget);
	
	// Abstract 클래스의 경우 BlueprintNativeEvent의 순수 가상함수는 불가능하기에 바디가 필요하다.
	// UHT 의 제약
	virtual void ExecuteAction_Implementation(AActor* InInstigator, AActor* InTarget) {};
};
