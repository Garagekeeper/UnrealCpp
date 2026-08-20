// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ActionHUD.generated.h"


class UUserWidget;
class UMainHUDWidget;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API AActionHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	UMainHUDWidget* GetMainHudWidget() const;

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainHUDWidget> MainHudWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMainHUDWidget> MainHudWidgetInstance;
	
};
