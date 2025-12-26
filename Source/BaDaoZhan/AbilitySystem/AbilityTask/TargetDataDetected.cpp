// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataDetected.h"
#include "AbilitySystemComponent.h"
#include "BaDaoZhan/PlayerController/BaDaoZhanPlayerController.h"

UTargetDataDetected* UTargetDataDetected::CreateTargetDataDetected(UGameplayAbility* OwningAbility)
{
	UTargetDataDetected* MyObj = NewAbilityTask<UTargetDataDetected>(OwningAbility);
	return MyObj;
}

void UTargetDataDetected::Activate()
{
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	check(PC)
	FDetectionResultData Data = Cast<ABaDaoZhanPlayerController>(PC)->GetDetectedData4BaDaoZhan();
	ValidData.Broadcast(Data.DetectedLocation,Data.bIsSuccessful);
}