// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"
#include "BaDaoZhan/AbilitySystem/BaDaoZhanAbilitySystemComponent.h"
#include "BaDaoZhan/Attributes/BaDaoZhanAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	const UBaDaoZhanAttributeSet* BaDaoZhanAttributeSet = CastChecked<UBaDaoZhanAttributeSet>(AttributeSet);
	
	OnMaxHealthChanged.Broadcast(BaDaoZhanAttributeSet->GetMaxHealth(),BaDaoZhanAttributeSet->GetMaxHealth());
	OnHealthChanged.Broadcast(BaDaoZhanAttributeSet->GetHealth(),BaDaoZhanAttributeSet->GetHealth());
	OnMaxStaminaChanged.Broadcast(BaDaoZhanAttributeSet->GetMaxStamina(),BaDaoZhanAttributeSet->GetMaxStamina());
	OnStaminaChanged.Broadcast(BaDaoZhanAttributeSet->GetStamina(),BaDaoZhanAttributeSet->GetStamina());
	BindCallBackToDependencies();
}

void UOverlayWidgetController::BindCallBackToDependencies()
{
	const UBaDaoZhanAttributeSet* BaDaoZhanAttributeSet = CastChecked<UBaDaoZhanAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaDaoZhanAttributeSet->GetHealthAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data){OnHealthChanged.Broadcast(Data.NewValue,Data.OldValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaDaoZhanAttributeSet->GetMaxHealthAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data){OnMaxHealthChanged.Broadcast(Data.NewValue,Data.OldValue);});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaDaoZhanAttributeSet->GetStaminaAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data){OnStaminaChanged.Broadcast(Data.NewValue,Data.OldValue);});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaDaoZhanAttributeSet->GetMaxStaminaAttribute()).AddLambda
	([this](const FOnAttributeChangeData& Data){OnMaxStaminaChanged.Broadcast(Data.NewValue,Data.OldValue);});
	
	Cast<UBaDaoZhanAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAppliedGetTagDelegate.AddLambda
	(
		[this](AActor* Target, FGameplayTag Tag)	
		{	
 			if(!Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("BaDaoZhan.Message")))))return;
			// GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Blue, FString(FString::Printf(TEXT("GE:tag %ls"),*Tag.GetTagName().ToString())));
			const FUIWidgetRow* Row = GetDataTableByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
			MessageWidgetRow.Broadcast(Target,*Row);
		}
	);
}