


#include "Widget/DamagePopupWidget.h"
#include "Components/TextBlock.h"

void UDamagePopupWidget::SetDamage(float InDamage)
{
	DamageText->SetText(FText::AsNumber(static_cast<int32>(InDamage)));
}

void UDamagePopupWidget::PlayPopupAnimation()
{
	PlayAnimation(PopupAnimation, 0.0f);
}

void UDamagePopupWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	if (Animation == PopupAnimation)
	{
		OnPopupAnimationFinished.ExecuteIfBound();
	}
}
