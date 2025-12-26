// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanAbilitySystemComponent.h"
// #include "MeshPaintVisualize.h"

UBaDaoZhanAbilitySystemComponent::UBaDaoZhanAbilitySystemComponent()
{

}

void UBaDaoZhanAbilitySystemComponent::OnAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UBaDaoZhanAbilitySystemComponent::EffectApplied);
}

void UBaDaoZhanAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for(TSubclassOf<UGameplayAbility> GA : StartupAbilities)
	{
		const FGameplayAbilitySpec GASpec = FGameplayAbilitySpec(GA,1);
		GiveAbility(GASpec);
	}
}

void UBaDaoZhanAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	// Print message for any effect applied	
	// GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Blue,FString("EffectApplied"));
	FGameplayTagContainer TagContainer;
	GESpec.GetAllAssetTags(TagContainer);
	for (const FGameplayTag& Tag : TagContainer)
	{
		OnEffectAppliedGetTagDelegate.Broadcast(GetOwner(),Tag);
	}
}