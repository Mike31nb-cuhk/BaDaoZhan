// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaDaoZhanBaseCharacter.h"
#include "BaDaoZhan/PlayerController/BaDaoZhanPlayerController.h"
#include "BaDaoZhanCharacter.generated.h"

UCLASS()
class BADAOZHAN_API ABaDaoZhanCharacter : public ABaDaoZhanBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaDaoZhanCharacter();
	UPROPERTY(BlueprintReadWrite)
	ABaDaoZhanPlayerController* BaDaoZhanPC;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void AbilitySystemComponentInitialize();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};