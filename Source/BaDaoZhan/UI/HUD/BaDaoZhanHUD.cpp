// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanHUD.h"

#include "BaDaoZhan/UI/WidgetController/OverlayWidgetController.h"
#include "Blueprint/UserWidget.h"


UOverlayWidgetController* ABaDaoZhanHud::GetOverlayWidgetController(const FWidgetControllerParams WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
	}
		return OverlayWidgetController;
}

void ABaDaoZhanHud::InitOverlay(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized, please fill out at HUD blueprint"))
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized, please fill out at HUD blueprint"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UBaDaoZhanUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValue();
	
	Widget->AddToViewport();
}