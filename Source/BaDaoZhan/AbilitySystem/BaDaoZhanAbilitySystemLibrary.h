// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaDaoZhanAbilitySystemLibrary.generated.h"

UENUM(BlueprintType)
enum class ECameraViewMode : uint8
{
	TD,	
	TP,
	Transition,
	NonTransition
};

/**
 * 
 */
UCLASS()
class BADAOZHAN_API UBaDaoZhanAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable, meta = ( WorldContext = "WorldContextObject"),Category = "ViewMode")
	static void SetViewModeTag(const UObject* WorldContextObject, ECameraViewMode NewViewMode);

};
