

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryUserInterface.generated.h"

class UInventoryComponent;
struct FInventoryCommand;
struct FCommandResult;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryUserInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREAL_CPP_API IInventoryUserInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UInventoryComponent* GetInventoryComponent() const = 0;
	virtual bool ExecuteInventoryCommand(const FInventoryCommand& Command, FCommandResult& OutResult) const = 0;
	
};
