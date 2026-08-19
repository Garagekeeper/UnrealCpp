

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UButton;
class UMoneyPanelWidget;
class UUniformGridPanel;
class UInventoryComponent;
class UInventorySlotWidget;



/**
 *
 */
UCLASS()
class UNREAL_CPP_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 인벤토리 설정용 (인벤토리 컴포넌트랑 위젯을 연결)
	void InitInventoryWidget(UInventoryComponent* InventoryComponent);
	// 인벤토리 초기화용 위젯과 인벤토리 컴포넌트 사이의 연결을 제거
	void ClearInventoryWidget();

	void OpenInventoryWidget();
	void CloseInventoryWidget();

#if WITH_EDITOR
	void TestRefresh();
#endif

protected:
	// 위젯 전체 갱신
	void RefreshInventoryWidget() const;
	// 특정 슬롯 위젯 갱신
	void RefreshSlotWidget(int32 InSlotIndex) const;
	// 머니 패널 위젯 갱신
	void RefreshMoneyPannel(int32 InCurrentMoney) const;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnCloseBtnClicked();

	// 해당 인덱스가 유효한지 확인
	inline bool IsValidIndex(int32 InIndex) const { return InIndex >= 0 && InIndex < SlotSize; };


protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UButton> CloseBtn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UMoneyPanelWidget> MoneyPanel;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Bindwidget))
	TObjectPtr<UUniformGridPanel> SlotGridPannel;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowprivateAccess = "true"))
	TWeakObjectPtr<UInventoryComponent> TargetInventory = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UInventorySlotWidget>> SlotWidgets;

	// 슬롯의 개수 (위젯의 슬롯 개수와 컴포넌트의 슬롯 개수 중 작은 수)
	int32 SlotSize = 0;

};
