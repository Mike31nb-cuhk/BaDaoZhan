// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataDetected.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDetectionTargetDataSignature,const FVector&, Data, const bool, bIsSuccessful);
/**
 * 
 */
UCLASS()
class BADAOZHAN_API UTargetDataDetected : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataDetected", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"),BlueprintInternalUseOnly)
	static UTargetDataDetected* CreateTargetDataDetected(UGameplayAbility* OwningAbility);
	
	UPROPERTY(BlueprintAssignable)
	FDetectionTargetDataSignature ValidData;
	
	virtual void Activate() override;
};