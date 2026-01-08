// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaDaoZhan/Interface/EnemyInterface.h"
#include "GameFramework/GameMode.h"
#include "BaDaoZhanBaseGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyListChangedSignature);

/**
 * 
 */
UCLASS()
class BADAOZHAN_API ABaDaoZhanBaseGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Enemy")
	TArray<TScriptInterface<IEnemyInterface>> EnemyList;

	UFUNCTION(Category = "Enemy")
	void AddEnemy(TScriptInterface<IEnemyInterface> Enemy);

	UFUNCTION(Category = "Enemy")
	void RemoveEnemy(TScriptInterface<IEnemyInterface> Enemy);

	FOnEnemyListChangedSignature OnEnemyListChanged;
};