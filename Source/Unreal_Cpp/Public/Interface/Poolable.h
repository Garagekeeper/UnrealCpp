

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Poolable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL_CPP_API IPoolable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ReturnPoolObject() = 0;

	UFUNCTION(BlueprintNativeEvent)
	void Onspawn();

	UFUNCTION(BlueprintNativeEvent)
	void OnReturn();
};
