// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaDaoZhanCameraLibrary.generated.h"

UENUM(BlueprintType)
enum class ECameraViewMode : uint8
{
	TD			UMETA(DisplayName = "Top-Down"),
	TP			UMETA(DisplayName = "Third-Person"),
	Transition	UMETA(DisplayName = "Transition")
};

/**
 * 相机视角模式工具库
 */
UCLASS()
class BADAOZHAN_API UBaDaoZhanCameraLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 设置相机视角模式（自动操作主角 ASC 上的 GameplayTag） */
	UFUNCTION(BlueprintCallable, Category = "BaDaoZhan|Camera", meta = (WorldContext = "WorldContextObject"))
	static void SetCameraViewMode(const UObject* WorldContextObject, ECameraViewMode NewMode);

	/** 获取当前相机视角模式 */
	UFUNCTION(BlueprintPure, Category = "BaDaoZhan|Camera", meta = (WorldContext = "WorldContextObject"))
	static ECameraViewMode GetCameraViewMode(const UObject* WorldContextObject);

	/** 快捷判断 */
	UFUNCTION(BlueprintPure, Category = "BaDaoZhan|Camera", meta = (WorldContext = "WorldContextObject"))
	static bool IsCameraModeTD(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "BaDaoZhan|Camera", meta = (WorldContext = "WorldContextObject"))
	static bool IsCameraModeTP(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "BaDaoZhan|Camera", meta = (WorldContext = "WorldContextObject"))
	static bool IsCameraModeTransition(const UObject* WorldContextObject);
};



