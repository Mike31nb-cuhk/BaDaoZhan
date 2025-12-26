// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BADAOZHAN_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// detection-related
	virtual void HighlightActor(float) = 0;
	virtual void UnHightlightActor() = 0;
	virtual FVector GetLocation() = 0;
	virtual float GetRadius() = 0;
	virtual UAbilitySystemComponent* GetEnemyAbilitySystemComponent() = 0;
	// 
};