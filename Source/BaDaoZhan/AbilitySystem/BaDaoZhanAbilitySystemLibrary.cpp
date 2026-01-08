// Fill out your copyright notice in the Description page of Project Settings.


#include "BaDaoZhanAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "WorldPartition/HLOD/HLODActor.h"


// 借鉴自 UAbilitySystemGlobals::GetAbilitySystemComponentFromActor
static UAbilitySystemComponent* GetASC(const UObject* WorldContextObject)
{
	if (WorldContextObject == nullptr)
	{
		return nullptr;
	}

	const IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(WorldContextObject);
	
	if (ASI){return ASI->GetAbilitySystemComponent();}
	
	ASI = Cast<IAbilitySystemInterface>(WorldContextObject->GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (ASI){return ASI->GetAbilitySystemComponent();}
	
	// I am confident any class with ASC will implement ASI (i.e. no bp first-ASC-owning class).
	// Therefore, no component search will be conducted.

	return nullptr;
}


void UBaDaoZhanAbilitySystemLibrary::SetViewModeTag(const UObject* WorldContextObject, ECameraViewMode NewViewModeTag)
{
	static FGameplayTag TP = FGameplayTag::RequestGameplayTag(TEXT("BaDaoZhan.Camera.Mode.TP"));
	static FGameplayTag TD = FGameplayTag::RequestGameplayTag(TEXT("BaDaoZhan.Camera.Mode.TD"));
	static FGameplayTag Transition = FGameplayTag::RequestGameplayTag(TEXT("BaDaoZhan.Camera.Mode.Transition"));
	
	UAbilitySystemComponent* ASC = GetASC(WorldContextObject);
	if (!ASC){return;}

	switch (NewViewModeTag)
	{
		// Add new CameraModeTag
	case ECameraViewMode::TD:
		if (!ASC->HasMatchingGameplayTag(TD)){ASC->AddLooseGameplayTag(TD);}
		if (ASC->HasMatchingGameplayTag(TP)){ASC->RemoveLooseGameplayTag(TP);}
		break;
	case ECameraViewMode::TP:
		if (!ASC->HasMatchingGameplayTag(TP)){ASC->AddLooseGameplayTag(TP);}
		if (ASC->HasMatchingGameplayTag(TD)){ASC->RemoveLooseGameplayTag(TD);}
		break;
	case ECameraViewMode::Transition:
		ASC->AddLooseGameplayTag(Transition);
		break;
	case ECameraViewMode::NonTransition:
		ASC->RemoveLooseGameplayTag(Transition);
		break;
	}
}