// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaDaoZhanWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UBaDaoZhanUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UBaDaoZhanUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

// 选择使用不同的delegate，允许监听委托的类在接受变量时能从变量名了解具体变量信息
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature,float,NewHealth,float,OldHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHealthChangedSignature,float,NewMaxHealth,float,OldMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedSignature,float,NewStamina,float,OldStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxStaminaChangedSignature,float,NewMaxStamina,float,OldMaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMessageWidgetRowSignature,AActor*,EffectTarget,const FUIWidgetRow,Row);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class BADAOZHAN_API UOverlayWidgetController : public UBaDaoZhanWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallBackToDependencies() override;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnStaminaChangedSignature OnStaminaChanged;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FOnMaxStaminaChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	FMessageWidgetRowSignature MessageWidgetRow;
	
	UPROPERTY(EditDefaultsOnly,Category = "WidgetData|Messages")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	// This is substituted with the 'AddLambda' function
	// UPROPERTY(BlueprintAssignable,Category = "GAS|Attributes")
	// FOnGamePlayEffectTriggeredSignature OnGamePlayEffectTriggered;
	
protected:
	
	template<typename T>
	T* GetDataTableByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};


template <typename T>
T* UOverlayWidgetController::GetDataTableByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}