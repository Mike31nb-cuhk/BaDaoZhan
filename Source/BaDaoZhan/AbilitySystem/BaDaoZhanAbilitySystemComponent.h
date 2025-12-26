// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaDaoZhanAbilitySystemComponent.generated.h"

class UBaDaoZhanUserWidget;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEffectAppliedDelegate, AActor*, const FGameplayTag /*AssetTags*/);

UCLASS()
class BADAOZHAN_API UBaDaoZhanAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	UBaDaoZhanAbilitySystemComponent();


public:
	void OnAbilityActorInfoSet();
	
	FOnEffectAppliedDelegate OnEffectAppliedGetTagDelegate;
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>&);
	
protected:
	void EffectApplied(UAbilitySystemComponent*, const FGameplayEffectSpec&, FActiveGameplayEffectHandle);
};