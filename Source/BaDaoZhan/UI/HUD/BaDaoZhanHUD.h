// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaDaoZhan/UI/Widget/BaDaoZhanUserWidget.h"
#include "GameFramework/HUD.h"
#include "BaDaoZhanHUD.generated.h"


class UOverlayWidgetController;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class BADAOZHAN_API ABaDaoZhanHud : public AHUD
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	TObjectPtr<UBaDaoZhanUserWidget> OverlayWidget;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UFUNCTION()
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams WCParams);
	
	UFUNCTION(BlueprintCallable)
	void InitOverlay(UAbilitySystemComponent* ASC,UAttributeSet* AS);
	
private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaDaoZhanUserWidget> OverlayWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};