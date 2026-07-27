// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ActionHUD.generated.h"


class UUserWidget;

/**
 * 
 */
UCLASS()
class UNREAL_CPP_API AActionHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	UUserWidget* GetMainHudWidget() const;
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainHudWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUserWidget> MainHudWidgetInstance;
	
};
