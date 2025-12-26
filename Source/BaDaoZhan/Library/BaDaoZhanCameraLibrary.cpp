// Fill out your copyright notice in the Description page of Project Settings.

#include "BaDaoZhanCameraLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

namespace CameraModeTags
{
	static FGameplayTag TD;
	static FGameplayTag TP;
	static FGameplayTag Transition;

	static void EnsureInitialized()
	{
		if (!TD.IsValid())
		{
			TD = FGameplayTag::RequestGameplayTag(FName("BaDaoZhan.Camera.Mode.TD"));
			TP = FGameplayTag::RequestGameplayTag(FName("BaDaoZhan.Camera.Mode.TP"));
			Transition = FGameplayTag::RequestGameplayTag(FName("BaDaoZhan.Camera.Mode.Transition"));
		}
	}
}

static UAbilitySystemComponent* GetPlayerASC(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return nullptr;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if (!PlayerPawn) return nullptr;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PlayerPawn))
	{
		return ASI->GetAbilitySystemComponent();
	}
	return nullptr;
}

void UBaDaoZhanCameraLibrary::SetCameraViewMode(const UObject* WorldContextObject, ECameraViewMode NewMode)
{
	UAbilitySystemComponent* ASC = GetPlayerASC(WorldContextObject);
	if (!ASC) return;

	CameraModeTags::EnsureInitialized();

	// 清除所有模式 Tag
	ASC->RemoveLooseGameplayTag(CameraModeTags::TD);
	ASC->RemoveLooseGameplayTag(CameraModeTags::TP);
	ASC->RemoveLooseGameplayTag(CameraModeTags::Transition);

	// 添加新模式 Tag
	switch (NewMode)
	{
	case ECameraViewMode::TD:
		ASC->AddLooseGameplayTag(CameraModeTags::TD);
		break;
	case ECameraViewMode::TP:
		ASC->AddLooseGameplayTag(CameraModeTags::TP);
		break;
	case ECameraViewMode::Transition:
		ASC->AddLooseGameplayTag(CameraModeTags::Transition);
		break;
	}
}

ECameraViewMode UBaDaoZhanCameraLibrary::GetCameraViewMode(const UObject* WorldContextObject)
{
	UAbilitySystemComponent* ASC = GetPlayerASC(WorldContextObject);
	if (!ASC) return ECameraViewMode::TP; // 默认 TP

	CameraModeTags::EnsureInitialized();

	if (ASC->HasMatchingGameplayTag(CameraModeTags::TD))
	{
		return ECameraViewMode::TD;
	}
	if (ASC->HasMatchingGameplayTag(CameraModeTags::Transition))
	{
		return ECameraViewMode::Transition;
	}
	return ECameraViewMode::TP;
}

bool UBaDaoZhanCameraLibrary::IsCameraModeTD(const UObject* WorldContextObject)
{
	return GetCameraViewMode(WorldContextObject) == ECameraViewMode::TD;
}

bool UBaDaoZhanCameraLibrary::IsCameraModeTP(const UObject* WorldContextObject)
{
	return GetCameraViewMode(WorldContextObject) == ECameraViewMode::TP;
}

bool UBaDaoZhanCameraLibrary::IsCameraModeTransition(const UObject* WorldContextObject)
{
	return GetCameraViewMode(WorldContextObject) == ECameraViewMode::Transition;
}



