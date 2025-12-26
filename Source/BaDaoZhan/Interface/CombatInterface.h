// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BADAOZHAN_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 拔刀出鞘 (To Weapon slot)
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UnEquipWeapon();
	// 收刀入鞘 (To Sheath slot)
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void EquipWeapon();
	// 角色开始看向鼠标
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void OnLookCursor();
	// 角色停止看向鼠标
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void OnUnLookCursor();
	// 设置'FacingTarget'矢量变量
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetFacingTarget(FVector FacingTarget);
};