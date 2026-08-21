

#pragma once

#include "CoreMinimal.h"
#include "Data/Item/Action/ItemAction.h"
#include "ItemAction_Heal.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API UItemAction_Heal : public UItemAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemAtion|Money");
	int32 Heal = 100;

public:
	virtual void ExecuteAction_Implementation(AActor* InInstigator, AActor* InTarget)override;
	
	
};
