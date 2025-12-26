// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Kismet/GameplayStatics.h"


// BaDaoZhanAbilitySystemLibrary.cpp
void UBaDaoZhanAbilitySystemLibrary::SetViewModeTag(const AActor* TargetActor, FGameplayTag NewCameraModeTag)
{
	// Get ASC
	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(TargetActor);
	if (!ASI) return;
	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (!ASC) return;

	// Make tags
	static FGameplayTag CameraModeParentTag = FGameplayTag::RequestGameplayTag(TEXT("BaDaoZhan.Camera.Mode"));
	if (!NewCameraModeTag.MatchesTag(CameraModeParentTag)) {return;}
	FGameplayTagContainer CurrentTags;
	
	// Remove all CameraModeTag
	ASC->GetOwnedGameplayTags(CurrentTags);
	for (const FGameplayTag& Tag : CurrentTags)
	{
		if (Tag.MatchesTag(CameraModeParentTag))
		{
			ASC->RemoveLooseGameplayTag(Tag);
		}
	}

	// Add new CameraModeTag
	ASC->AddLooseGameplayTag(NewCameraModeTag);
}