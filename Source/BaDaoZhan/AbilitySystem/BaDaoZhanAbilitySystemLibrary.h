// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaDaoZhanAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BADAOZHAN_API UBaDaoZhanAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "ViewMode")
	static void SetViewModeTag(const AActor* TargetActor, FGameplayTag NewViewModeTag);

};
