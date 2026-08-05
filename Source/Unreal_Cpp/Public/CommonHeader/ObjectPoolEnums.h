

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EObjectPoolPolicy : uint8
{
	DoNotSpawn		UMETA(ToolTip = "더 이상 생성하지 않음"),
	ForceGrow		UMETA(ToolTip = "사이즈 무시하고 강제 생성"),
	ReUseOldest		UMETA(ToolTip = "가장 오래된 액터 재사용")
};